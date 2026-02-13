/* GIMP - The GNU Image Manipulation Program
 * Copyright (C) 1995 Spencer Kimball and Peter Mattis
 *
 * gimpfiledialog-backend.c
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

#include "gimpfiledialog-backend.h"
#include "gimpfiledialog-backend-gtk3.h"


void
gimp_file_dialog_backend_add_default_buttons (GtkDialog   *dialog,
                                              const gchar *ok_button_label)
{
  gimp_file_dialog_backend_gtk3_add_default_buttons (dialog, ok_button_label);
}

void
gimp_file_dialog_backend_configure_chooser (GtkFileChooser *chooser,
                                            gboolean        local_only,
                                            gboolean        overwrite_confirmation)
{
  gimp_file_dialog_backend_gtk3_configure_chooser (chooser,
                                                   local_only,
                                                   overwrite_confirmation);
}

void
gimp_file_dialog_backend_set_extra_widget (GtkFileChooser *chooser,
                                           GtkWidget      *widget)
{
  gimp_file_dialog_backend_gtk3_set_extra_widget (chooser, widget);
}

void
gimp_file_dialog_backend_pack_progress (GtkDialog *dialog,
                                        GtkWidget *progress_widget)
{
  gimp_file_dialog_backend_gtk3_pack_progress (dialog, progress_widget);
}

void
gimp_file_dialog_backend_add_shortcut_folder (GtkFileChooser *chooser,
                                              const gchar    *folder_path)
{
  gimp_file_dialog_backend_gtk3_add_shortcut_folder (chooser, folder_path);
}
