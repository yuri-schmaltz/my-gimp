/* GIMP - The GNU Image Manipulation Program
 * Copyright (C) 1995 Spencer Kimball and Peter Mattis
 *
 * gimpshortcutbridge.c
 * Copyright (C) 2026
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "config.h"

#include <gtk/gtk.h>

#include "gimpshortcutbridge.h"


#define GIMP_SHORTCUT_BRIDGE_DATA_KEY "gimp-shortcut-bridge-accel-group"


static GtkAccelGroup *
gimp_shortcut_bridge_get_accel_group (GtkWindow *window)
{
  GtkAccelGroup *group;

  group = g_object_get_data (G_OBJECT (window), GIMP_SHORTCUT_BRIDGE_DATA_KEY);

  if (! group)
    {
      group = gtk_accel_group_new ();
      gtk_window_add_accel_group (window, group);
      g_object_set_data_full (G_OBJECT (window), GIMP_SHORTCUT_BRIDGE_DATA_KEY,
                              group, g_object_unref);
    }

  return group;
}

void
gimp_shortcut_bridge_add_action_accel (GtkWindow      *window,
                                       GtkApplication *application,
                                       const gchar    *action_name,
                                       GCallback       callback,
                                       gpointer        user_data)
{
  GtkAccelGroup  *group;
  gchar         **accels;
  guint           accel_key;
  GdkModifierType accel_mods;

  g_return_if_fail (GTK_IS_WINDOW (window));
  g_return_if_fail (GTK_IS_APPLICATION (application));
  g_return_if_fail (action_name != NULL);
  g_return_if_fail (callback != NULL);

  accels = gtk_application_get_accels_for_action (application, action_name);
  if (! accels || ! accels[0] || accels[0][0] == '\0')
    {
      g_strfreev (accels);
      return;
    }

  accel_key  = 0;
  accel_mods = 0;
  gtk_accelerator_parse (accels[0], &accel_key, &accel_mods);
  g_strfreev (accels);

  if (accel_key == 0)
    return;

  group = gimp_shortcut_bridge_get_accel_group (window);

  gtk_accel_group_connect (group,
                           accel_key, accel_mods, 0,
                           g_cclosure_new (callback, user_data, NULL));
}
