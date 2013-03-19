#include "ic_titlebar.h"
#include "ic_button.h"

struct _IcTitlebarPriv
{
	GtkWidget *label;
	GtkWidget *min_btn;
	GtkWidget *max_btn;
	GtkWidget *close_btn;
};

enum {
	IC_TITLEBAR_DESTROY_SIGNAL,
	IC_TITLEBAR_SIGNAL_N
};

static gint ic_titlebar_signals[IC_TITLEBAR_SIGNAL_N] = {0};
static void ic_titlebar_class_init(IcTitlebarClass *titlebar_class);
static void ic_titlebar_init(IcTitlebar *titlebar);
static void on_close_button_clicked(GtkWidget *widget);

GType ic_titlebar_get_type ()
{
	static GType ic_titlebar_type = 0;
	if(!ic_titlebar_type)
	{
		static const GTypeInfo ic_titlebar_info = {
			sizeof(IcTitlebarClass),
			NULL,
			NULL,
			(GClassInitFunc)ic_titlebar_class_init,
			NULL,
			NULL,
			sizeof(IcTitlebar),
			0,
			(GInstanceInitFunc)ic_titlebar_init,
			0			
		};
		ic_titlebar_type = g_type_register_static(GTK_TYPE_HBOX, "IcTitlebar", &ic_titlebar_info, 0);
	}

	return ic_titlebar_type;
}

static void ic_titlebar_class_init(IcTitlebarClass *titlebar_class)
{
	g_type_class_add_private(titlebar_class, sizeof(IcTitlebarPriv));
	GtkWidgetClass *object_class;
	object_class = (GtkWidgetClass*)titlebar_class;
	ic_titlebar_signals[IC_TITLEBAR_DESTROY_SIGNAL]
		= g_signal_new("ic-titlebar-destroy",
					G_TYPE_FROM_CLASS(object_class),
					G_SIGNAL_RUN_FIRST,
					G_STRUCT_OFFSET(IcTitlebarClass, ic_titlebar_destroy),
					NULL, NULL,
					g_cclosure_marshal_VOID__VOID,
					G_TYPE_NONE, 0);
}

static void ic_titlebar_init(IcTitlebar *titlebar)
{
	GdkPixbuf *pixbuf;

	IcTitlebarPriv *priv = G_TYPE_INSTANCE_GET_PRIVATE(titlebar, IC_TYPE_TITLEBAR,
	                                                  IcTitlebarPriv);
	priv->label = gtk_label_new (NULL);

	priv->min_btn = ic_button_new();
	gtk_widget_set_size_request(priv->min_btn, 27, 21);
	pixbuf = gdk_pixbuf_new_from_file_at_scale(RESDIR"images/min1.png", 27, 21, FALSE, NULL);
	ic_button_set_image(priv->min_btn, pixbuf);
	pixbuf = gdk_pixbuf_new_from_file_at_scale(RESDIR"images/min2.png", 27, 21, FALSE, NULL);
	ic_button_set_hovered_image(priv->min_btn, pixbuf);
	pixbuf = gdk_pixbuf_new_from_file_at_scale(RESDIR"images/min3.png", 27, 21, FALSE, NULL);
	ic_button_set_pressed_image(priv->min_btn, pixbuf);

	priv->max_btn = ic_button_new();
	gtk_widget_set_size_request(priv->max_btn, 27, 21);
	pixbuf = gdk_pixbuf_new_from_file_at_scale(RESDIR"images/max1.png", 27, 21, FALSE, NULL);
	ic_button_set_image(priv->max_btn, pixbuf);
	pixbuf = gdk_pixbuf_new_from_file_at_scale(RESDIR"images/max2.png", 27, 21, FALSE, NULL);
	ic_button_set_hovered_image(priv->max_btn , pixbuf);
	pixbuf = gdk_pixbuf_new_from_file_at_scale(RESDIR"images/max3.png", 27, 21, FALSE, NULL);
	ic_button_set_pressed_image(priv->max_btn , pixbuf);

	priv->close_btn = ic_button_new();
	gtk_widget_set_size_request(priv->close_btn, 38, 21);
	pixbuf = gdk_pixbuf_new_from_file_at_scale(RESDIR"images/close1.png", 38, 21, FALSE, NULL);
	ic_button_set_image(priv->close_btn, pixbuf);
	pixbuf = gdk_pixbuf_new_from_file_at_scale(RESDIR"images/close2.png", 38, 21, FALSE, NULL);
	ic_button_set_hovered_image(priv->close_btn, pixbuf);
	pixbuf = gdk_pixbuf_new_from_file_at_scale(RESDIR"images/close3.png", 38, 21, FALSE, NULL);
	ic_button_set_pressed_image(priv->close_btn, pixbuf);

	g_signal_connect_swapped(G_OBJECT(priv->close_btn), "button-release-event", 
	                 G_CALLBACK(on_close_button_clicked), titlebar);
	
	gtk_box_pack_start(GTK_BOX(titlebar), priv->label, FALSE, FALSE, 0);
	gtk_box_pack_end(GTK_BOX(titlebar), priv->close_btn, FALSE, FALSE, 0);
	gtk_box_pack_end(GTK_BOX(titlebar), priv->max_btn, FALSE, FALSE, 0);
	gtk_box_pack_end(GTK_BOX(titlebar), priv->min_btn, FALSE, FALSE, 0);
}

GtkWidget *ic_titlebar_new ()
{
	return g_object_new(IC_TYPE_TITLEBAR, NULL);
}

GtkWidget *ic_titlebar_new_with_label(gchar *label)
{
	GtkWidget *widget = ic_titlebar_new ();
	ic_titlebar_set_label (widget, label);
	return widget;
}

void ic_titlebar_set_label(GtkWidget *widget, gchar *label)
{
	IcTitlebarPriv *priv = G_TYPE_INSTANCE_GET_PRIVATE(widget, IC_TYPE_TITLEBAR,
	                                               IcTitlebarPriv);
	gtk_label_set_text (GTK_LABEL(priv->label), label);
}

static void on_close_button_clicked(GtkWidget *widget)
{
	g_signal_emit((gpointer)widget, ic_titlebar_signals[IC_TITLEBAR_DESTROY_SIGNAL], 
	              0);
}

static void ic_titlebar_destroy(GtkWidget *widget, gpointer data)
{
}
