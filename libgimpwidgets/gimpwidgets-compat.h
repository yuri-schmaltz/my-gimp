/* LIBGIMP - The GIMP Library
 * Copyright (C) 1995-1997 Peter Mattis and Spencer Kimball
 *
 * gimpwidgets-compat.h
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

#pragma once

#include <gtk/gtk.h>


void gimp_widgets_compat_box_pack_start (GtkBox    *box,
                                         GtkWidget *child,
                                         gboolean   expand,
                                         gboolean   fill,
                                         guint      padding);
void gimp_widgets_compat_box_pack_end   (GtkBox    *box,
                                         GtkWidget *child,
                                         gboolean   expand,
                                         gboolean   fill,
                                         guint      padding);
void gimp_widgets_compat_container_add  (GtkWidget *container,
                                         GtkWidget *child);
void gimp_widgets_compat_container_remove (GtkWidget *container,
                                           GtkWidget *child);
GtkWidget *gimp_widgets_compat_bin_get_child (GtkWidget *bin);
void gimp_widgets_compat_paned_pack1    (GtkPaned   *paned,
                                         GtkWidget  *child,
                                         gboolean    resize,
                                         gboolean    shrink);
void gimp_widgets_compat_paned_pack2    (GtkPaned   *paned,
                                         GtkWidget  *child,
                                         gboolean    resize,
                                         gboolean    shrink);
void gimp_widgets_compat_widget_set_border_width
                                        (GtkWidget  *widget,
                                         guint       border_width);
GtkWidget    *gimp_widgets_compat_tree_view_new_with_model (GtkTreeModel     *model);
void          gimp_widgets_compat_tree_view_set_headers_visible
                                                        (GtkWidget         *tree_view,
                                                         gboolean           visible);
void          gimp_widgets_compat_tree_view_set_tooltip_column
                                                        (GtkWidget         *tree_view,
                                                         gint               column);
GtkTreeModel *gimp_widgets_compat_tree_view_get_model (GtkWidget          *tree_view);
gpointer      gimp_widgets_compat_tree_view_get_selection
                                                        (GtkWidget         *tree_view);
void          gimp_widgets_compat_tree_view_append_column
                                                        (GtkWidget         *tree_view,
                                                         GtkTreeViewColumn *column);
void          gimp_widgets_compat_tree_view_insert_column
                                                        (GtkWidget         *tree_view,
                                                         GtkTreeViewColumn *column,
                                                         gint               position);
void          gimp_widgets_compat_tree_view_set_headers_clickable
                                                        (GtkWidget         *tree_view,
                                                         gboolean           setting);
void          gimp_widgets_compat_tree_selection_set_mode
                                                        (gpointer           selection,
                                                         GtkSelectionMode   mode);
gboolean      gimp_widgets_compat_tree_selection_get_selected
                                                        (gpointer           selection,
                                                         GtkTreeModel      **model,
                                                         GtkTreeIter       *iter);
void          gimp_widgets_compat_tree_selection_select_path
                                                        (gpointer           selection,
                                                         GtkTreePath       *path);
void          gimp_widgets_compat_tree_selection_unselect_path
                                                        (gpointer           selection,
                                                         GtkTreePath       *path);
void          gimp_widgets_compat_tree_selection_select_iter
                                                        (gpointer           selection,
                                                         GtkTreeIter       *iter);
void          gimp_widgets_compat_tree_view_columns_autosize
                                                        (GtkWidget         *tree_view);
void          gimp_widgets_compat_tree_view_expand_to_path
                                                        (GtkWidget         *tree_view,
                                                         GtkTreePath       *path);
void          gimp_widgets_compat_tree_view_scroll_to_cell
                                                        (GtkWidget         *tree_view,
                                                         GtkTreePath       *path,
                                                         gboolean           use_align,
                                                         gfloat             row_align,
                                                         gfloat             col_align);
