#ifndef IC_CLIENT_CONTEXT_H
#define IC_CLIENT_CONTEXT_H

#include <gtk/gtk.h>
#include "ic_data_type.h"

void ic_user_login(const gchar*, const gchar*);
void ic_client_init();
void ic_client_handle_mesg(char *mesg);
void ic_main_panel_init(UserInfo *user);
GtkWidget *ic_client_get_main_panel();
UserInfo *ic_get_user_info();
FriendInfo *ic_get_friendinfo_by_name(gchar *user_name);
GtkStatusIcon *ic_client_get_tray_icon();

#endif 
