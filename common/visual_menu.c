/*
 * Copyright 2010-2011 Calxeda, Inc.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <common.h>
#include <malloc.h>
#include <errno.h>
#include <lcd.h>
#include <linux/list.h>
#include <linux/input.h>
#include <video.h>
#include <visual_menu.h>

/*
 * Internally, each item in a menu is represented by a struct visual_menu_item.
 *
 * These items will be alloc'd and initialized by visual_menu_item_add and destroyed
 * by visual_menu_item_destroy, and the consumer of the interface never sees that
 * this struct is used at all.
 */
struct visual_menu_item {
	char *key;
	void *data;
	struct list_head list;
};

/*
 * The menu is composed of a list of items along with settings and callbacks
 * provided by the user. An incomplete definition of this struct is available
 * in menu.h, but the full definition is here to prevent consumers from
 * relying on its contents.
 */
struct visual_menu {
	struct visual_menu_item *default_item;
	int timeout;
	char *title;
	int prompt;
	struct visual_menu_item *selected;
	int (*read_input)(void *);
	struct list_head items;
	void *priv;
};

/*
 * An iterator function for menu items. callback will be called for each item
 * in m, with m, a pointer to the item, and extra being passed to callback. If
 * callback returns a value other than NULL, iteration stops and the value
 * return by callback is returned from visual_menu_items_iter.  This allows it to be
 * used for search type operations. It is also safe for callback to remove the
 * item from the list of items.
 */
static inline void *visual_menu_items_iter(struct visual_menu *m,
		void *(*callback)(struct visual_menu *, struct visual_menu_item *, void *),
		void *extra)
{
	struct list_head *pos, *n;
	struct visual_menu_item *item;
	void *ret;

	list_for_each_safe(pos, n, &m->items) {
		item = list_entry(pos, struct visual_menu_item, list);

		ret = callback(m, item, extra);

		if (ret)
			return ret;
	}

	return NULL;
}

/*
 * Print a visual_menu_item. If the consumer provided an item_data_print function
 * when creating the menu, call it with a pointer to the item's private data.
 * Otherwise, print the key of the item.
 */
static inline void *visual_menu_item_print(struct visual_menu *m,
				struct visual_menu_item *item,
				void *extra)
{
	if (m->selected == item)
		lcd_puts("> ");
	else
		lcd_puts("  ");

	lcd_puts(item->key);
	lcd_putc('\n');

	return NULL;
}

/*
 * Free the memory used by a menu item. This includes the memory used by its
 * key.
 */
static inline void *visual_menu_item_destroy(struct visual_menu *m,
				struct visual_menu_item *item,
				void *extra)
{
	if (item->key)
		free(item->key);

	free(item);

	return NULL;
}

void __visual_menu_display_statusline(struct visual_menu *m)
{
	return;
}
void visual_menu_display_statusline(struct visual_menu *m)
	__attribute__ ((weak, alias("__visual_menu_display_statusline")));

/*
 * Display a menu so the user can make a choice of an item. First display its
 * title, if any, and then each item in the menu.
 */
static inline void visual_menu_display(struct visual_menu *m)
{
	lcd_clear();

	lcd_puts("---\n");
	if (m->title) {
		lcd_puts(m->title);
		lcd_puts("\n---\n");
	}
	visual_menu_display_statusline(m);

	visual_menu_items_iter(m, visual_menu_item_print, NULL);
	lcd_puts("---\n");
}

/*
 * Check if an item's key matches a provided string, pointed to by extra. If
 * extra is NULL, an item with a NULL key will match. Otherwise, the item's
 * key has to match according to strcmp.
 *
 * This is called via visual_menu_items_iter, so it returns a pointer to the item if
 * the key matches, and returns NULL otherwise.
 */
static inline void *visual_menu_item_key_match(struct visual_menu *m,
			struct visual_menu_item *item, void *extra)
{
	char *item_key = extra;

	if (!item_key || !item->key) {
		if (item_key == item->key)
			return item;

		return NULL;
	}

	if (strcmp(item->key, item_key) == 0)
		return item;

	return NULL;
}

/*
 * Find the first item with a key matching item_key, if any exists.
 */
static inline struct visual_menu_item *visual_menu_item_by_key(struct visual_menu *m,
							char *item_key)
{
	return visual_menu_items_iter(m, visual_menu_item_key_match, item_key);
}

/*
 * Wait for the user to hit a key according to the timeout set for the menu.
 * Returns 1 if the user hit a key, or 0 if the timeout expired.
 */
static inline int visual_menu_interrupted(struct visual_menu *m)
{
	if (!m->timeout)
		return 0;

	if (abortboot(m->timeout))
		return 1;

	return 0;
}

/*
 * Checks whether or not the default menu item should be used without
 * prompting for a user choice. If the menu is set to always prompt, or the
 * user hits a key during the timeout period, return 0. Otherwise, return 1 to
 * indicate we should use the default menu item.
 */
static inline int visual_menu_use_default(struct visual_menu *m)
{
	return !m->prompt && !visual_menu_interrupted(m);
}

/*
 * Set *choice to point to the default item's data, if any default item was
 * set, and returns 1. If no default item was set, returns -ENOENT.
 */
static inline int visual_menu_default_choice(struct visual_menu *m, void **choice)
{
	if (m->default_item) {
		*choice = m->default_item->data;
		return 1;
	}

	return -ENOENT;
}

/*
 * Displays the menu and asks the user to choose an item. *choice will point
 * to the private data of the item the user chooses. The user makes a choice
 * by inputting a string matching the key of an item. Invalid choices will
 * cause the user to be prompted again, repeatedly, until the user makes a
 * valid choice. The user can exit the menu without making a choice via ^c.
 *
 * Returns 1 if the user made a choice, or -EINTR if they bail via ^c.
 */
static int visual_menu_interactive_choice(struct visual_menu *m, void **choice)
{
	int c;
	struct visual_menu_item *pos;;

	pos = list_first_entry(&m->items, struct visual_menu_item, list);
	m->selected = pos;

	while (1) {
		visual_menu_display(m);

		c = m->read_input(m->priv);

		debug("%s: c = %d\n", __func__, c);

		switch (c) {
		case ACTION_UP:
			if (pos->list.prev == &m->items)
				break;
			pos = list_entry(pos->list.prev,
						struct visual_menu_item, list);
			m->selected = pos;
			break;
		case ACTION_DOWN:
			if (pos->list.next == &m->items)
				break;
			pos = list_first_entry(&pos->list,
						struct visual_menu_item, list);
			m->selected = pos;
			break;
		case ACTION_ENTER:
			*choice = m->selected->data;
			lcd_clear();
			return 1;
		case ACTION_EXIT:
			*choice = m->selected->data;
			lcd_clear();
			return -EINTR;
		}
	}
}

/*
 * visual_menu_default_set() - Sets the default choice for the menu. This is safe to
 * call more than once on a menu.
 *
 * m - Points to a menu created by visual_menu_create().
 *
 * item_key - Points to a string that, when compared using strcmp, matches the
 * key for an existing item in the menu.
 *
 * Returns 1 if successful, -EINVAL if m is NULL, or -ENOENT if no item with a
 * key matching item_key is found.
 */
int visual_menu_default_set(struct visual_menu *m, const char *item_key)
{
	struct visual_menu_item *item;

	if (!m)
		return -EINVAL;

	item = visual_menu_item_by_key(m, item_key);
	if (!item)
		return -ENOENT;

	m->default_item = item;

	return 1;
}

/*
 * visual_menu_get_choice() - Returns the user's selected menu entry, or the default
 * if the menu is set to not prompt or the timeout expires. This is safe to
 * call more than once.
 *
 * m - Points to a menu created by visual_menu_create().
 *
 * choice - Points to a location that will store a pointer to the selected
 * menu item. If no item is selected or there is an error, no value will be
 * written at the location it points to.
 *
 * Returns 1 if successful, -EINVAL if m or choice is NULL, -ENOENT if no
 * default has been set and the menu is set to not prompt or the timeout
 * expires, or -EINTR if the user exits the menu via ^c.
 */
int visual_menu_get_choice(struct visual_menu *m, void **choice)
{
	if (!m || !choice || list_empty(&m->items))
		return -EINVAL;

	if (visual_menu_use_default(m))
		return visual_menu_default_choice(m, choice);

	return visual_menu_interactive_choice(m, choice);
}

/*
 * visual_menu_item_add() - Adds or replaces a menu item. Note that this replaces the
 * data of an item if it already exists, but doesn't change the order of the
 * item.
 *
 * m - Points to a menu created by visual_menu_create().
 *
 * item_key - Points to a string that will uniquely identify the item.  The
 * string will be copied to internal storage, and is safe to discard after
 * passing to visual_menu_item_add.
 *
 * item_data - An opaque pointer associated with an item. It is never
 * dereferenced internally, but will be passed to the item_data_print, and
 * will be returned from visual_menu_get_choice if the menu item is selected.
 *
 * Returns 1 if successful, -EINVAL if m is NULL, or -ENOMEM if there is
 * insufficient memory to add the menu item.
 */
int visual_menu_item_add(struct visual_menu *m,
					const char *item_key, void *item_data)
{
	struct visual_menu_item *item;

	if (!m)
		return -EINVAL;

	item = visual_menu_item_by_key(m, item_key);

	if (item) {
		item->data = item_data;
		return 1;
	}

	item = malloc(sizeof *item);
	if (!item)
		return -ENOMEM;

	item->key = strdup(item_key);

	if (!item->key) {
		free(item);
		return -ENOMEM;
	}

	item->data = item_data;

	list_add_tail(&item->list, &m->items);

	if (!m->default_item)
		m->default_item = item;

	return 1;
}

/*
 * visual_menu_create() - Creates a menu handle with default settings
 *
 * title - If not NULL, points to a string that will be displayed before the
 * list of menu items. It will be copied to internal storage, and is safe to
 * discard after passing to visual_menu_create().
 *
 * timeout - A delay in seconds to wait for user input. If 0, timeout is
 * disabled, and the default choice will be returned unless prompt is 1.
 *
 * prompt - If 0, don't ask for user input unless there is an interrupted
 * timeout. If 1, the user will be prompted for input regardless of the value
 * of timeout.
 *
 * item_data_print - If not NULL, will be called for each item when the menu
 * is displayed, with the pointer to the item's data passed as the argument.
 * If NULL, each item's key will be printed instead.  Since an item's key is
 * what must be entered to select an item, the item_data_print function should
 * make it obvious what the key for each entry is.
 *
 * Returns a pointer to the menu if successful, or NULL if there is
 * insufficient memory available to create the menu.
 */
struct visual_menu *visual_menu_create(const char *title, int timeout,
			int prompt, int (*read_input)(void *), void *priv)
{
	struct visual_menu *m;

	m = malloc(sizeof *m);
	if (!m)
		return NULL;

	m->default_item = NULL;
	m->prompt = prompt;
	m->timeout = timeout;
	m->read_input = read_input;
	m->priv = priv;

	if (title) {
		m->title = strdup(title);
		if (!m->title) {
			free(m);
			return NULL;
		}
	} else
		m->title = NULL;


	INIT_LIST_HEAD(&m->items);

	return m;
}

/*
 * visual_menu_destroy() - frees the memory used by a menu and its items.
 *
 * m - Points to a menu created by visual_menu_create().
 *
 * Returns 1 if successful, or -EINVAL if m is NULL.
 */
int visual_menu_destroy(struct visual_menu *m)
{
	if (!m)
		return -EINVAL;

	visual_menu_items_iter(m, visual_menu_item_destroy, NULL);

	if (m->title)
		free(m->title);

	free(m);

	return 1;
}
