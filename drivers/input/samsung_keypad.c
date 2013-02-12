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
#include <samsung_keypad.h>
#include <stdio_dev.h>
#include <asm/io.h>

#define SAMSUNG_KEYIFCON			0x00
#define SAMSUNG_KEYIFSTSCLR			0x04
#define SAMSUNG_KEYIFCOL			0x08
#define SAMSUNG_KEYIFROW			0x0c
#define SAMSUNG_KEYIFFC				0x10

/* SAMSUNG_KEYIFCON */
#define SAMSUNG_KEYIFCON_INT_F_EN		(1 << 0)
#define SAMSUNG_KEYIFCON_INT_R_EN		(1 << 1)
#define SAMSUNG_KEYIFCON_DF_EN			(1 << 2)
#define SAMSUNG_KEYIFCON_FC_EN			(1 << 3)
#define SAMSUNG_KEYIFCON_WAKEUPEN		(1 << 4)

/* SAMSUNG_KEYIFSTSCLR */
#define SAMSUNG_KEYIFSTSCLR_P_INT_MASK		(0xff << 0)
#define SAMSUNG_KEYIFSTSCLR_R_INT_MASK		(0xff << 8)
#define SAMSUNG_KEYIFSTSCLR_R_INT_OFFSET	8

/* SAMSUNG_KEYIFCOL */
#define SAMSUNG_KEYIFCOL_MASK			(0xff << 0)

/* SAMSUNG_KEYIFROW */
#define SAMSUNG_KEYIFROW_MASK			(0xff << 0)

/* SAMSUNG_KEYIFFC */
#define SAMSUNG_KEYIFFC_MASK			(0x3ff << 0)

#define KEYPAD_MAX				64

struct samsung_keypad {
	struct input_config input;
	void *base;
	unsigned int row_shift;
	unsigned int rows;
	unsigned int cols;
	unsigned int row_state[SAMSUNG_MAX_COLS];
	unsigned short keycodes[];
};

static struct samsung_keypad *keypad;

static void samsung_keypad_scan(struct samsung_keypad *keypad,
				unsigned int *row_state)
{
	unsigned int col;
	unsigned int val;

	for (col = 0; col < keypad->cols; col++) {
		val = SAMSUNG_KEYIFCOL_MASK;
		val &= ~(1 << col);

		writel(val, keypad->base + SAMSUNG_KEYIFCOL);
		udelay(1000);

		val = readl(keypad->base + SAMSUNG_KEYIFROW);
		row_state[col] = ~val & ((1 << keypad->rows) - 1);
	}

	/* KEYIFCOL reg clear */
	writel(0, keypad->base + SAMSUNG_KEYIFCOL);
}

static void samsung_keypad_report(struct samsung_keypad *keypad,
				  unsigned int *row_state)
{
	unsigned int changed;
	unsigned int pressed;
	unsigned int key_down = 0;
	unsigned int val;
	unsigned int col, row;
	int fifo[KEYPAD_MAX];
	int cnt = 0;

	for (col = 0; col < keypad->cols; col++) {
		changed = row_state[col] ^ keypad->row_state[col];
		key_down |= row_state[col];
		if (!changed)
			continue;

		for (row = 0; row < keypad->rows; row++) {
			if (!(changed & (1 << row)))
				continue;

			pressed = row_state[col] & (1 << row);
			if (!pressed)
				continue;

			val = MATRIX_SCAN_CODE(row, col, keypad->row_shift);
			if (!keypad->keycodes[val])
				continue;

			fifo[cnt++] = keypad->keycodes[val];
		}
	}

	memcpy(keypad->row_state, row_state, sizeof(keypad->row_state));

	input_send_keycodes(&keypad->input, fifo, cnt);
}

static int samsung_keypad_poll(struct input_config *input)
{
	struct samsung_keypad *keypad = container_of(input,
						struct samsung_keypad, input);
	unsigned int row_state[SAMSUNG_MAX_COLS];

	samsung_keypad_scan(keypad, row_state);

	samsung_keypad_report(keypad, row_state);

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
	return input_tstc(&keypad->input);
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
	return input_getc(&keypad->input);
}

static int init_samsung_keypad(void)
{
	return 0;
}

static __inline__ int get_count_order(unsigned int count)
{
	int order;

	order = fls(count) - 1;
	if (count & (count - 1))
		order++;
	return order;
}

void samsung_keypad_init(struct samsung_keypad_platdata *pdata)
{
	struct stdio_dev dev;
	unsigned int row_shift;
	unsigned int keymap_size;
	const struct matrix_keymap_data *keymap_data;

	row_shift = get_count_order(pdata->cols);
	keymap_size = (pdata->rows << row_shift) * sizeof(keypad->keycodes[0]);

	keypad = malloc(sizeof(*keypad)
				+ keymap_size * sizeof(*keypad->keycodes));
	if (!keypad) {
		printf("%s: failed to allocate driver data\n", __func__);
		return;
	}

	input_init(&keypad->input, 0);

	keypad->base = (void *)samsung_get_base_keypad();
	keypad->row_shift = row_shift;
	keypad->rows = pdata->rows;
	keypad->cols = pdata->cols;

	keymap_data = pdata->keymap_data;
	if (!keymap_data) {
		printf("%s: no keymap data defined\n", __func__);
		return;
	}

	matrix_keypad_build_keymap(keymap_data, row_shift, keypad->keycodes);

	keypad->input.read_keys = samsung_keypad_poll;

	memset(&dev, '\0', sizeof(dev));
	strcpy(dev.name, "samsung-keypad");
	dev.flags = DEV_FLAGS_INPUT | DEV_FLAGS_SYSTEM;
	dev.getc = kbd_getc;
	dev.tstc = kbd_tstc;
	dev.start = init_samsung_keypad;

	/* Register the device. init_tegra_keyboard() will be called soon */
	input_stdio_register(&dev);
}
