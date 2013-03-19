#include "ic_client_context.h"
#include "ic_main_panel.h"
#include "ic_login_window.h"
#include "ic_loading_panel.h"
#include "ic_chat_manager.h"
#include "ic_xml_parser.h"
#include "ic_data_type.h"
#include "ic_tray_icon.h"

#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

static GtkWidget *login_window;
static GtkWidget *loading_window;
static GtkWidget *main_window;
GtkStatusIcon *tray_icon;

static char ic_username[20];
static char ic_password[20];
static UserInfo *user_info = NULL;
static gboolean is_user_login = FALSE;
static gint loading_time = 0;

static gboolean ic_client_timer();
static void ic_client_verify_user(const gchar *name, const gchar *password);

void ic_client_init() {
	login_window = ic_login_window_new();
	ic_login_window_show(login_window);	
	tray_icon = ic_tray_icon_new ();
}

void ic_user_login(const gchar *name, const gchar *password) {
	strcpy(ic_username, name);
	strcpy(ic_password, password);
	ic_login_window_hide(login_window);
	loading_window = ic_loading_panel_new_with_label(name);
	ic_loading_panel_show(loading_window);
	g_timeout_add(1000, (GSourceFunc)ic_client_timer, NULL);
}

static void ic_client_verify_user(const gchar *name, const gchar *password)
{
	IcData *data = NULL;

	if(strcmp(name, "nixingfeng")==0 && strcmp(password, "1234")==0)
	{
		data = ic_xml_parser_file(RESDIR"user_info.xml");
	}

	if(data == NULL)
	{
		gdk_threads_enter();
		ic_loading_panel_destroy(loading_window);
		ic_login_window_show(login_window);
        ic_login_window_dialog_show(login_window, ERROR_TYPE_INVALIDATE);
		gdk_threads_leave();
	}

	if(data->type == DATA_TYPE_USERINFO)
	{
		is_user_login = TRUE;
		
		user_info = (UserInfo *)(data->data);
		gdk_threads_enter();
		ic_login_window_destroy(login_window);
		ic_loading_panel_destroy(loading_window);
		main_window = ic_main_panel_new(user_info);
		ic_main_panel_show(main_window);
		gdk_threads_leave();
	}
	
	free(data);
}

static gboolean ic_client_timer(){
	loading_time++;
	if(loading_time > 2){
		ic_client_verify_user(ic_username, ic_password);
		return FALSE;
	}else{
		if(loading_time>5){
			loading_time=0;
			gdk_threads_enter();
			ic_loading_panel_destroy(loading_window);
			ic_login_window_show(login_window);
            ic_login_window_dialog_show(login_window, ERROR_TYPE_CONN_TIMEOUT);
			gdk_threads_leave();
			return FALSE;
		}
	}
	return TRUE;
}

GtkWidget *ic_client_get_main_panel()
{
	return main_window;
}

UserInfo *ic_get_user_info() 
{
	return user_info;
}

FriendInfo *ic_get_friendinfo_by_name(gchar *user_name)
{
	GList *friend_list = user_info->friend_list;
	GList *iter = NULL;

	for(iter = friend_list; iter; iter=iter->next)
	{
		FriendInfo *friend_info = (FriendInfo *)iter->data;
		if(strcmp(user_name, friend_info->friend_name) == 0)
		{
			return friend_info;
		}
	}
	return NULL;
}

GtkStatusIcon *ic_client_get_tray_icon()
{
	return tray_icon;
}
