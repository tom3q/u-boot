/*
 * GPIO keys driver for generic input subsystem
 *
 * (C) Copyright 2012 Tomasz Figa <tomasz.figa at gmail.com>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */ 

#include <common.h>
#include <exports.h>
#include <linux/input.h>
#include <input.h>
#include <stdio_dev.h>
#include <asm/gpio.h>
#include <gpio_keys.h>

struct gpio_key_priv {
	struct gpio_key *key;

	unsigned int debounce_ticks;

	unsigned int state;
	unsigned int debouncing;
	unsigned int timer;
};

struct gpio_keys {
	struct input_config input;
	struct gpio_key_priv *keys;
	unsigned int num_keys;
	int *fifo;
};

static struct gpio_keys keys;

static int gpio_keys_handle_key(struct gpio_keys *data,
						struct gpio_key_priv *priv)
{
	struct gpio_key *key = priv->key;
	int val = gpio_get_value(key->gpio) ^ key->polarity;
	unsigned long ticks;

	if (!key->debounce_ms) {
		priv->state = val;
		return val;
	}

	if (priv->state == val) {
		priv->debouncing = 0;
		return priv->state;
	}

	if (!priv->debouncing) {
		priv->timer = get_ticks();
		priv->debouncing = 1;
		return priv->state;
	}

	ticks = get_ticks();
	if (ticks < priv->timer
	    || ticks > (priv->timer + priv->debounce_ticks)) {
		priv->debouncing = 0;
		priv->state = val;
	}

	return priv->state;
}

static int gpio_keys_poll(struct input_config *input)
{
	struct gpio_keys *data = container_of(input, struct gpio_keys, input);
	unsigned int i;
	unsigned int cnt = 0;

	for (i = 0; i < data->num_keys; ++i) {
		if (gpio_keys_handle_key(data, &data->keys[i]))
			data->fifo[cnt++] = data->keys[i].key->scancode;
	}

	input_send_keycodes(&data->input, data->fifo, cnt);

	return 0;
}

/**
 * Test if keys are available to be read
 *
 * @return 0 if no keys available, 1 if keys are available
 */
static int kbd_tstc(void)
{
	/* Just get input to do this for us */
	return input_tstc(&keys.input);
}

/**
 * Read a key
 *
 * TODO: U-Boot wants 0 for no key, but Ctrl-@ is a valid key...
 *
 * @return ASCII key code, or 0 if no key, or -1 if error
 */
static int kbd_getc(void)
{
	/* Just get input to do this for us */
	return input_getc(&keys.input);
}

static int init_gpio_keys(void)
{
	return 0;
}

int gpio_keys_init(struct gpio_key *keys_data, unsigned int num_keys)
{
	struct gpio_key_priv *priv = malloc(sizeof(*priv)*num_keys);
	struct stdio_dev dev;
	unsigned int i;
	int error;

	if (input_init(&keys.input, 0)) {
		debug("%s: Cannot set up input\n", __func__);
		return -1;
	}

	keys.keys = priv;
	keys.num_keys = num_keys;
	keys.fifo = malloc(num_keys * sizeof(int));

	memset(priv, 0, sizeof(*priv)*num_keys);
	for (i = 0; i < num_keys; ++i) {
		gpio_direction_input(keys_data[i].gpio);
		priv[i].key = &keys_data[i];
		priv[i].debounce_ticks =
				(keys_data[i].debounce_ms * get_tbclk()) / 1000;
	}

	keys.input.read_keys = gpio_keys_poll;

	memset(&dev, '\0', sizeof(dev));
	strcpy(dev.name, "gpio-keys");
	dev.flags = DEV_FLAGS_INPUT | DEV_FLAGS_SYSTEM;
	dev.getc = kbd_getc;
	dev.tstc = kbd_tstc;
	dev.start = init_gpio_keys;

	/* Register the device. init_tegra_keyboard() will be called soon */
	input_stdio_register(&dev);
	if (error) {
		debug("%s: Cannot set up stdio\n", __func__);
		return error;
	}

	return 0;
}
