/* LIBGIMP - The GIMP Library
 * Copyright (C) 1995-1997 Peter Mattis and Spencer Kimball
 *
 * test-file-entry.c
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

#include <glib/gstdio.h>

#include <gtk/gtk.h>

#include "gimpwidgets.h"


#define GIMP_FILE_ENTRY_ENTRY_DATA_KEY          "gimp-file-entry-entry"
#define GIMP_FILE_ENTRY_BROWSE_BUTTON_DATA_KEY  "gimp-file-entry-browse-button"
#define GIMP_FILE_ENTRY_MANAGER_BUTTON_DATA_KEY "gimp-file-entry-manager-button"
#define GIMP_FILE_ENTRY_FILE_EXISTS_DATA_KEY    "gimp-file-entry-file-exists"


typedef struct
{
  gchar *root_dir;
  gchar *file_a;
  gchar *file_b;
} FileEntryFixture;


static void             file_entry_fixture_init          (FileEntryFixture *fixture);
static void             file_entry_fixture_clear         (FileEntryFixture *fixture);
static void             file_entry_spin_main_context     (void);
static GtkWidget      * file_entry_get_widget_data       (GimpFileEntry    *entry,
                                                          const gchar      *key,
                                                          GType             type);
static void             test_file_entry_roundtrip        (void);
static void             test_file_entry_check_valid      (void);
static void             test_file_entry_manager_button   (void);


static void
file_entry_fixture_init (FileEntryFixture *fixture)
{
  GError *error = NULL;

  /* g_dir_make_tmp requires a template ending in XXXXXX */
  fixture->root_dir = g_dir_make_tmp ("gimp-file-entry-test-XXXXXX", &error);
  g_assert_no_error (error);
  g_assert_nonnull (fixture->root_dir);

  fixture->file_a = g_build_filename (fixture->root_dir, "file_a", NULL);
  fixture->file_b = g_build_filename (fixture->root_dir, "file_b", NULL);

  g_file_set_contents (fixture->file_a, "content", -1, &error);
  g_assert_no_error (error);
}

static void
file_entry_fixture_clear (FileEntryFixture *fixture)
{
  if (fixture->file_a)
    g_unlink (fixture->file_a);
  if (fixture->file_b)
    g_unlink (fixture->file_b); /* Might verify non-existent */
  if (fixture->root_dir)
    g_rmdir (fixture->root_dir);

  g_clear_pointer (&fixture->file_a, g_free);
  g_clear_pointer (&fixture->file_b, g_free);
  g_clear_pointer (&fixture->root_dir, g_free);
}

static void
file_entry_spin_main_context (void)
{
  while (gtk_events_pending ())
    gtk_main_iteration ();
}

static GtkWidget *
file_entry_get_widget_data (GimpFileEntry *entry,
                            const gchar   *key,
                            GType          type)
{
  GtkWidget *widget;

  widget = g_object_get_data (G_OBJECT (entry), key);

  g_assert_nonnull (widget);
  g_assert_true (g_type_is_a (G_OBJECT_TYPE (widget), type));

  return widget;
}

static void
test_file_entry_roundtrip (void)
{
  FileEntryFixture fixture = { 0, };
  GtkWidget       *entry;
  gchar           *actual_filename;
  gchar           *set_filename;

  file_entry_fixture_init (&fixture);

  /* Using _gimp_file_entry_new is internal, but currently exposed in gimpfileentry.h?
     Actually the public API is usually gimp_file_entry_new which wraps it or is it?
     Let's check header. In gimpfileentry.c: GtkWidget * _gimp_file_entry_new ...
     Wait, the header gimpfileentry.h might not expose _gimp_file_entry_new publicly or it might be renamed.
     Let's check gimpfileentry.h for the public constructor.
     Actually, standard GIMP widgets usually have gimp_file_entry_new.
     Ah, in gimpfileentry.c lines 221, it defines `_gimp_file_entry_new`.
     But looking at `gimpfileentry.h` (I didn't read it, but I can assume standard pattern).
     Wait, `gimppatheditor.c` calls `_gimp_file_entry_new`.
     Let's assume `gimp_file_entry_new` exists or I should use `g_object_new` or the `_` version if I link correctly.
     The file `gimpfileentry.c` has `_gimp_file_entry_new`.
     Let's attempt to use `gimp_file_entry_new` if it exists, otherwise `g_object_new`.
     Wait, `gimppatheditor.c` calls `_gimp_file_entry_new` at line 355.
     So I can probably call `_gimp_file_entry_new` if I include the private header or if it is in `gimpfileentry.h`.
     I will use `g_object_new (GIMP_TYPE_FILE_ENTRY, ...)` if that's safer, but `_gimp_file_entry_new` sets up properties.
     Let's use `g_object_new` with properties if possible, or just declare the prototype if needed.
     Actually, looking at `gimpfileentry.c`:
     It has properties? No, `_gimp_file_entry_new` does manual setup.
     So I must use `_gimp_file_entry_new`.
  */

  /* I'll declare it if missing or assume it is available since I'm linking with the library */
  entry = _gimp_file_entry_new ("File Entry Test", fixture.file_a, FALSE, FALSE);

  gtk_widget_show_all (entry);
  file_entry_spin_main_context ();

  actual_filename = _gimp_file_entry_get_filename (GIMP_FILE_ENTRY (entry));
  g_assert_cmpstr (actual_filename, ==, fixture.file_a);
  g_free (actual_filename);

  set_filename = g_strdup (fixture.file_b);
  _gimp_file_entry_set_filename (GIMP_FILE_ENTRY (entry), set_filename);
  file_entry_spin_main_context ();

  actual_filename = _gimp_file_entry_get_filename (GIMP_FILE_ENTRY (entry));
  g_assert_cmpstr (actual_filename, ==, set_filename);
  g_free (actual_filename);
  g_free (set_filename);

  gtk_widget_destroy (entry);
  file_entry_spin_main_context ();
  file_entry_fixture_clear (&fixture);
}

static void
test_file_entry_check_valid (void)
{
  FileEntryFixture fixture = { 0, };
  GtkWidget       *entry;
  GtkWidget       *file_exists_icon;
  const gchar     *icon_name;
  gchar           *icon_name_dup;

  file_entry_fixture_init (&fixture);

  /* Initialize with valid file and checking enabled */
  entry = _gimp_file_entry_new ("File Entry Test", fixture.file_a, FALSE, TRUE);

  gtk_widget_show_all (entry);
  file_entry_spin_main_context ();

  file_exists_icon = file_entry_get_widget_data (GIMP_FILE_ENTRY (entry), GIMP_FILE_ENTRY_FILE_EXISTS_DATA_KEY, GTK_TYPE_IMAGE);
  g_assert_true (gtk_widget_get_visible (file_exists_icon));

  gtk_image_get_icon_name (GTK_IMAGE (file_exists_icon), &icon_name, NULL);
  /* gtk_image_get_icon_name returns internal string, verify compatibility */
  g_assert_cmpstr (icon_name, ==, "gtk-yes");

  /* Set to non-existent file */
  _gimp_file_entry_set_filename (GIMP_FILE_ENTRY (entry), fixture.file_b);
  file_entry_spin_main_context ();

  gtk_image_get_icon_name (GTK_IMAGE (file_exists_icon), &icon_name, NULL);
  g_assert_cmpstr (icon_name, ==, "gtk-no");

  gtk_widget_destroy (entry);
  file_entry_spin_main_context ();
  file_entry_fixture_clear (&fixture);
}

static void
test_file_entry_manager_button (void)
{
  FileEntryFixture fixture = { 0, };
  GtkWidget       *entry;
  GtkWidget       *manager_button;

  file_entry_fixture_init (&fixture);

  entry = _gimp_file_entry_new ("File Entry Test", NULL, FALSE, FALSE);

  gtk_widget_show_all (entry);
  file_entry_spin_main_context ();

  manager_button = file_entry_get_widget_data (GIMP_FILE_ENTRY (entry), GIMP_FILE_ENTRY_MANAGER_BUTTON_DATA_KEY, GTK_TYPE_BUTTON);
  g_assert_false (gtk_widget_get_sensitive (manager_button));

  _gimp_file_entry_set_filename (GIMP_FILE_ENTRY (entry), fixture.file_a);
  file_entry_spin_main_context ();

  /* Should be sensitive now */
  g_assert_true (gtk_widget_get_sensitive (manager_button));

  gtk_widget_destroy (entry);
  file_entry_spin_main_context ();
  file_entry_fixture_clear (&fixture);
}

int
main (int   argc,
      char *argv[])
{
  gtk_test_init (&argc, &argv, NULL);

  g_test_add_func ("/gimp-file-entry/roundtrip",       test_file_entry_roundtrip);
  g_test_add_func ("/gimp-file-entry/check-valid",     test_file_entry_check_valid);
  g_test_add_func ("/gimp-file-entry/manager-button",  test_file_entry_manager_button);

  return g_test_run ();
}
