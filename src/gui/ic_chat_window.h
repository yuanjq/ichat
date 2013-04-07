#ifndef IC_CHAT_WINDOW_H
#define IC_CHAT_WINDOW_H

#include "ic_data_type.h"
#include <gtk/gtk.h>
#include "type.h"

#define IC_TYPE_CHAT_WINDOW			(ic_chat_window_get_type())
#define IC_CHAT_WINDOW(obj)			(G_TYPE_CHECK_INSTANCE_CAST(obj, IC_TYPE_CHAT_WINDOW, IcChatWindow))
#define IC_CHAT_WINDOW_CLASS(class)	(G_TYPE_CHECK_CLASS_CAST(class, IC_TYPE_CHAT_WINDOW, IcChatWindowClass))
#define IC_IS_CHAT_WINDOW(obj)		(G_TYPE_CHECK_INSTANCE_TYPE(obj, IC_TYPE_CHAT_WINDOW))

typedef struct _IcChatWindow 		IcChatWindow;
typedef struct _IcChatWindowClass 	IcChatWindowClass;
typedef struct _IcChatWindowPrivate IcChatWindowPrivate;

struct _IcChatWindow
{
	GtkWindow chat_window;

	IcChatWindowPrivate *private;
};

struct _IcChatWindowClass
{
	GtkWindowClass parent_class;
};

GType ic_chat_window_get_type();
GtkWidget *ic_chat_window_new();
void ic_chat_window_set_friend(GtkWidget *widget, LwqqBuddy *buddy);
void ic_chat_window_show(GtkWidget *widget);

#endif
