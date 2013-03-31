#ifndef IC_CLIENT_CONTEXT_H
#define IC_CLIENT_CONTEXT_H

#include <gtk/gtk.h>
#include "ic_data_type.h"

void ic_user_login(const gchar*, const gchar*, const gint);
void ic_client_init();
void ic_client_handle_mesg(char *mesg);
void ic_main_panel_init(UserInfo *user);
GtkWidget *ic_client_get_main_panel();
GtkStatusIcon *ic_client_get_tray_icon();

#endif 
