#include "ic_loading_panel.h"
#include "ic_titlebar.h"
#include <string.h>
#include <stdio.h>

static char user_name[20] = {0};
static gint count = 0;
static gboolean plus_or_minus;
static gboolean is_destroy;

static gboolean on_mouse_press_event (GtkWidget* widget,
                GdkEventButton * event, gpointer data);
static gboolean timer(GtkWidget *widget);

GtkWidget *ic_loading_panel_new()
{
	GtkWidget *window;
	GtkWidget *titlebar;
	GtkWidget *vbox;
	GtkWidget *cancel_button;
	GtkWidget *align;
	GtkWidget *label2;
	GtkWidget *image1, *image2, *image3;
	GdkPixbuf *pixbuf1, *pixbuf2, *new_pixbuf;
	GdkScreen *scr;
	gint pic_width, pic_height;
	gint new_width, new_height;
	gint scr_width;
	GdkScreen *screen;
	GdkVisual *visual;

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_decorated (GTK_WINDOW(window), FALSE);
	gtk_window_set_default_size(GTK_WINDOW(window), 280, 580);
	gtk_window_set_keep_above (GTK_WINDOW(window), TRUE);
	
	screen = gtk_widget_get_screen(window);
	visual = gdk_screen_get_rgba_visual(screen);
	gtk_widget_set_visual(window, visual);
	GdkColor color = {0, 30313, 51153, 63408};
	gtk_widget_modify_bg(window, GTK_STATE_NORMAL, &color);
	
	scr = gdk_screen_get_default();
	scr_width = gdk_screen_get_width(scr);
	gtk_window_move(GTK_WINDOW(window), scr_width-350, 50);

	vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	titlebar = ic_titlebar_new ();
	gtk_box_pack_start (GTK_BOX(vbox), titlebar, FALSE, FALSE, 0);
	
	/*label1 = gtk_label_new("\niChat");
	PangoFontDescription *font_desc = pango_font_description_from_string("Purisa Bold 25");
	gtk_widget_modify_font(label1, font_desc);
	pango_font_description_free(font_desc);

	gtk_widget_set_size_request(label1, 100, 25);
	gtk_box_pack_start(GTK_BOX(vbox), label1, FALSE, FALSE, 0);
    */
    image3 = gtk_image_new_from_file(RESDIR"images/ichat.png");
    gtk_box_pack_start(GTK_BOX(vbox), image3, FALSE, FALSE, 0);

	pixbuf1 = gdk_pixbuf_new_from_file(RESDIR"images/loading/loading-01.png", NULL);
	image1 = gtk_image_new_from_pixbuf(pixbuf1);
	gtk_box_pack_start(GTK_BOX(vbox), image1, FALSE, FALSE, 0);

	pixbuf2 = gdk_pixbuf_new_from_file(RESDIR"images/gtk.png", NULL);
	pic_width = gdk_pixbuf_get_width(pixbuf2);
	pic_height = gdk_pixbuf_get_height(pixbuf2);
	new_width = 150;
	new_height = ((float)new_width/pic_width)*pic_height;
	
	new_pixbuf = gdk_pixbuf_scale_simple (pixbuf2, new_width, new_height, GDK_INTERP_BILINEAR);
	image2 = gtk_image_new_from_pixbuf(new_pixbuf);
	gtk_box_pack_start(GTK_BOX(vbox), image2, FALSE, FALSE, 5);

	gchar str[30];
	memset(str, 0, sizeof(str));
	strcat(str, user_name);
	strcat(str, "正在登录");
	label2 = gtk_label_new(str);
	gtk_box_pack_start(GTK_BOX(vbox), label2, FALSE, FALSE, 5);
	
	align = gtk_alignment_new(0.5, 0.5, 0, 0);
	cancel_button = gtk_button_new_with_label("  取 消  ");
	gtk_container_add(GTK_CONTAINER(align), cancel_button);
	gtk_box_pack_start(GTK_BOX(vbox), align, FALSE, FALSE, 5);

	gtk_widget_set_events(window, GDK_EXPOSURE_MASK | GDK_LEAVE_NOTIFY_MASK
			| GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK | GDK_POINTER_MOTION_MASK
			| GDK_POINTER_MOTION_HINT_MASK);
	g_signal_connect(G_OBJECT(window), "button_press_event",
			G_CALLBACK(on_mouse_press_event), NULL);
	g_signal_connect(G_OBJECT(window), "delete-event", G_CALLBACK(gtk_main_quit), NULL);
	g_timeout_add(100, (GSourceFunc)timer, (gpointer)image1);

	gtk_container_add(GTK_CONTAINER(window), vbox);
	return window;
}

GtkWidget *ic_loading_panel_new_with_label(const gchar *str)
{
	GtkWidget *loading_window = ic_loading_panel_new();
	strcpy(user_name, str);
	return loading_window;
}

void ic_loading_panel_show(GtkWidget *widget) {
	is_destroy = FALSE;
	gtk_widget_show_all(widget);
}

void ic_loading_panel_destroy(GtkWidget *window) {
	is_destroy = TRUE;
	gtk_widget_destroy(window);
}

static gboolean on_mouse_press_event (GtkWidget* widget,
                GdkEventButton * event, gpointer data)
{
	if (event->type == GDK_BUTTON_PRESS)
	{
		if (event->button == 1) {
			gtk_window_begin_move_drag(GTK_WINDOW(gtk_widget_get_toplevel(widget)),
				event->button,
				event->x_root,
				event->y_root,
				event->time);
		}
	}
	return FALSE;
}

static gboolean timer(GtkWidget *image) {
	GdkPixbuf *pixbuf;
	int i;
	char str[200];
	str[0]='\0';
	char str1[]=RESDIR"images/loading/loading-";
	char str2[]=".png";

	strcat(str, str1);
	
	i=count%11+1;
	if(i==1){
		plus_or_minus = TRUE;
	}else if(i==11){
		plus_or_minus = FALSE;
	}
	
	if(plus_or_minus){
		count++;
	}else{
		count--;
	}

	char chs[3];
	if(i<=9){
		chs[0]='0';
		chs[1]='0'+i;
		chs[2]='\0';
	}else{
		chs[0]='0'+i/10;
		chs[1]='0'+i%10;
		chs[2]='\0';
	}
	strcat(str, chs);
	strcat(str, str2);
	if(!is_destroy) {
		pixbuf = gdk_pixbuf_new_from_file(str, NULL);
		gtk_image_set_from_pixbuf((GtkImage *)image, pixbuf);
		return TRUE;
	}
	return FALSE;
}

