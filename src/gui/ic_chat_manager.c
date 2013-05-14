#include "ic_data_type.h"
#include "ic_chat_manager.h"
#include "ic_chat_window.h"
#include "ic_client_context.h"
#include "ic_tray_icon.h"
#include <stdlib.h>
#include <glib.h>
#include <string.h>

typedef struct _ChatEntity {
    //gchar *qqnumber;
	GtkWidget *chat_window;
}ChatEntity;

GHashTable *chat_entity_table = NULL;
GList *unread_mesg_list = NULL;

void ic_chat_entity_realize(gchar *qqnumber)
{
	if(chat_entity_table == NULL)
	{
		chat_entity_table = g_hash_table_new (g_str_hash, g_str_equal);
	}

    LwqqBuddy *friend = ic_get_friend_by_id(qqnumber); 
    if(!g_hash_table_contains(chat_entity_table, qqnumber))
    {
        ChatEntity *entity = (ChatEntity *)g_malloc(sizeof(ChatEntity));
        GtkWidget *chat_window = ic_chat_window_new_with_info(friend);
        gchar *key = g_strdup(qqnumber);
        entity->chat_window = chat_window;
        g_hash_table_insert(chat_entity_table, (gpointer)key, (gpointer)entity);
        //ic_chat_window_show(chat_window);
    }
}

void ic_chat_entity_destroy(gchar *qqnumber)
{
    ChatEntity *entity = (ChatEntity *)g_hash_table_lookup(chat_entity_table, (gconstpointer)qqnumber);
    GtkWidget *widget = entity->chat_window;
    gtk_widget_destroy(widget);
    g_free(entity);
	g_hash_table_remove(chat_entity_table, (gconstpointer)qqnumber);
}

void ic_chat_manager_show_all()
{
#if 0
	GList *iter = NULL;
	for(iter = unread_mesg_list; iter; iter = iter->next)
	{
		Message *mesg = (Message *)(iter->data);
		FriendInfo *friend_info = ic_get_friendinfo_by_name(mesg->sender);
		if(!g_hash_table_contains(chat_entity_table, friend_info->friend_name))
		{
			ic_chat_entity_realize(mesg->sender);
		}
		struct ChatEntity *entity = g_hash_table_lookup (chat_entity_table,
	                                                 friend_info->friend_name);
		GtkWidget *chat_window = entity->chat_window;
		ic_chatwindow_append_recvmesg (chat_window, mesg);	
		free(mesg);
	}
	g_list_free(unread_mesg_list);
#endif
}
