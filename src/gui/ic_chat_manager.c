#include "ic_data_type.h"
#include "ic_chat_manager.h"
#include "ic_chat_window.h"
#include "ic_client_context.h"
#include "ic_tray_icon.h"
#include <stdlib.h>
#include <glib.h>
#include <string.h>

typedef struct _ChatEntity {
	//char user_name[20];
    int qqnumber;
	GtkWidget *chat_window;
}ChatEntity;

GHashTable *chat_entity_table = NULL;
GList *unread_mesg_list = NULL;

void ic_chat_entity_realize(gint qqnumber)
{
	if(chat_entity_table == NULL)
	{
		chat_entity_table = g_hash_table_new (g_int_hash, g_int_equal);
	}

    //LwqqBuddy *friend = ic_get_friend_by_id(qqnumber); 
    g_debug("qqnumber: %d\n", qqnumber);
    if(!g_hash_table_contains(chat_entity_table, GINT_TO_POINTER(qqnumber)))
    {
        ChatEntity *entity = (ChatEntity *)malloc(sizeof(ChatEntity));

        GtkWidget *chat_window = ic_chat_window_new();
        //ic_chat_window_set_friend(chat_window, friend);
        ic_chat_window_show(chat_window);

        //strcpy(entity->usr_name, friend->qqnumber);
        entity->chat_window = chat_window;
        g_hash_table_insert(chat_entity_table, GINT_TO_POINTER(qqnumber), (gpointer)entity);
    }
}

void ic_chat_entity_destroy(gchar *qqnumber)
{
	g_hash_table_remove(chat_entity_table, qqnumber);
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
