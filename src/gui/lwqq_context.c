#include "lwqq_context.h"
#include "type.h"
#include "login.h"
#include "logger.h"
#include "info.h"
#include "smemory.h"
#include "msg.h"

#include <pthread.h>
#include <glib.h>

//LwqqClient *lwqq_client = NULL;

static gpointer receive_msg(gpointer data);

void lwqq_context_handle_login(const char *username, const char *password, const int status)
{
    LwqqClient *lc = lwqq_client_new(username, password);
    LwqqErrorCode err = LWQQ_EC_ERROR;

    lwqq_login(lc, &err);

    switch (err) {
    case LWQQ_EC_OK:
        lwqq_log(LOG_NOTICE, "Login successfully\n");
        lwqq_info_get_friends_info(lc, NULL);

        pthread_t tid;
        pthread_attr_t attr;

        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
        pthread_create(&tid, &attr, receive_msg, lc->msg_list);
        pthread_attr_destroy(&attr);

        /* update main panel */
        /*gqq_mainloop_attach(&gtkloop, qq_mainpanel_update, 1,
                            QQ_MAINPANEL(QQ_MAINWINDOW(panel->container)->main_panel));*/

        /* show main panel */
        /*gqq_mainloop_attach(&gtkloop, qq_mainwindow_show_mainpanel,
                            1, panel->container);*/
        
        //update_details(lc, panel);
        break;
    case LWQQ_EC_LOGIN_NEED_VC:
        ic_client_context_get_verifycode(lc);
        break;
    case LWQQ_EC_ERROR:
    default:
        lwqq_log(LOG_ERROR, "Login failed\n");
#if 0
        gqq_mainloop_attach(&gtkloop, gtk_label_set_text, 2,
                            GTK_LABEL(panel->err_label), "Login failed");
        gqq_mainloop_attach(&gtkloop, qq_mainwindow_show_loginpanel,
                            1, panel->container);
#endif
        break;
    }
}

static gpointer receive_msg(gpointer data)
{
    LwqqRecvMsgList *l = (LwqqRecvMsgList *)data;
    l->poll_msg(l);
    while (1) {
        LwqqRecvMsg *msg;
        pthread_mutex_lock(&l->mutex);
        if (SIMPLEQ_EMPTY(&l->head)) {
            /* No message now, wait 100ms */
            pthread_mutex_unlock(&l->mutex);
            usleep(100000);
            continue;
        }
        msg = SIMPLEQ_FIRST(&l->head);
        SIMPLEQ_REMOVE_HEAD(&l->head, entries);
        pthread_mutex_unlock(&l->mutex);
        //gqq_mainloop_attach(&gtkloop, handle_new_msg, 1, msg);
    }
    return NULL;
}

#if 0
static void handle_new_msg(LwqqRecvMsg *recvmsg)
{
    LwqqMsg *msg = recvmsg->msg;

    printf("Receive message type: %d\n", msg->type);

    if (msg->type == LWQQ_MT_BUDDY_MSG) {
        LwqqMsgMessage *mmsg = msg->opaque;
        GtkWidget *cw = g_hash_table_lookup(lwqq_chat_window, mmsg->from);
//        printf("Receive message: %s\n", mmsg->content);
        if (!cw) {
            cw = qq_chatwindow_new(mmsg->from);
            lwqq_log(LOG_DEBUG, "No chat window for uin:%s, create a new:%p\n",
                     mmsg->from, cw);
#if 0
            // not show it
            gtk_widget_hide(cw);
#endif
            g_hash_table_insert(lwqq_chat_window, g_strdup(mmsg->from), cw);
        } else {
            lwqq_log(LOG_DEBUG, "Found chat window:%p for uin:%s\n", cw, mmsg->from);
        }
        qq_chatwindow_add_recv_message(cw, mmsg);
        gtk_widget_show(cw);

    } else if (msg->type == LWQQ_MT_GROUP_MSG) {
        
    } else if (msg->type == LWQQ_MT_STATUS_CHANGE) {
        
    } else {
        printf("unknow message\n");
    }
    
    lwqq_msg_free(recvmsg->msg);
    s_free(recvmsg);
}
#endif



#if 0
static void update_details(LwqqClient *lc, QQLoginPanel *panel)
{
#if 0
    // update my information
    qq_get_buddy_info(info, info -> me, NULL);
    gqq_mainloop_attach(&gtkloop
                        , qq_mainpanel_update_my_info
                        , 1
                        , QQ_MAINWINDOW(panel -> container) -> main_panel);
#endif

    QQMainPanel *mp = QQ_MAINPANEL(QQ_MAINWINDOW(panel->container)->main_panel);
    /* update online buddies */
    lwqq_info_get_online_buddies(lc, NULL);
    gqq_mainloop_attach(&gtkloop, qq_mainpanel_update_online_buddies, 1, mp);

    //update qq number
    update_buddy_qq_number(lc, (QQMainPanel *)QQ_MAINWINDOW(panel->container)->main_panel);
#if 0
    // update group number
    gint i;
    QQGroup *grp;
    gchar num[100];
    for(i = 0; i < info->groups->len; ++i){
        grp = g_ptr_array_index(info -> groups, i);
        if(grp == NULL){
            continue;
        }
        qq_get_qq_number(info, grp -> code -> str, num, NULL);
        qq_group_set(grp, "gnumber", num);
    }
    gqq_mainloop_attach(&gtkloop, qq_mainpanel_update_group_info , 1,
                        QQ_MAINWINDOW(panel -> container) -> main_panel);

    //update face image
    update_face_image(info,
                      (QQMainPanel*)QQ_MAINWINDOW(panel->container)-> main_panel);
#endif
}
#endif
