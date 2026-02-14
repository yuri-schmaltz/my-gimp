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
gimp_widgets_compat_box_pack_end (GtkBox    *box,
                                  GtkWidget *child,
                                  gboolean   expand,
                                  gboolean   fill,
                                  guint      padding)
{
#if GTK_CHECK_VERSION(4, 0, 0)
  (void) expand;
  (void) fill;
  (void) padding;

  gtk_box_prepend (box, child);
#else
  gtk_box_pack_end (box, child, expand, fill, padding);
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

void
gimp_widgets_compat_container_remove (GtkWidget *container,
                                      GtkWidget *child)
{
#if GTK_CHECK_VERSION(4, 0, 0)
  if (GTK_IS_BOX (container))
    gtk_box_remove (GTK_BOX (container), child);
  else
    g_warning ("gimp_widgets_compat_container_remove() unsupported container type: %s",
               G_OBJECT_TYPE_NAME (container));
#else
  gtk_container_remove (GTK_CONTAINER (container), child);
#endif
}

GtkWidget *
gimp_widgets_compat_bin_get_child (GtkWidget *bin)
{
#if GTK_CHECK_VERSION(4, 0, 0)
  if (GTK_IS_FRAME (bin))
    return gtk_frame_get_child (GTK_FRAME (bin));
  else if (GTK_IS_WINDOW (bin))
    return gtk_window_get_child (GTK_WINDOW (bin));
  else
    return gtk_widget_get_first_child (bin);
#else
  return gtk_bin_get_child (GTK_BIN (bin));
#endif
}

void
gimp_widgets_compat_paned_pack1 (GtkPaned  *paned,
                                 GtkWidget *child,
                                 gboolean   resize,
                                 gboolean   shrink)
{
#if GTK_CHECK_VERSION(4, 0, 0)
  gtk_paned_set_start_child (paned, child);
  gtk_paned_set_resize_start_child (paned, resize);
  gtk_paned_set_shrink_start_child (paned, shrink);
#else
  gtk_paned_pack1 (paned, child, resize, shrink);
#endif
}

void
gimp_widgets_compat_paned_pack2 (GtkPaned  *paned,
                                 GtkWidget *child,
                                 gboolean   resize,
                                 gboolean   shrink)
{
#if GTK_CHECK_VERSION(4, 0, 0)
  gtk_paned_set_end_child (paned, child);
  gtk_paned_set_resize_end_child (paned, resize);
  gtk_paned_set_shrink_end_child (paned, shrink);
#else
  gtk_paned_pack2 (paned, child, resize, shrink);
#endif
}

void
gimp_widgets_compat_widget_set_border_width (GtkWidget *widget,
                                             guint      border_width)
{
#if GTK_CHECK_VERSION(4, 0, 0)
  gtk_widget_set_margin_start  (widget, border_width);
  gtk_widget_set_margin_end    (widget, border_width);
  gtk_widget_set_margin_top    (widget, border_width);
  gtk_widget_set_margin_bottom (widget, border_width);
#else
  gtk_container_set_border_width (GTK_CONTAINER (widget), border_width);
#endif
}

GtkWidget *
gimp_widgets_compat_tree_view_new_with_model (GtkTreeModel *model)
{
#if GTK_CHECK_VERSION(4, 0, 0)
  (void) model;
  return gtk_list_box_new ();
#else
  return gtk_tree_view_new_with_model (model);
#endif
}

void
gimp_widgets_compat_tree_view_set_headers_visible (GtkWidget *tree_view,
                                                   gboolean   visible)
{
#if GTK_CHECK_VERSION(4, 0, 0)
  (void) tree_view;
  (void) visible;
#else
  gtk_tree_view_set_headers_visible (GTK_TREE_VIEW (tree_view), visible);
#endif
}

void
gimp_widgets_compat_tree_view_set_tooltip_column (GtkWidget *tree_view,
                                                  gint       column)
{
#if GTK_CHECK_VERSION(4, 0, 0)
  (void) tree_view;
  (void) column;
#else
  gtk_tree_view_set_tooltip_column (GTK_TREE_VIEW (tree_view), column);
#endif
}

GtkTreeModel *
gimp_widgets_compat_tree_view_get_model (GtkWidget *tree_view)
{
#if GTK_CHECK_VERSION(4, 0, 0)
  (void) tree_view;
  return NULL;
#else
  return gtk_tree_view_get_model (GTK_TREE_VIEW (tree_view));
#endif
}

gpointer
gimp_widgets_compat_tree_view_get_selection (GtkWidget *tree_view)
{
#if GTK_CHECK_VERSION(4, 0, 0)
  (void) tree_view;
  return NULL;
#else
  return gtk_tree_view_get_selection (GTK_TREE_VIEW (tree_view));
#endif
}

void
gimp_widgets_compat_tree_view_append_column (GtkWidget         *tree_view,
                                             GtkTreeViewColumn *column)
{
#if GTK_CHECK_VERSION(4, 0, 0)
  (void) tree_view;
  (void) column;
#else
  gtk_tree_view_append_column (GTK_TREE_VIEW (tree_view), column);
#endif
}

void
gimp_widgets_compat_tree_view_insert_column (GtkWidget         *tree_view,
                                             GtkTreeViewColumn *column,
                                             gint               position)
{
#if GTK_CHECK_VERSION(4, 0, 0)
  (void) tree_view;
  (void) column;
  (void) position;
#else
  gtk_tree_view_insert_column (GTK_TREE_VIEW (tree_view), column, position);
#endif
}

void
gimp_widgets_compat_tree_view_set_headers_clickable (GtkWidget *tree_view,
                                                     gboolean   setting)
{
#if GTK_CHECK_VERSION(4, 0, 0)
  (void) tree_view;
  (void) setting;
#else
  gtk_tree_view_set_headers_clickable (GTK_TREE_VIEW (tree_view), setting);
#endif
}

void
gimp_widgets_compat_tree_selection_set_mode (gpointer         selection,
                                             GtkSelectionMode mode)
{
#if GTK_CHECK_VERSION(4, 0, 0)
  (void) selection;
  (void) mode;
#else
  gtk_tree_selection_set_mode (selection, mode);
#endif
}

gboolean
gimp_widgets_compat_tree_selection_get_selected (gpointer      selection,
                                                 GtkTreeModel **model,
                                                 GtkTreeIter   *iter)
{
#if GTK_CHECK_VERSION(4, 0, 0)
  (void) selection;
  (void) model;
  (void) iter;
  return FALSE;
#else
  return gtk_tree_selection_get_selected (selection, model, iter);
#endif
}

void
gimp_widgets_compat_tree_selection_select_path (gpointer     selection,
                                                GtkTreePath *path)
{
#if GTK_CHECK_VERSION(4, 0, 0)
  (void) selection;
  (void) path;
#else
  gtk_tree_selection_select_path (selection, path);
#endif
}

void
gimp_widgets_compat_tree_selection_unselect_path (gpointer     selection,
                                                  GtkTreePath *path)
{
#if GTK_CHECK_VERSION(4, 0, 0)
  (void) selection;
  (void) path;
#else
  gtk_tree_selection_unselect_path (selection, path);
#endif
}

void
gimp_widgets_compat_tree_selection_select_iter (gpointer     selection,
                                                GtkTreeIter *iter)
{
#if GTK_CHECK_VERSION(4, 0, 0)
  (void) selection;
  (void) iter;
#else
  gtk_tree_selection_select_iter (selection, iter);
#endif
}

void
gimp_widgets_compat_tree_view_columns_autosize (GtkWidget *tree_view)
{
#if GTK_CHECK_VERSION(4, 0, 0)
  (void) tree_view;
#else
  gtk_tree_view_columns_autosize (GTK_TREE_VIEW (tree_view));
#endif
}

void
gimp_widgets_compat_tree_view_expand_to_path (GtkWidget   *tree_view,
                                              GtkTreePath *path)
{
#if GTK_CHECK_VERSION(4, 0, 0)
  (void) tree_view;
  (void) path;
#else
  gtk_tree_view_expand_to_path (GTK_TREE_VIEW (tree_view), path);
#endif
}

void
gimp_widgets_compat_tree_view_scroll_to_cell (GtkWidget   *tree_view,
                                              GtkTreePath *path,
                                              gboolean     use_align,
                                              gfloat       row_align,
                                              gfloat       col_align)
{
#if GTK_CHECK_VERSION(4, 0, 0)
  (void) tree_view;
  (void) path;
  (void) use_align;
  (void) row_align;
  (void) col_align;
#else
  gtk_tree_view_scroll_to_cell (GTK_TREE_VIEW (tree_view),
                                path, NULL,
                                use_align, row_align, col_align);
#endif
}
