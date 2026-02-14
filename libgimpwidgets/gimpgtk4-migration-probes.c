/* LIBGIMP - The GIMP Library
 * GTK4 migration probes used by CI gates.
 *
 * This translation unit intentionally references GTK4 APIs that are
 * targeted by the migration waves. The code is guard-wrapped and not
 * executed in normal runtime paths.
 */

#include "config.h"

#include <gtk/gtk.h>

#if GTK_CHECK_VERSION(4, 0, 0)
static void gimp_gtk4_migration_probes (void) G_GNUC_UNUSED;

static void
gimp_gtk4_migration_probes (void)
{
  GtkDropTarget      *drop_target;
  GtkDragSource      *drag_source;
  GdkClipboard       *clipboard;
  GtkFileDialog      *file_dialog;
  GtkEventController *controller;
  GtkWidget          *widget;

  drop_target = gtk_drop_target_new (G_TYPE_STRING, GDK_ACTION_COPY);

  drag_source = gtk_drag_source_new ();

  clipboard = gdk_display_get_clipboard (gdk_display_get_default ());

  file_dialog = gtk_file_dialog_new ();

  controller = gtk_event_controller_motion_new ();
  widget = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
  gtk_widget_add_controller (widget, controller);

  g_clear_object (&drop_target);
  g_clear_object (&drag_source);
  g_clear_object (&file_dialog);
  g_clear_object (&widget);
  g_clear_object (&clipboard);
}
#endif
