/* LIBGIMP - The GIMP Library
 * Copyright (C) 1995-1997 Peter Mattis and Spencer Kimball
 *
 * test-frame.c
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

static void
test_frame_new (void)
{
  GtkWidget   *frame;
  const gchar *label_text;

  frame = gimp_frame_new ("Test Frame");
  g_assert_nonnull (frame);

  label_text = gtk_frame_get_label (GTK_FRAME (frame));
  g_assert_cmpstr (label_text, ==, "Test Frame");

  gtk_widget_destroy (frame);
}

static void
test_frame_indent (void)
{
  GtkWidget *frame;
  GtkWidget *child;
  gint       margin_start;

  frame = gimp_frame_new ("Test Frame");
  child = gtk_label_new ("Child");
  gtk_container_add (GTK_CONTAINER (frame), child);

  /* Margin should be applied automatically */
  margin_start = gtk_widget_get_margin_start (child);
  g_assert_cmpint (margin_start, >, 0);

  gtk_widget_destroy (frame);
}

static void
test_frame_label_bold (void)
{
  GtkWidget       *frame;
  GtkWidget       *label_widget;
  PangoAttrList   *attrs;
  PangoAttrIterator *iter;
  gboolean         has_bold = FALSE;

  frame = gimp_frame_new ("Test Frame");
  label_widget = gtk_frame_get_label_widget (GTK_FRAME (frame));
  g_assert_true (GTK_IS_LABEL (label_widget));

  attrs = gtk_label_get_attributes (GTK_LABEL (label_widget));
  g_assert_nonnull (attrs);

  iter = pango_attr_list_get_iterator (attrs);
  do
    {
      PangoAttribute *attr = pango_attr_iterator_get (iter, PANGO_ATTR_WEIGHT);
      if (attr && ((PangoAttrInt *) attr)->value == PANGO_WEIGHT_BOLD)
        {
          has_bold = TRUE;
          break;
        }
    }
  while (pango_attr_iterator_next (iter));
  pango_attr_iterator_destroy (iter);

  g_assert_true (has_bold);

  gtk_widget_destroy (frame);
}

int
main (int   argc,
      char *argv[])
{
  gtk_test_init (&argc, &argv, NULL);

  g_test_add_func ("/gimp-frame/new",        test_frame_new);
  g_test_add_func ("/gimp-frame/indent",     test_frame_indent);
  g_test_add_func ("/gimp-frame/label-bold", test_frame_label_bold);

  return g_test_run ();
}
