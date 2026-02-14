/* LIBGIMP - The GIMP Library
 * Copyright (C) 1995-1997 Peter Mattis and Spencer Kimball
 *
 * gimpwidgets-compat.c
 * Copyright (C) 2026
 *
 * This library is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see
 * <https://www.gnu.org/licenses/>.
 */

#include "config.h"

#include <gtk/gtk.h>

#include "gimpwidgets-compat.h"


void
gimp_widgets_compat_box_pack_start (GtkBox    *box,
                                    GtkWidget *child,
                                    gboolean   expand,
                                    gboolean   fill,
                                    guint      padding)
{
#if GTK_CHECK_VERSION(4, 0, 0)
  (void) expand;
  (void) fill;
  (void) padding;

  gtk_box_append (box, child);
#else
  gtk_box_pack_start (box, child, expand, fill, padding);
#endif
}

void
gimp_widgets_compat_container_add (GtkWidget *container,
                                   GtkWidget *child)
{
#if GTK_CHECK_VERSION(4, 0, 0)
  if (GTK_IS_BUTTON (container))
    gtk_button_set_child (GTK_BUTTON (container), child);
  else if (GTK_IS_SCROLLED_WINDOW (container))
    gtk_scrolled_window_set_child (GTK_SCROLLED_WINDOW (container), child);
  else if (GTK_IS_LIST_BOX_ROW (container))
    gtk_list_box_row_set_child (GTK_LIST_BOX_ROW (container), child);
  else if (GTK_IS_WINDOW (container))
    gtk_window_set_child (GTK_WINDOW (container), child);
  else
    g_warning ("gimp_widgets_compat_container_add() unsupported container type: %s",
               G_OBJECT_TYPE_NAME (container));
#else
  gtk_container_add (GTK_CONTAINER (container), child);
#endif
}
