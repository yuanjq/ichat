#include "ic_titlebar.h"
#include "ic_button.h"

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
	GtkWidgetClass *object_class;
	object_class = (GtkWidgetClass*)titlebar_class;
	ic_titlebar_signals[IC_TITLEBAR_DESTROY_SIGNAL]
		= g_signal_new("ic-titlebar-destroy",
					G_TYPE_FROM_CLASS(object_class),
					G_SIGNAL_RUN_FIRST,
                    0,
					NULL, NULL,
					g_cclosure_marshal_VOID__VOID,
					G_TYPE_NONE, 0);
}

static void ic_titlebar_init(IcTitlebar *titlebar)
{
	GdkPixbuf *pixbuf;

	titlebar->label = gtk_label_new (NULL);

	titlebar->min_btn = ic_button_new();
	gtk_widget_set_size_request(titlebar->min_btn, 27, 21);
	pixbuf = gdk_pixbuf_new_from_file_at_scale(RESDIR"images/min1.png", 27, 21, FALSE, NULL);
	ic_button_set_image(titlebar->min_btn, pixbuf);
	pixbuf = gdk_pixbuf_new_from_file_at_scale(RESDIR"images/min2.png", 27, 21, FALSE, NULL);
	ic_button_set_hovered_image(titlebar->min_btn, pixbuf);
	pixbuf = gdk_pixbuf_new_from_file_at_scale(RESDIR"images/min3.png", 27, 21, FALSE, NULL);
	ic_button_set_pressed_image(titlebar->min_btn, pixbuf);

	titlebar->max_btn = ic_button_new();
	gtk_widget_set_size_request(titlebar->max_btn, 27, 21);
	pixbuf = gdk_pixbuf_new_from_file_at_scale(RESDIR"images/max1.png", 27, 21, FALSE, NULL);
	ic_button_set_image(titlebar->max_btn, pixbuf);
	pixbuf = gdk_pixbuf_new_from_file_at_scale(RESDIR"images/max2.png", 27, 21, FALSE, NULL);
	ic_button_set_hovered_image(titlebar->max_btn , pixbuf);
	pixbuf = gdk_pixbuf_new_from_file_at_scale(RESDIR"images/max3.png", 27, 21, FALSE, NULL);
	ic_button_set_pressed_image(titlebar->max_btn , pixbuf);

	titlebar->close_btn = ic_button_new();
	gtk_widget_set_size_request(titlebar->close_btn, 38, 21);
	pixbuf = gdk_pixbuf_new_from_file_at_scale(RESDIR"images/close1.png", 38, 21, FALSE, NULL);
	ic_button_set_image(titlebar->close_btn, pixbuf);
	pixbuf = gdk_pixbuf_new_from_file_at_scale(RESDIR"images/close2.png", 38, 21, FALSE, NULL);
	ic_button_set_hovered_image(titlebar->close_btn, pixbuf);
	pixbuf = gdk_pixbuf_new_from_file_at_scale(RESDIR"images/close3.png", 38, 21, FALSE, NULL);
	ic_button_set_pressed_image(titlebar->close_btn, pixbuf);

	g_signal_connect_swapped(G_OBJECT(titlebar->close_btn), "button-release-event", 
	                 G_CALLBACK(on_close_button_clicked), titlebar);
	
	gtk_box_pack_start(GTK_BOX(titlebar), titlebar->label, FALSE, FALSE, 0);
	gtk_box_pack_end(GTK_BOX(titlebar), titlebar->close_btn, FALSE, FALSE, 0);
	gtk_box_pack_end(GTK_BOX(titlebar), titlebar->max_btn, FALSE, FALSE, 0);
	gtk_box_pack_end(GTK_BOX(titlebar), titlebar->min_btn, FALSE, FALSE, 0);
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
    IcTitlebar *titlebar = IC_TITLEBAR(widget);
	gtk_label_set_text (GTK_LABEL(titlebar->label), label);
}

static void on_close_button_clicked(GtkWidget *widget)
{
	g_signal_emit((gpointer)widget, ic_titlebar_signals[IC_TITLEBAR_DESTROY_SIGNAL], 
	              0);
}

