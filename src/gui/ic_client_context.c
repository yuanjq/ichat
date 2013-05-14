#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "ic_client_context.h"
#include "ic_main_panel.h"
#include "ic_login_window.h"
#include "ic_loading_panel.h"
#include "ic_chat_manager.h"
#include "ic_data_type.h"
#include "ic_tray_icon.h"

#include "type.h"
#include "login.h"
#include "logger.h"
#include "info.h"
#include "smemory.h"
#include "msg.h"

static GtkWidget *login_window;
static GtkWidget *loading_window;
static GtkWidget *main_window;
GtkStatusIcon *tray_icon;

LwqqClient *lwqq_client = NULL;
static gboolean is_user_login = FALSE;
static gboolean is_login_need_vc = FALSE;
static gint loading_time = 0;

static gboolean ic_client_timer();
static void ic_client_verify_user(const gchar *name, const gchar *password);
static gpointer ic_client_receive_msg(gpointer data);
static gpointer ic_thread_lwqq_login(gpointer data);

void ic_client_init() {
	login_window = ic_login_window_new();
	ic_login_window_show(login_window);	
	tray_icon = ic_tray_icon_new ();
}

void ic_user_login(const gchar *username, const gchar *password, const gint status) {
	ic_login_window_hide(login_window);
	loading_window = ic_loading_panel_new_with_label(username);
	ic_loading_panel_show(loading_window);
	g_timeout_add(1000, (GSourceFunc)ic_client_timer, NULL);

    lwqq_client = lwqq_client_new(username, password);
    
    pthread_t tid;
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_create(&tid, &attr, ic_thread_lwqq_login, NULL);
    pthread_attr_destroy(&attr);
}

static gpointer ic_thread_lwqq_login(gpointer data)
{
    LwqqErrorCode err = LWQQ_EC_ERROR;
    lwqq_login(lwqq_client, &err);
    
    switch (err) {
    case LWQQ_EC_OK:
        lwqq_log(LOG_NOTICE, "Login successfully\n");
        lwqq_info_get_friends_info(lwqq_client, NULL);
        lwqq_info_get_friend_detail_info(lwqq_client, lwqq_client->myself, NULL);
        lwqq_info_get_all_friend_qqnumbers(lwqq_client, NULL);
        lwqq_info_get_long_nick(lwqq_client, lwqq_client->myself);
        lwqq_info_get_avatar(lwqq_client, lwqq_client->myself, NULL);

        is_user_login = TRUE;

        gdk_threads_enter();
		ic_login_window_destroy(login_window);
		ic_loading_panel_destroy(loading_window);
		main_window = ic_main_panel_new(lwqq_client);
		ic_main_panel_show(main_window);
        gdk_threads_leave();

        pthread_t tid;
        pthread_attr_t attr;

        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
        pthread_create(&tid, &attr, ic_client_receive_msg, lwqq_client->msg_list);
        pthread_attr_destroy(&attr);

        //update_details(lc, panel);
        break;
    case LWQQ_EC_LOGIN_NEED_VC:
        is_login_need_vc = TRUE;
        loading_time = 0;
        gdk_threads_enter();
		ic_loading_panel_destroy(loading_window);
        ic_login_window_show(login_window);
        ic_login_window_get_verifycode(login_window, lwqq_client);
        gdk_threads_leave();
        break;
    case LWQQ_EC_ERROR:
    default:

        gdk_threads_enter();
        ic_loading_panel_destroy(loading_window);
        ic_login_window_show(login_window);
        ic_login_window_dialog_show(login_window, ERROR_TYPE_INVALIDATE);
        gdk_threads_leave();

        lwqq_log(LOG_ERROR, "Login failed\n");
        break;
    }
}

static gpointer ic_client_receive_msg(gpointer data)
{
    LwqqRecvMsgList *l = (LwqqRecvMsgList *)data;
    l->poll_msg(l);
    while (1) {
        LwqqRecvMsg *msg;
        pthread_mutex_lock(&l->mutex);
        if (SIMPLEQ_EMPTY(&l->head)) {
            /* No message now, wait 100ms */
            pthread_mutex_unlock(&l->mutex);
            sleep(1); 
            continue;
        }
        msg = SIMPLEQ_FIRST(&l->head);
        SIMPLEQ_REMOVE_HEAD(&l->head, entries);
        pthread_mutex_unlock(&l->mutex);
        //gqq_mainloop_attach(&gtkloop, handle_new_msg, 1, msg);
    }
    return NULL;
}

static gboolean ic_client_timer(){
	loading_time++;
	if(is_user_login || is_login_need_vc){
		return FALSE;
	}else{
		if(loading_time>9){
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

LwqqBuddy *ic_get_friend_by_id(gchar *qqnumber)
{
    LwqqBuddy *friend = NULL;
    LIST_FOREACH(friend, &lwqq_client->friends, entries)
    {
        if(strcmp(friend->qqnumber, qqnumber) == 0)
        {
            return friend;
        }
    }

    return NULL;
}

GtkStatusIcon *ic_client_get_tray_icon()
{
	return tray_icon;
}

