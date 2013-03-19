#ifndef IC_LOADING_PANEL_H
#define IC_LOADING_PANEL_H

#include <gtk/gtk.h>

GtkWidget *ic_loading_panel_new();
GtkWidget *ic_loading_panel_new_with_label(const gchar *str);
void ic_loading_panel_show(GtkWidget *widget);
void ic_loading_panel_destroy(GtkWidget *widget);

#endif
