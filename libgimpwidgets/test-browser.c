/* LIBGIMP - The GIMP Library
 * Copyright (C) 1995-1997 Peter Mattis and Spencer Kimball
 *
 * test-browser.c
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

#define GIMP_BROWSER_LEFT_VBOX_DATA_KEY    "gimp-browser-left-vbox"
#define GIMP_BROWSER_SEARCH_ENTRY_DATA_KEY "gimp-browser-search-entry"
#define GIMP_BROWSER_COUNT_LABEL_DATA_KEY  "gimp-browser-count-label"
#define GIMP_BROWSER_RIGHT_VBOX_DATA_KEY   "gimp-browser-right-vbox"

typedef struct
{
  gboolean search_emitted;
  gchar   *search_string;
  gint     search_type;
} TestData;

static void
on_browser_search (GimpBrowser *browser,
                   const gchar *string,
                   gint         type,
                   TestData    *data)
{
  data->search_emitted = TRUE;
  g_free (data->search_string);
  data->search_string = g_strdup (string);
  data->search_type = type;
}

static void
test_browser_layout (void)
{
  GtkWidget *browser;
  GtkWidget *vbox;

  browser = gimp_browser_new ();
  g_assert_nonnull (browser);

  vbox = g_object_get_data (G_OBJECT (browser), GIMP_BROWSER_LEFT_VBOX_DATA_KEY);
  g_assert_true (GTK_IS_BOX (vbox));

  vbox = g_object_get_data (G_OBJECT (browser), GIMP_BROWSER_RIGHT_VBOX_DATA_KEY);
  g_assert_true (GTK_IS_BOX (vbox));

  gtk_widget_destroy (browser);
}

static gboolean
timeout_callback (gpointer data)
{
  g_main_loop_quit ((GMainLoop *) data);
  return FALSE;
}

static void
test_browser_search (void)
{
  GtkWidget *browser;
  GtkWidget *entry;
  TestData   data = { FALSE, NULL, 0 };
  GMainLoop *loop;

  browser = gimp_browser_new ();
  entry = g_object_get_data (G_OBJECT (browser), GIMP_BROWSER_SEARCH_ENTRY_DATA_KEY);
  g_assert_true (GTK_IS_SEARCH_ENTRY (entry));

  g_signal_connect (browser, "search",
                    G_CALLBACK (on_browser_search), &data);

  /* Set search string */
  gtk_entry_set_text (GTK_ENTRY (entry), "test-query");

  /* Wait for timeout (100ms in GimpBrowser + some margin) */
  loop = g_main_loop_new (NULL, FALSE);
  g_timeout_add (200, timeout_callback, loop);
  g_main_loop_run (loop);
  g_main_loop_unref (loop);

  g_assert_true (data.search_emitted);
  g_assert_cmpstr (data.search_string, ==, "test-query");

  g_free (data.search_string);
  gtk_widget_destroy (browser);
}

int
main (int   argc,
      char *argv[])
{
  gtk_test_init (&argc, &argv, NULL);

  g_test_add_func ("/gimp-browser/layout", test_browser_layout);
  g_test_add_func ("/gimp-browser/search", test_browser_search);

  return g_test_run ();
}
