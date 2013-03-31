#ifndef IC_MAIN_PANEL_H
#define IC_MAIN_PANEL_H

#include <gtk/gtk.h>
#include "ic_data_type.h"
#include "type.h"

GtkWidget *ic_main_panel_new(LwqqClient *lc);
void ic_main_panel_show(GtkWidget *widget);
void ic_main_panel_destroy(GtkWidget *widget);

#endif
