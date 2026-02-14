/* LIBGIMP - The GIMP Library
 * Copyright (C) 1995-1997 Peter Mattis and Spencer Kimball
 *
 * test-busybox.c
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

#include "gimpwidgets.h"

#define GIMP_BUSY_BOX_SPINNER_DATA_KEY "gimp-busy-box-spinner"
#define GIMP_BUSY_BOX_LABEL_DATA_KEY   "gimp-busy-box-label"

static void
test_busy_box_message (void)
{
  GtkWidget *busy;
  GtkWidget *label;

  busy = gimp_busy_box_new ("Working...");
  g_assert_nonnull (busy);

  label = g_object_get_data (G_OBJECT (busy), GIMP_BUSY_BOX_LABEL_DATA_KEY);
  g_assert_true (GTK_IS_LABEL (label));

  g_assert_cmpstr (gimp_busy_box_get_message (GIMP_BUSY_BOX (busy)), ==, "Working...");
  g_assert_cmpstr (gtk_label_get_text (GTK_LABEL (label)), ==, "Working...");

  gimp_busy_box_set_message (GIMP_BUSY_BOX (busy), "Almost done...");
  g_assert_cmpstr (gimp_busy_box_get_message (GIMP_BUSY_BOX (busy)), ==, "Almost done...");

  gtk_widget_destroy (busy);
}

static void
test_busy_box_structure (void)
{
  GtkWidget *busy;
  GtkWidget *spinner;

  busy = gimp_busy_box_new (NULL);
  spinner = g_object_get_data (G_OBJECT (busy), GIMP_BUSY_BOX_SPINNER_DATA_KEY);
  g_assert_true (GTK_IS_SPINNER (spinner));

  gtk_widget_destroy (busy);
}

int
main (int   argc,
      char *argv[])
{
  gtk_test_init (&argc, &argv, NULL);

  g_test_add_func ("/gimp-busy-box/message",   test_busy_box_message);
  g_test_add_func ("/gimp-busy-box/structure", test_busy_box_structure);

  return g_test_run ();
}
