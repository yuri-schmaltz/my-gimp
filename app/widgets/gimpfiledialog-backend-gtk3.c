/* GIMP - The GNU Image Manipulation Program
 * Copyright (C) 1995 Spencer Kimball and Peter Mattis
 *
 * gimpfiledialog-backend-gtk3.c
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

#include "gimpfiledialog-backend-gtk3.h"
#include "gimp-intl.h"


void
gimp_file_dialog_backend_gtk3_add_default_buttons (GtkDialog   *dialog,
                                                   const gchar *ok_button_label)
{
  gtk_dialog_add_buttons (dialog,
                          _("_Cancel"),     GTK_RESPONSE_CANCEL,
                          ok_button_label,  GTK_RESPONSE_OK,
                          NULL);
}

void
gimp_file_dialog_backend_gtk3_configure_chooser (GtkFileChooser *chooser,
                                                 gboolean        local_only,
                                                 gboolean        overwrite_confirmation)
{
  gtk_file_chooser_set_local_only (chooser, local_only);
  gtk_file_chooser_set_do_overwrite_confirmation (chooser, overwrite_confirmation);
}

void
gimp_file_dialog_backend_gtk3_set_extra_widget (GtkFileChooser *chooser,
                                                GtkWidget      *widget)
{
  gtk_file_chooser_set_extra_widget (chooser, widget);
}

void
gimp_file_dialog_backend_gtk3_pack_progress (GtkDialog *dialog,
                                             GtkWidget *progress_widget)
{
  gtk_box_pack_end (GTK_BOX (gtk_dialog_get_content_area (dialog)),
                    progress_widget, FALSE, FALSE, 0);
}

void
gimp_file_dialog_backend_gtk3_add_shortcut_folder (GtkFileChooser *chooser,
                                                   const gchar    *folder_path)
{
  gtk_file_chooser_add_shortcut_folder (chooser, folder_path, NULL);
}
