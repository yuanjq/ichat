#ifndef IC_LOGIN_WINDOW_H
#define IC_LOGIN_WINDOW_H

#include <gtk/gtk.h>
#include "type.h"

GtkWidget *ic_login_window_new();
void ic_login_window_show(GtkWidget *);
void ic_login_window_destroy(GtkWidget *);
void ic_login_window_hide(GtkWidget *);
void ic_login_window_dialog_show(GtkWidget *window, gint error_type);
void ic_login_window_get_verifycode(GtkWidget *widget, LwqqClient *lwqq_client);

#endif
