#ifndef IC_CHATWINDOW_TITLEBAR_H
#define IC_CHATWINDOW_TITLEBAR_H

#include <gtk/gtk.h>

#define IC_TYPE_CHATWINDOW_TITLEBAR (ic_chatwindow_titlebar_get_type())
#define IC_CHATWINDOW_TITLEBAR(obj) (G_TYPE_CHECK_INSTANCE_CAST(obj, IC_TYPE_CHATWINDOW_TITLEBAR, IcChatwindowTitlebar))

typedef struct _IcChatwindowTitlebar IcChatwindowTitlebar;
typedef struct _IcChatwindowTitlebarClass IcChatwindowTitlebarClass;

struct _IcChatwindowTitlebar {
	GtkVBox box;
	GtkWidget *nick_name;
	GtkWidget *signature;
	GtkWidget *menu_button;
	GtkWidget *min_button;
	GtkWidget *max_button;
	GtkWidget *close_button;
};

struct _IcChatwindowTitlebarClass {
	GtkVBoxClass parent_class;
};

GType ic_chatwindow_titlebar_get_type(void);
GtkWidget *ic_chatwindow_titlebar_new(void);
void ic_chatwindow_titlebar_set_label(GtkWidget *widget, gchar *label);
void ic_chatwindow_titlebar_set_signatrue(GtkWidget *widget, gchar *label);
GtkWidget *ic_chatwindow_titlebar_new_with_label(gchar *label);

#endif
