#ifndef _GPIO_KEYS_H
#define _GPIO_KEYS_H

struct gpio_key {
	unsigned int gpio;
	unsigned int polarity;

	unsigned int debounce_ms;

	unsigned int scancode;
};

extern int gpio_keys_init(struct gpio_key *keys, unsigned int num_keys);

#endif /* _GPIO_KEYS_H */
