/* LIBGIMP - The GIMP Library
 * Copyright (C) 1995-1997 Peter Mattis and Spencer Kimball
 *
 * test-chain-button.c
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

#define GIMP_CHAIN_BUTTON_IMAGE_DATA_KEY  "gimp-chain-button-image"
#define GIMP_CHAIN_BUTTON_BUTTON_DATA_KEY "gimp-chain-button-button"
#define GIMP_CHAIN_BUTTON_LINE1_DATA_KEY  "gimp-chain-button-line1"
#define GIMP_CHAIN_BUTTON_LINE2_DATA_KEY  "gimp-chain-button-line2"

static void
test_chain_button_toggle (void)
{
  GtkWidget *chain;
  gboolean   active;

  chain = gimp_chain_button_new (GIMP_CHAIN_TOP);
  g_assert_nonnull (chain);

  /* Default should be inactive */
  g_assert_false (gimp_chain_button_get_active (GIMP_CHAIN_BUTTON (chain)));

  /* Toggle active */
  gimp_chain_button_set_active (GIMP_CHAIN_BUTTON (chain), TRUE);
  g_assert_true (gimp_chain_button_get_active (GIMP_CHAIN_BUTTON (chain)));

  /* Toggle back */
  gimp_chain_button_set_active (GIMP_CHAIN_BUTTON (chain), FALSE);
  g_assert_false (gimp_chain_button_get_active (GIMP_CHAIN_BUTTON (chain)));

  gtk_widget_destroy (chain);
}

static void
test_chain_button_clicked (void)
{
  GtkWidget *chain;
  GtkWidget *button;

  chain = gimp_chain_button_new (GIMP_CHAIN_TOP);
  button = g_object_get_data (G_OBJECT (chain), GIMP_CHAIN_BUTTON_BUTTON_DATA_KEY);
  g_assert_nonnull (button);

  g_assert_false (gimp_chain_button_get_active (GIMP_CHAIN_BUTTON (chain)));

  /* Simulate click */
  gtk_button_clicked (GTK_BUTTON (button));
  g_assert_true (gimp_chain_button_get_active (GIMP_CHAIN_BUTTON (chain)));

  /* Click again */
  gtk_button_clicked (GTK_BUTTON (button));
  g_assert_false (gimp_chain_button_get_active (GIMP_CHAIN_BUTTON (chain)));

  gtk_widget_destroy (chain);
}

static void
test_chain_button_icon_update (void)
{
  GtkWidget   *chain;
  GtkWidget   *image;
  const gchar *icon_name;

  chain = gimp_chain_button_new (GIMP_CHAIN_TOP);
  image = g_object_get_data (G_OBJECT (chain), GIMP_CHAIN_BUTTON_IMAGE_DATA_KEY);
  g_assert_nonnull (image);

  /* Inactive Top (Horizontal) */
  g_assert_false (gimp_chain_button_get_active (GIMP_CHAIN_BUTTON (chain)));
  gtk_image_get_icon_name (GTK_IMAGE (image), &icon_name, NULL);
  g_assert_cmpstr (icon_name, ==, GIMP_ICON_CHAIN_HORIZONTAL_BROKEN);

  /* Active Top */
  gimp_chain_button_set_active (GIMP_CHAIN_BUTTON (chain), TRUE);
  gtk_image_get_icon_name (GTK_IMAGE (image), &icon_name, NULL);
  g_assert_cmpstr (icon_name, ==, GIMP_ICON_CHAIN_HORIZONTAL);

  gtk_widget_destroy (chain);

  /* Vertical chain */
  chain = gimp_chain_button_new (GIMP_CHAIN_LEFT);
  image = g_object_get_data (G_OBJECT (chain), GIMP_CHAIN_BUTTON_IMAGE_DATA_KEY);

  /* Inactive Left (Vertical) */
  gtk_image_get_icon_name (GTK_IMAGE (image), &icon_name, NULL);
  g_assert_cmpstr (icon_name, ==, GIMP_ICON_CHAIN_VERTICAL_BROKEN);

  /* Active Left */
  gimp_chain_button_set_active (GIMP_CHAIN_BUTTON (chain), TRUE);
  gtk_image_get_icon_name (GTK_IMAGE (image), &icon_name, NULL);
  g_assert_cmpstr (icon_name, ==, GIMP_ICON_CHAIN_VERTICAL);

  gtk_widget_destroy (chain);
}

int
main (int   argc,
      char *argv[])
{
  gtk_test_init (&argc, &argv, NULL);

  g_test_add_func ("/gimp-chain-button/toggle",       test_chain_button_toggle);
  g_test_add_func ("/gimp-chain-button/clicked",      test_chain_button_clicked);
  g_test_add_func ("/gimp-chain-button/icon-update",  test_chain_button_icon_update);

  return g_test_run ();
}
