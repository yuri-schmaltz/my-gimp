/* LIBGIMP - The GIMP Library
 * Copyright (C) 1995-1997 Peter Mattis and Spencer Kimball
 *
 * test-path-editor.c
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

#include "gimpwidgetstypes.h"
#include "gimpwidgets.h"


#define GIMP_PATH_EDITOR_LISTBOX_DATA_KEY       "gimp-path-editor-listbox"
#define GIMP_PATH_EDITOR_ENTRY_DATA_KEY         "gimp-path-editor-entry"
#define GIMP_PATH_EDITOR_NEW_BUTTON_DATA_KEY    "gimp-path-editor-new-button"
#define GIMP_PATH_EDITOR_UP_BUTTON_DATA_KEY     "gimp-path-editor-up-button"
#define GIMP_PATH_EDITOR_DOWN_BUTTON_DATA_KEY   "gimp-path-editor-down-button"
#define GIMP_PATH_EDITOR_DELETE_BUTTON_DATA_KEY "gimp-path-editor-delete-button"
#define GIMP_PATH_EDITOR_ROW_TOGGLE_DATA_KEY    "gimp-path-editor-row-toggle"


typedef struct
{
  gchar *root_dir;
  gchar *dir_a;
  gchar *dir_b;
  gchar *dir_c;
} PathEditorFixture;


static void             path_editor_fixture_init         (PathEditorFixture *fixture);
static void             path_editor_fixture_clear        (PathEditorFixture *fixture);
static gchar          * path_editor_join2                (const gchar       *a,
                                                          const gchar       *b);
static gchar          * path_editor_join3                (const gchar       *a,
                                                          const gchar       *b,
                                                          const gchar       *c);
static void             path_editor_spin_main_context    (void);
static GtkWidget      * path_editor_get_widget_data      (GtkWidget         *editor,
                                                          const gchar       *key,
                                                          GType              type);
static GtkWidget      * path_editor_get_listbox          (GtkWidget         *editor);
static GtkWidget      * path_editor_get_entry            (GtkWidget         *editor);
static GtkWidget      * path_editor_get_button           (GtkWidget         *editor,
                                                          const gchar       *key);
static GtkToggleButton *path_editor_get_row_toggle       (GtkWidget         *editor,
                                                          gint               index);
static gint             path_editor_get_row_count        (GtkWidget         *editor);
static void             path_editor_path_changed         (GimpPathEditor    *editor,
                                                          gpointer           data);
static void             path_editor_writable_changed     (GimpPathEditor    *editor,
                                                          gpointer           data);
static void             test_path_editor_roundtrip       (void);
static void             test_path_editor_reorder_buttons (void);
static void             test_path_editor_add_and_delete  (void);
static void             test_path_editor_writable_toggle (void);


static void
path_editor_fixture_init (PathEditorFixture *fixture)
{
  GError *error = NULL;

  fixture->root_dir = g_dir_make_tmp ("gimp-path-editor-test-XXXXXX", &error);
  g_assert_no_error (error);
  g_assert_nonnull (fixture->root_dir);

  fixture->dir_a = g_build_filename (fixture->root_dir, "a", NULL);
  fixture->dir_b = g_build_filename (fixture->root_dir, "b", NULL);
  fixture->dir_c = g_build_filename (fixture->root_dir, "c", NULL);

  g_assert_cmpint (g_mkdir_with_parents (fixture->dir_a, 0700), ==, 0);
  g_assert_cmpint (g_mkdir_with_parents (fixture->dir_b, 0700), ==, 0);
  g_assert_cmpint (g_mkdir_with_parents (fixture->dir_c, 0700), ==, 0);
}

static void
path_editor_fixture_clear (PathEditorFixture *fixture)
{
  if (fixture->dir_a)
    g_rmdir (fixture->dir_a);
  if (fixture->dir_b)
    g_rmdir (fixture->dir_b);
  if (fixture->dir_c)
    g_rmdir (fixture->dir_c);
  if (fixture->root_dir)
    g_rmdir (fixture->root_dir);

  g_clear_pointer (&fixture->dir_a, g_free);
  g_clear_pointer (&fixture->dir_b, g_free);
  g_clear_pointer (&fixture->dir_c, g_free);
  g_clear_pointer (&fixture->root_dir, g_free);
}

static gchar *
path_editor_join2 (const gchar *a,
                   const gchar *b)
{
  gchar sep[2] = { G_SEARCHPATH_SEPARATOR, '\0' };

  return g_strjoin (sep, a, b, NULL);
}

static gchar *
path_editor_join3 (const gchar *a,
                   const gchar *b,
                   const gchar *c)
{
  gchar sep[2] = { G_SEARCHPATH_SEPARATOR, '\0' };

  return g_strjoin (sep, a, b, c, NULL);
}

static void
path_editor_spin_main_context (void)
{
  while (gtk_events_pending ())
    gtk_main_iteration ();
}

static GtkWidget *
path_editor_get_widget_data (GtkWidget   *editor,
                             const gchar *key,
                             GType        type)
{
  GtkWidget *widget;

  widget = g_object_get_data (G_OBJECT (editor), key);

  g_assert_nonnull (widget);
  g_assert_true (g_type_is_a (G_OBJECT_TYPE (widget), type));

  return widget;
}

static GtkWidget *
path_editor_get_listbox (GtkWidget *editor)
{
  return path_editor_get_widget_data (editor,
                                      GIMP_PATH_EDITOR_LISTBOX_DATA_KEY,
                                      GTK_TYPE_LIST_BOX);
}

static GtkWidget *
path_editor_get_entry (GtkWidget *editor)
{
  return path_editor_get_widget_data (editor,
                                      GIMP_PATH_EDITOR_ENTRY_DATA_KEY,
                                      GTK_TYPE_ENTRY);
}

static GtkWidget *
path_editor_get_button (GtkWidget   *editor,
                        const gchar *key)
{
  return path_editor_get_widget_data (editor, key, GTK_TYPE_BUTTON);
}

static GtkToggleButton *
path_editor_get_row_toggle (GtkWidget *editor,
                            gint       index)
{
  GtkWidget       *listbox;
  GtkListBoxRow   *row;
  GtkToggleButton *toggle;

  listbox = path_editor_get_listbox (editor);
  row = gtk_list_box_get_row_at_index (GTK_LIST_BOX (listbox), index);
  g_assert_nonnull (row);

  toggle = g_object_get_data (G_OBJECT (row), GIMP_PATH_EDITOR_ROW_TOGGLE_DATA_KEY);
  g_assert_nonnull (toggle);

  return toggle;
}

static gint
path_editor_get_row_count (GtkWidget *editor)
{
  GtkWidget *listbox;
  gint       count = 0;

  listbox = path_editor_get_listbox (editor);

  while (gtk_list_box_get_row_at_index (GTK_LIST_BOX (listbox), count))
    count++;

  return count;
}

static void
path_editor_path_changed (GimpPathEditor *editor,
                          gpointer        data)
{
  gint *count = data;

  (void) editor;

  *count += 1;
}

static void
path_editor_writable_changed (GimpPathEditor *editor,
                              gpointer        data)
{
  gint *count = data;

  (void) editor;

  *count += 1;
}

static void
test_path_editor_roundtrip (void)
{
  PathEditorFixture fixture = { 0, };
  GtkWidget        *editor;
  gchar            *initial_path;
  gchar            *actual_path;

  path_editor_fixture_init (&fixture);

  initial_path = path_editor_join3 (fixture.dir_a, fixture.dir_b, fixture.dir_c);
  editor = gimp_path_editor_new ("Path Editor Test", initial_path);

  gtk_widget_show_all (editor);
  path_editor_spin_main_context ();

  actual_path = gimp_path_editor_get_path (GIMP_PATH_EDITOR (editor));
  g_assert_cmpstr (actual_path, ==, initial_path);
  g_assert_cmpint (path_editor_get_row_count (editor), ==, 3);

  g_free (actual_path);
  g_free (initial_path);

  gtk_widget_destroy (editor);
  path_editor_spin_main_context ();
  path_editor_fixture_clear (&fixture);
}

static void
test_path_editor_reorder_buttons (void)
{
  PathEditorFixture fixture = { 0, };
  GtkWidget        *editor;
  GtkWidget        *listbox;
  GtkWidget        *up_button;
  GtkWidget        *down_button;
  GtkListBoxRow    *row;
  gchar            *initial_path;
  gchar            *expected_path;
  gchar            *actual_path;
  gint              path_changed_count = 0;

  path_editor_fixture_init (&fixture);

  initial_path = path_editor_join3 (fixture.dir_a, fixture.dir_b, fixture.dir_c);
  editor = gimp_path_editor_new ("Path Editor Test", initial_path);
  g_signal_connect (editor, "path-changed",
                    G_CALLBACK (path_editor_path_changed),
                    &path_changed_count);

  gtk_widget_show_all (editor);
  path_editor_spin_main_context ();

  listbox = path_editor_get_listbox (editor);
  up_button = path_editor_get_button (editor, GIMP_PATH_EDITOR_UP_BUTTON_DATA_KEY);
  down_button = path_editor_get_button (editor, GIMP_PATH_EDITOR_DOWN_BUTTON_DATA_KEY);

  row = gtk_list_box_get_row_at_index (GTK_LIST_BOX (listbox), 1);
  g_assert_nonnull (row);
  gtk_list_box_select_row (GTK_LIST_BOX (listbox), row);
  path_editor_spin_main_context ();

  gtk_button_clicked (GTK_BUTTON (up_button));
  path_editor_spin_main_context ();

  expected_path = path_editor_join3 (fixture.dir_b, fixture.dir_a, fixture.dir_c);
  actual_path = gimp_path_editor_get_path (GIMP_PATH_EDITOR (editor));
  g_assert_cmpstr (actual_path, ==, expected_path);
  g_free (actual_path);
  g_free (expected_path);

  gtk_button_clicked (GTK_BUTTON (down_button));
  path_editor_spin_main_context ();

  actual_path = gimp_path_editor_get_path (GIMP_PATH_EDITOR (editor));
  g_assert_cmpstr (actual_path, ==, initial_path);
  g_assert_cmpint (path_changed_count, >=, 2);

  g_free (actual_path);
  g_free (initial_path);

  gtk_widget_destroy (editor);
  path_editor_spin_main_context ();
  path_editor_fixture_clear (&fixture);
}

static void
test_path_editor_add_and_delete (void)
{
  PathEditorFixture fixture = { 0, };
  GtkWidget        *editor;
  GtkWidget        *listbox;
  GtkWidget        *new_button;
  GtkWidget        *delete_button;
  GtkWidget        *entry;
  GtkListBoxRow    *row;
  gchar            *initial_path;
  gchar            *expected_path;
  gchar            *actual_path;
  gint              path_changed_count = 0;

  path_editor_fixture_init (&fixture);

  initial_path = g_strdup (fixture.dir_a);
  editor = gimp_path_editor_new ("Path Editor Test", initial_path);
  g_signal_connect (editor, "path-changed",
                    G_CALLBACK (path_editor_path_changed),
                    &path_changed_count);

  gtk_widget_show_all (editor);
  path_editor_spin_main_context ();

  listbox = path_editor_get_listbox (editor);
  entry = path_editor_get_entry (editor);
  new_button = path_editor_get_button (editor, GIMP_PATH_EDITOR_NEW_BUTTON_DATA_KEY);
  delete_button = path_editor_get_button (editor, GIMP_PATH_EDITOR_DELETE_BUTTON_DATA_KEY);

  gtk_button_clicked (GTK_BUTTON (new_button));
  path_editor_spin_main_context ();

  gtk_entry_set_text (GTK_ENTRY (entry), fixture.dir_b);
  g_signal_emit_by_name (entry, "activate");
  path_editor_spin_main_context ();

  expected_path = path_editor_join2 (fixture.dir_a, fixture.dir_b);
  actual_path = gimp_path_editor_get_path (GIMP_PATH_EDITOR (editor));
  g_assert_cmpstr (actual_path, ==, expected_path);
  g_assert_cmpint (path_editor_get_row_count (editor), ==, 2);
  g_free (actual_path);
  g_free (expected_path);

  row = gtk_list_box_get_row_at_index (GTK_LIST_BOX (listbox), 1);
  g_assert_nonnull (row);
  gtk_list_box_select_row (GTK_LIST_BOX (listbox), row);
  path_editor_spin_main_context ();

  gtk_button_clicked (GTK_BUTTON (delete_button));
  path_editor_spin_main_context ();

  actual_path = gimp_path_editor_get_path (GIMP_PATH_EDITOR (editor));
  g_assert_cmpstr (actual_path, ==, fixture.dir_a);
  g_assert_cmpint (path_changed_count, >=, 2);
  g_free (actual_path);

  g_free (initial_path);

  gtk_widget_destroy (editor);
  path_editor_spin_main_context ();
  path_editor_fixture_clear (&fixture);
}

static void
test_path_editor_writable_toggle (void)
{
  PathEditorFixture fixture = { 0, };
  GtkWidget        *editor;
  gchar            *initial_path;
  gchar            *expected_path;
  gchar            *actual_path;
  gint              writable_changed_count = 0;
  GtkToggleButton  *toggle_a;
  GtkToggleButton  *toggle_b;

  path_editor_fixture_init (&fixture);

  initial_path = path_editor_join2 (fixture.dir_a, fixture.dir_b);
  editor = gimp_path_editor_new ("Path Editor Test", initial_path);
  g_signal_connect (editor, "writable-changed",
                    G_CALLBACK (path_editor_writable_changed),
                    &writable_changed_count);

  gtk_widget_show_all (editor);
  path_editor_spin_main_context ();

  gimp_path_editor_set_writable_path (GIMP_PATH_EDITOR (editor), fixture.dir_a);
  path_editor_spin_main_context ();

  actual_path = gimp_path_editor_get_writable_path (GIMP_PATH_EDITOR (editor));
  g_assert_cmpstr (actual_path, ==, fixture.dir_a);
  g_free (actual_path);

  toggle_a = path_editor_get_row_toggle (editor, 0);
  toggle_b = path_editor_get_row_toggle (editor, 1);
  g_assert_true (gtk_widget_get_visible (GTK_WIDGET (toggle_a)));
  g_assert_true (gtk_widget_get_visible (GTK_WIDGET (toggle_b)));

  gtk_toggle_button_set_active (toggle_b, TRUE);
  path_editor_spin_main_context ();

  expected_path = path_editor_join2 (fixture.dir_a, fixture.dir_b);
  actual_path = gimp_path_editor_get_writable_path (GIMP_PATH_EDITOR (editor));
  g_assert_cmpstr (actual_path, ==, expected_path);
  g_free (actual_path);
  g_free (expected_path);

  gtk_toggle_button_set_active (toggle_a, FALSE);
  path_editor_spin_main_context ();

  actual_path = gimp_path_editor_get_writable_path (GIMP_PATH_EDITOR (editor));
  g_assert_cmpstr (actual_path, ==, fixture.dir_b);
  g_assert_cmpint (writable_changed_count, >=, 2);
  g_free (actual_path);

  g_free (initial_path);

  gtk_widget_destroy (editor);
  path_editor_spin_main_context ();
  path_editor_fixture_clear (&fixture);
}

int
main (int   argc,
      char *argv[])
{
  gtk_test_init (&argc, &argv, NULL);

  g_test_add_func ("/gimp-path-editor/roundtrip",       test_path_editor_roundtrip);
  g_test_add_func ("/gimp-path-editor/reorder-buttons", test_path_editor_reorder_buttons);
  g_test_add_func ("/gimp-path-editor/add-and-delete",  test_path_editor_add_and_delete);
  g_test_add_func ("/gimp-path-editor/writable-toggle", test_path_editor_writable_toggle);

  return g_test_run ();
}
