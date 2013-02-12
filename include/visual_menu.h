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

#ifndef __VISUAL_MENU_H__
#define __VISUAL_MENU_H__

enum {
	ACTION_NONE,
	ACTION_UP,
	ACTION_DOWN,
	ACTION_ENTER,
	ACTION_EXIT,
};

struct visual_menu;

struct visual_menu *visual_menu_create(const char *title, int timeout,
			int prompt, int (*read_input)(void *), void *priv);
int visual_menu_default_set(struct visual_menu *m, const char *item_key);
int visual_menu_get_choice(struct visual_menu *m, void **choice);
int visual_menu_item_add(struct visual_menu *m,
					const char *item_key, void *item_data);
int visual_menu_destroy(struct visual_menu *m);
void visual_menu_display_statusline(struct visual_menu *m);

#if defined(CONFIG_MENU_SHOW)
int menu_show(int bootdelay);
#endif

#endif /* __VISUAL_MENU_H__ */
