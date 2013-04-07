#include "ic_window_titlebar.h"
#include "ic_button.h"

static void ic_window_titlebar_init(IcWindowTitlebar *title_bar);
static void ic_window_titlebar_class_init(IcWindowTitlebarClass *title_bar_class);

G_DEFINE_TYPE(IcWindowTitlebar, ic_window_titlebar, IC_TYPE_TITLEBAR);

static void ic_window_titlebar_class_init(IcWindowTitlebarClass *class)
{
}

// 初始化实例结构
static void ic_window_titlebar_init(IcWindowTitlebar *title_bar) {
	GtkWidget *h_box1, *h_box2;
	GdkPixbuf *pixbuf;

    IcTitlebar *parent = IC_TITLEBAR(title_bar);
	h_box1 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	h_box2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

	parent->label = gtk_label_new(NULL);
	title_bar->signature = gtk_label_new(NULL);
	gtk_box_pack_start(GTK_BOX(h_box1), parent->label, FALSE, FALSE, 0);

	title_bar->menu_btn = ic_button_new();
	gtk_widget_set_size_request(title_bar->menu_btn, 27, 21);
	pixbuf = gdk_pixbuf_new_from_file_at_scale(RESDIR"images/menu1.png", 27, 21, FALSE, NULL);
	ic_button_set_normal_image(title_bar->menu_btn, pixbuf);
    g_object_unref(pixbuf);
	pixbuf = gdk_pixbuf_new_from_file_at_scale(RESDIR"images/menu2.png", 27, 21, FALSE, NULL);
	ic_button_set_hovered_image(title_bar->menu_btn, pixbuf);
    g_object_unref(pixbuf);
	pixbuf = gdk_pixbuf_new_from_file_at_scale(RESDIR"images/menu3.png", 27, 21, FALSE, NULL);
	ic_button_set_pressed_image(title_bar->menu_btn, pixbuf);
    g_object_unref(pixbuf);
	
    parent->min_btn = ic_button_new();
	gtk_widget_set_size_request(parent->min_btn, 27, 21);
	pixbuf = gdk_pixbuf_new_from_file_at_scale(RESDIR"images/min1.png", 27, 21, FALSE, NULL);
	ic_button_set_image(parent->min_btn, pixbuf);
    g_object_unref(pixbuf);
	pixbuf = gdk_pixbuf_new_from_file_at_scale(RESDIR"images/min2.png", 27, 21, FALSE, NULL);
	ic_button_set_hovered_image(parent->min_btn, pixbuf);
    g_object_unref(pixbuf);
	pixbuf = gdk_pixbuf_new_from_file_at_scale(RESDIR"images/min3.png", 27, 21, FALSE, NULL);
	ic_button_set_pressed_image(parent->min_btn, pixbuf);
    g_object_unref(pixbuf);

	parent->max_btn = ic_button_new();
	gtk_widget_set_size_request(parent->max_btn, 27, 21);
	pixbuf = gdk_pixbuf_new_from_file_at_scale(RESDIR"images/max1.png", 27, 21, FALSE, NULL);
	ic_button_set_image(parent->max_btn, pixbuf);
    g_object_unref(pixbuf);
	pixbuf = gdk_pixbuf_new_from_file_at_scale(RESDIR"images/max2.png", 27, 21, FALSE, NULL);
	ic_button_set_hovered_image(parent->max_btn, pixbuf);
    g_object_unref(pixbuf);
	pixbuf = gdk_pixbuf_new_from_file_at_scale(RESDIR"images/max3.png", 27, 21, FALSE, NULL);
	ic_button_set_pressed_image(parent->max_btn, pixbuf);
    g_object_unref(pixbuf);

	parent->close_btn = ic_button_new();
	gtk_widget_set_size_request(parent->close_btn, 38, 21);
	pixbuf = gdk_pixbuf_new_from_file_at_scale(RESDIR"images/close1.png", 38, 21, FALSE, NULL);
	ic_button_set_image(parent->close_btn, pixbuf);
    g_object_unref(pixbuf);
	pixbuf = gdk_pixbuf_new_from_file_at_scale(RESDIR"images/close2.png", 38, 21, FALSE, NULL);
	ic_button_set_hovered_image(parent->close_btn, pixbuf);
    g_object_unref(pixbuf);
	pixbuf = gdk_pixbuf_new_from_file_at_scale(RESDIR"images/close3.png", 38, 21, FALSE, NULL);
	ic_button_set_pressed_image(parent->close_btn, pixbuf);
    g_object_unref(pixbuf);

	gtk_box_pack_end(GTK_BOX(h_box1), parent->close_btn, FALSE, FALSE, 0);
	gtk_box_pack_end(GTK_BOX(h_box1), parent->max_btn, FALSE, FALSE, 0);
	gtk_box_pack_end(GTK_BOX(h_box1), parent->min_btn, FALSE, FALSE, 0);
	gtk_box_pack_end(GTK_BOX(h_box1), title_bar->menu_btn, FALSE, FALSE, 0);

	gtk_box_pack_start(GTK_BOX(h_box2), title_bar->signature, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(title_bar), h_box1, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(title_bar), h_box2, FALSE, FALSE, 0);
}

// 创建新的自定义构建
GtkWidget *ic_window_titlebar_new(void) {
	return GTK_WIDGET(g_object_new(IC_TYPE_WINDOW_TITLEBAR, 0));
}

void ic_window_titlebar_set_label(GtkWidget *widget, gchar *label) {
	gtk_label_set_text(GTK_LABEL(IC_TITLEBAR(widget)->label), label);
}
void ic_window_titlebar_set_signatrue(GtkWidget *widget, gchar *label) {
	gtk_label_set_text(GTK_LABEL(IC_WINDOW_TITLEBAR(widget)->signature), label);
}

GtkWidget *ic_window_titlebar_new_with_label(gchar *label) {
	GtkWidget *title_bar;
	title_bar = ic_window_titlebar_new();
	ic_window_titlebar_set_label(title_bar, label);
	return title_bar;
}

