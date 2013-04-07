#ifndef IC_CLIENT_CONTEXT_H
#define IC_CLIENT_CONTEXT_H

#include <gtk/gtk.h>
#include "type.h"

void ic_user_login(const gchar*, const gchar*, const gint);
void ic_client_init();
void ic_client_handle_mesg(char *mesg);
GtkWidget *ic_client_get_main_panel();
GtkStatusIcon *ic_client_get_tray_icon();
LwqqBuddy *ic_get_friend_by_id(char *qqnumber);

#endif 
