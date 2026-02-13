/* GIMP - The GNU Image Manipulation Program
 * Copyright (C) 1995 Spencer Kimball and Peter Mattis
 *
 * gimpclipboardcompat.c
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

#include "gimpclipboardcompat.h"


GtkClipboard *
gimp_clipboard_compat_get_default (void)
{
  return gtk_clipboard_get_for_display (gdk_display_get_default (),
                                        GDK_SELECTION_CLIPBOARD);
}

GtkClipboard *
gimp_clipboard_compat_get_primary (void)
{
  return gtk_clipboard_get_for_display (gdk_display_get_default (),
                                        GDK_SELECTION_PRIMARY);
}

GObject *
gimp_clipboard_compat_get_owner (GtkClipboard *clipboard)
{
  if (! clipboard)
    return NULL;

  return gtk_clipboard_get_owner (clipboard);
}

gboolean
gimp_clipboard_compat_owner_is (GtkClipboard *clipboard,
                                gpointer      owner)
{
  return (gimp_clipboard_compat_get_owner (clipboard) == G_OBJECT (owner));
}

void
gimp_clipboard_compat_store (GtkClipboard *clipboard)
{
  if (clipboard)
    gtk_clipboard_store (clipboard);
}

void
gimp_clipboard_compat_clear (GtkClipboard *clipboard)
{
  if (clipboard)
    gtk_clipboard_clear (clipboard);
}

void
gimp_clipboard_compat_set_text (GtkClipboard *clipboard,
                                const gchar  *text)
{
  if (clipboard)
    gtk_clipboard_set_text (clipboard, text, -1);
}

void
gimp_clipboard_compat_set_with_owner (GtkClipboard         *clipboard,
                                      const GtkTargetEntry *targets,
                                      guint                 n_targets,
                                      GtkClipboardGetFunc   get_func,
                                      GObject              *owner)
{
  if (clipboard)
    {
      gtk_clipboard_set_with_owner (clipboard,
                                    (GtkTargetEntry *) targets,
                                    n_targets,
                                    get_func,
                                    NULL,
                                    owner);
    }
}

void
gimp_clipboard_compat_set_can_store (GtkClipboard         *clipboard,
                                     const GtkTargetEntry *targets,
                                     gint                  n_targets)
{
  if (clipboard)
    gtk_clipboard_set_can_store (clipboard,
                                 (GtkTargetEntry *) targets,
                                 n_targets);
}

GtkSelectionData *
gimp_clipboard_compat_wait_for_contents (GtkClipboard *clipboard,
                                         GdkAtom       target)
{
  if (! clipboard)
    return NULL;

  return gtk_clipboard_wait_for_contents (clipboard, target);
}
