#include "ic_tray_icon.h"
#include "ic_chat_manager.h"
#include "ic_client_context.h"

struct _IcTrayIconPriv
{
	GtkWidget *popup_menu;
	GtkWidget *notify_box;
	gboolean blinking;
	GdkPixbuf *blink_icon;
	GdkPixbuf *blank_icon;
};

static void ic_tray_icon_class_init(IcTrayIconClass *class);
static void ic_tray_icon_init(IcTrayIcon *tray_icon);

static void on_tray_activate(GtkStatusIcon *tray_icon, gpointer data);
static void on_tray_popup_menu(GtkStatusIcon *tray_icon, guint button, 
                               guint activate_time, gpointer data);
static void on_tray_open_main_panel(GtkWidget *widget, gpointer data);

static void ic_tray_icon_start_blinking(IcTrayIcon *tray_icon);
static void ic_tray_icon_stop_blinking(IcTrayIcon *tray_icon);
static gboolean ic_tray_blink_timer(IcTrayIcon *tray_icon);

GType ic_tray_icon_get_type ()
{
	static GType ic_tray_icon_type = 0;
	if(!ic_tray_icon_type)
	{
		static const GTypeInfo ic_tray_icon_info = {
			sizeof(IcTrayIconClass),
			NULL,
			NULL,
			(GClassInitFunc)ic_tray_icon_class_init,
			NULL,
			NULL,
			sizeof(IcTrayIcon),
			0,
			(GInstanceInitFunc)ic_tray_icon_init
		};
		ic_tray_icon_type = g_type_register_static(GTK_TYPE_STATUS_ICON, 
		                                           "IcTrayIcon",
		                                           &ic_tray_icon_info, 0);
	}
	return ic_tray_icon_type;
}

static void ic_tray_icon_class_init(IcTrayIconClass *class)
{
	g_type_class_add_private(class, sizeof(IcTrayIconPriv));
}

static void ic_tray_icon_init(IcTrayIcon *tray_icon)
{
	gtk_status_icon_set_tooltip_markup (GTK_STATUS_ICON(tray_icon), "<b>iChat</b>");
	IcTrayIconPriv *priv = G_TYPE_INSTANCE_GET_PRIVATE(tray_icon, IC_TYPE_TRAY_ICON,
	                                                   IcTrayIconPriv);

	priv->blink_icon = gdk_pixbuf_new_from_file (RESDIR"images/head.png", NULL);
	priv->blank_icon = gdk_pixbuf_new (GDK_COLORSPACE_RGB, TRUE, 8,
	                                   12, 12);
	gdk_pixbuf_fill(priv->blank_icon, 0);
	
	gtk_status_icon_set_from_pixbuf(GTK_STATUS_ICON(tray_icon), priv->blink_icon);
	gtk_status_icon_set_tooltip_text (GTK_STATUS_ICON(tray_icon), "ichat");
	gtk_status_icon_set_visible (GTK_STATUS_ICON(tray_icon), TRUE);
	ic_tray_icon_set_blinking(tray_icon);

	/*g_signal_connect(G_OBJECT(tray_icon), "query-tooltip", 
	                 G_CALLBACK(on_tray_activate), NULL);
	*/
	g_signal_connect(G_OBJECT(tray_icon), "activate", 
	                 G_CALLBACK(on_tray_activate), NULL);
	g_signal_connect(G_OBJECT(tray_icon), "popup-menu", 
	                 G_CALLBACK(on_tray_popup_menu), NULL);

	priv->popup_menu = gtk_menu_new ();
	priv->blinking = FALSE;
	
	GtkWidget *menu_item;
	menu_item = gtk_menu_item_new_with_label("主面板");
	gtk_menu_shell_append (GTK_MENU_SHELL(priv->popup_menu), menu_item);
	g_signal_connect(G_OBJECT(menu_item), "activate", 
	                 G_CALLBACK(on_tray_open_main_panel), NULL);
	menu_item = gtk_menu_item_new_with_label("退出");
	gtk_menu_shell_append (GTK_MENU_SHELL(priv->popup_menu), menu_item);
	g_signal_connect(G_OBJECT(menu_item), "activate",
	                 G_CALLBACK(gtk_main_quit), NULL);
	gtk_widget_show_all (priv->popup_menu);
}

static void on_tray_activate (GtkStatusIcon *tray_icon, gpointer data)
{
	IcTrayIconPriv *priv = G_TYPE_INSTANCE_GET_PRIVATE(tray_icon, IC_TYPE_TRAY_ICON,
	                                                   IcTrayIconPriv);
	if(priv->blinking)
	{
		ic_chat_manager_show_all();
		ic_tray_icon_stop_blinking (IC_TRAY_ICON(tray_icon));
	}else
	{
		GtkWidget *main_panel = ic_client_get_main_panel();
		if(main_panel != NULL)
		{
			gtk_widget_show_all(main_panel);
		}
	}
}

static void on_tray_popup_menu(GtkStatusIcon *tray_icon, guint button, 
                               guint activate_time, gpointer data)
{
	IcTrayIconPriv *priv = G_TYPE_INSTANCE_GET_PRIVATE(tray_icon, IC_TYPE_TRAY_ICON,
	                                                   IcTrayIconPriv);
	gtk_menu_popup (GTK_MENU(priv->popup_menu), NULL, NULL, gtk_status_icon_position_menu,
	                tray_icon, button, activate_time);
}

static void on_tray_open_main_panel(GtkWidget *widget, gpointer data)
{
	GtkWidget *main_panel = ic_client_get_main_panel();
	if(main_panel != NULL)
	{
		gtk_widget_show_all(main_panel);
	}
}

void ic_tray_icon_set_blinking(IcTrayIcon *tray_icon)
{
	IcTrayIconPriv *priv = G_TYPE_INSTANCE_GET_PRIVATE(tray_icon, IC_TYPE_TRAY_ICON,
	                                                   IcTrayIconPriv);
	if(priv->blinking == FALSE)
	{
		ic_tray_icon_start_blinking (tray_icon);
	}
}

static void ic_tray_icon_start_blinking(IcTrayIcon *tray_icon)
{
	IcTrayIconPriv *priv = G_TYPE_INSTANCE_GET_PRIVATE(tray_icon, IC_TYPE_TRAY_ICON,
	                                                   IcTrayIconPriv);
	//priv->blinking = TRUE;
	gdk_threads_add_timeout(300, (GSourceFunc)ic_tray_blink_timer, (gpointer)tray_icon);
}

static gboolean ic_tray_blink_timer(IcTrayIcon *tray_icon)
{
	static gboolean visible = TRUE;
	IcTrayIconPriv *priv = G_TYPE_INSTANCE_GET_PRIVATE(tray_icon, IC_TYPE_TRAY_ICON,
	                                                   IcTrayIconPriv);
	
	visible = visible^TRUE;
	if(visible)
	{
		gtk_status_icon_set_from_pixbuf (GTK_STATUS_ICON(tray_icon),
		                                 priv->blink_icon);
	}
	else
	{
		gtk_status_icon_set_from_pixbuf (GTK_STATUS_ICON(tray_icon),
		                                 priv->blank_icon);
	}
	
	if(priv->blinking == FALSE)
	{
		gtk_status_icon_set_from_pixbuf (GTK_STATUS_ICON(tray_icon),
		                                 priv->blink_icon);
		return FALSE;
	}

	return TRUE;
}	

static void ic_tray_icon_stop_blinking(IcTrayIcon *tray_icon)
{
	IcTrayIconPriv *priv = G_TYPE_INSTANCE_GET_PRIVATE(tray_icon, IC_TYPE_TRAY_ICON,
	                                                   IcTrayIconPriv);
	priv->blinking = FALSE;
}

IcTrayIcon *ic_tray_icon_new ()
{
	return g_object_new (IC_TYPE_TRAY_ICON, NULL);
}
