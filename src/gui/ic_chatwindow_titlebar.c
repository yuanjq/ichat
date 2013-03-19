#include "ic_chatwindow_titlebar.h"
#include "ic_button.h"

enum {
	IC_CHATWINDOW_TITLEBAR_DESTROY_SIGNAL,
	IC_CHATWINDOW_TITLEBAR_SIGNAL_N
};

static gint ic_chatwindow_titlebar_signals[IC_CHATWINDOW_TITLEBAR_SIGNAL_N] = {0};
static void ic_chatwindow_titlebar_init(IcChatwindowTitlebar *ic_title_bar);
static void ic_chatwindow_titlebar_class_init(IcChatwindowTitlebarClass *ic_title_bar_class);
static void ic_on_close_button_pressed(GtkWidget *widget, gpointer data);

// 注册自定义控件
GType ic_chatwindow_titlebar_get_type(void) {
	static GType ic_chatwindow_titlebar_type = 0;
	if(!ic_chatwindow_titlebar_type) {
		static const GTypeInfo ic_chatwindow_titlebar_info = {
			sizeof(IcChatwindowTitlebarClass),
			NULL,
			NULL,
			(GClassInitFunc)ic_chatwindow_titlebar_class_init,
			NULL,
			NULL,
			sizeof(IcChatwindowTitlebar),
			0,
			(GInstanceInitFunc)ic_chatwindow_titlebar_init
		};
		ic_chatwindow_titlebar_type = g_type_register_static(GTK_TYPE_VBOX, "IcChatwindowTitlebar", &ic_chatwindow_titlebar_info, 0);
	}
	return ic_chatwindow_titlebar_type;
}

// 初始化类结构
static void ic_chatwindow_titlebar_class_init(IcChatwindowTitlebarClass *ic_chatwindow_titlebar_class) {
	GtkWidgetClass *object_class;
	object_class = (GtkWidgetClass*)ic_chatwindow_titlebar_class;
	ic_chatwindow_titlebar_signals[IC_CHATWINDOW_TITLEBAR_DESTROY_SIGNAL]
		= g_signal_new("ic-chatwindow-destroy",
					G_TYPE_FROM_CLASS(object_class),
					G_SIGNAL_RUN_FIRST,
					0,
					NULL, NULL,
					g_cclosure_marshal_VOID__VOID,
					G_TYPE_NONE, 0);
}

// 初始化实例结构
static void ic_chatwindow_titlebar_init(IcChatwindowTitlebar *ic_title_bar) {
	GtkWidget *h_box1, *h_box2;
	GdkPixbuf *pixbuf;

	h_box1 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	h_box2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

	ic_title_bar->nick_name = gtk_label_new(NULL);
	ic_title_bar->signature = gtk_label_new(NULL);
	gtk_box_pack_start(GTK_BOX(h_box1), ic_title_bar->nick_name, FALSE, FALSE, 0);

	ic_title_bar->menu_button = ic_button_new();
	gtk_widget_set_size_request(ic_title_bar->menu_button, 27, 21);
	pixbuf = gdk_pixbuf_new_from_file_at_scale(RESDIR"images/menu1.png", 27, 21, FALSE, NULL);
	ic_button_set_image(ic_title_bar->menu_button, pixbuf);
	pixbuf = gdk_pixbuf_new_from_file_at_scale(RESDIR"images/menu2.png", 27, 21, FALSE, NULL);
	ic_button_set_hovered_image(ic_title_bar->menu_button, pixbuf);
	pixbuf = gdk_pixbuf_new_from_file_at_scale(RESDIR"images/menu3.png", 27, 21, FALSE, NULL);
	ic_button_set_pressed_image(ic_title_bar->menu_button, pixbuf);

	ic_title_bar->min_button = ic_button_new();
	gtk_widget_set_size_request(ic_title_bar->min_button, 27, 21);
	pixbuf = gdk_pixbuf_new_from_file_at_scale(RESDIR"images/min1.png", 27, 21, FALSE, NULL);
	ic_button_set_image(ic_title_bar->min_button, pixbuf);
	pixbuf = gdk_pixbuf_new_from_file_at_scale(RESDIR"images/min2.png", 27, 21, FALSE, NULL);
	ic_button_set_hovered_image(ic_title_bar->min_button, pixbuf);
	pixbuf = gdk_pixbuf_new_from_file_at_scale(RESDIR"images/min3.png", 27, 21, FALSE, NULL);
	ic_button_set_pressed_image(ic_title_bar->min_button, pixbuf);

	ic_title_bar->max_button = ic_button_new();
	gtk_widget_set_size_request(ic_title_bar->max_button, 27, 21);
	pixbuf = gdk_pixbuf_new_from_file_at_scale(RESDIR"images/max1.png", 27, 21, FALSE, NULL);
	ic_button_set_image(ic_title_bar->max_button, pixbuf);
	pixbuf = gdk_pixbuf_new_from_file_at_scale(RESDIR"images/max2.png", 27, 21, FALSE, NULL);
	ic_button_set_hovered_image(ic_title_bar->max_button, pixbuf);
	pixbuf = gdk_pixbuf_new_from_file_at_scale(RESDIR"images/max3.png", 27, 21, FALSE, NULL);
	ic_button_set_pressed_image(ic_title_bar->max_button, pixbuf);

	ic_title_bar->close_button = ic_button_new();
	gtk_widget_set_size_request(ic_title_bar->close_button, 38, 21);
	pixbuf = gdk_pixbuf_new_from_file_at_scale(RESDIR"images/close1.png", 38, 21, FALSE, NULL);
	ic_button_set_image(ic_title_bar->close_button, pixbuf);
	pixbuf = gdk_pixbuf_new_from_file_at_scale(RESDIR"images/close2.png", 38, 21, FALSE, NULL);
	ic_button_set_hovered_image(ic_title_bar->close_button, pixbuf);
	pixbuf = gdk_pixbuf_new_from_file_at_scale(RESDIR"images/close3.png", 38, 21, FALSE, NULL);
	ic_button_set_pressed_image(ic_title_bar->close_button, pixbuf);

	g_signal_connect_swapped(G_OBJECT(ic_title_bar->close_button), "button-release-event",
			G_CALLBACK(ic_on_close_button_pressed), ic_title_bar);

	gtk_box_pack_end(GTK_BOX(h_box1), ic_title_bar->close_button, FALSE, FALSE, 0);
	gtk_box_pack_end(GTK_BOX(h_box1), ic_title_bar->max_button, FALSE, FALSE, 0);
	gtk_box_pack_end(GTK_BOX(h_box1), ic_title_bar->min_button, FALSE, FALSE, 0);
	gtk_box_pack_end(GTK_BOX(h_box1), ic_title_bar->menu_button, FALSE, FALSE, 0);

	gtk_box_pack_start(GTK_BOX(h_box2), ic_title_bar->signature, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(ic_title_bar), h_box1, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(ic_title_bar), h_box2, FALSE, FALSE, 0);
}

// 创建新的自定义构建
GtkWidget *ic_chatwindow_titlebar_new(void) {
	return GTK_WIDGET(g_object_new(IC_TYPE_CHATWINDOW_TITLEBAR, 0));
}

void ic_chatwindow_titlebar_set_label(GtkWidget *widget, gchar *label) {
	gtk_label_set_text(GTK_LABEL(IC_CHATWINDOW_TITLEBAR(widget)->nick_name), label);
}
void ic_chatwindow_titlebar_set_signatrue(GtkWidget *widget, gchar *label) {
	gtk_label_set_text(GTK_LABEL(IC_CHATWINDOW_TITLEBAR(widget)->signature), label);
}

GtkWidget *ic_chatwindow_titlebar_new_with_label(gchar *label) {
	GtkWidget *ic_title_bar;
	ic_title_bar = ic_chatwindow_titlebar_new();
	ic_chatwindow_titlebar_set_label(ic_title_bar, label);
	return ic_title_bar;
}

static void ic_on_close_button_pressed(GtkWidget *widget, gpointer data)
{
	g_signal_emit((gpointer)widget, ic_chatwindow_titlebar_signals[IC_CHATWINDOW_TITLEBAR_DESTROY_SIGNAL], 
	              0);
}

