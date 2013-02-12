#ifndef _SAMSUNG_KEYPAD_H
#define _SAMSUNG_KEYPAD_H

#include <matrix_keypad.h>

#define SAMSUNG_MAX_ROWS	8
#define SAMSUNG_MAX_COLS	8

/**
 * struct samsung_keypad_platdata - Platform device data for Samsung Keypad.
 * @keymap_data: pointer to &matrix_keymap_data.
 * @rows: number of keypad row supported.
 * @cols: number of keypad col supported.
 * @no_autorepeat: disable key autorepeat.
 * @wakeup: controls whether the device should be set up as wakeup source.
 * @cfg_gpio: configure the GPIO.
 *
 * Initialisation data specific to either the machine or the platform
 * for the device driver to use or call-back when configuring gpio.
 */
struct samsung_keypad_platdata {
	const struct matrix_keymap_data	*keymap_data;
	unsigned int rows;
	unsigned int cols;
};

extern void samsung_keypad_init(struct samsung_keypad_platdata *pdata);

#endif /* _SAMSUNG_KEYPAD_H */
