#include "ic_data_type.h"
#include "ic_chat_manager.h"
#include "ic_chat_window.h"
#include "ic_client_context.h"
#include "ic_tray_icon.h"
#include <stdlib.h>
#include <glib.h>
#include <string.h>

struct ChatEntity {
	char user_name[20];
	GtkWidget *chat_window;
};

GHashTable *chat_entity_table = NULL;
GList *unread_mesg_list = NULL;

void ic_chat_entity_realize(gchar *user_name)
{
	if(chat_entity_table == NULL)
	{
		chat_entity_table = g_hash_table_new (g_str_hash, g_direct_equal);
	}
	
	FriendInfo *friend_info = ic_get_friendinfo_by_name(user_name);
	
	if(!g_hash_table_contains (chat_entity_table, friend_info->friend_name))
	{
		struct ChatEntity *chat_entity = (struct ChatEntity *)
			malloc(sizeof(struct ChatEntity));
		
		GtkWidget *chat_window = ic_chat_window_new();
		ic_chat_window_set_friend_info(chat_window, friend_info);
		ic_chat_window_show(chat_window);
		
		strcpy(chat_entity->user_name, friend_info->friend_name);
		chat_entity->chat_window = chat_window;
		g_hash_table_insert(chat_entity_table, friend_info->friend_name, 
		                    (gpointer)chat_entity);
	}

}

void ic_chat_entity_destroy(gchar *user_name)
{
	g_hash_table_remove(chat_entity_table, user_name);
}

void ic_chat_manager_show_all()
{
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
}