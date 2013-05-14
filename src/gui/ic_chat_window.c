#include "ic_chat_window.h"
#include "ic_window_titlebar.h"
#include "ic_toolbar.h"
#include "ic_input_toolbar.h"
#include "ic_button.h"
#include "ic_data_type.h"
#include "type.h"
#include <string.h>

struct _IcChatWindowPrivate
{
	GtkWidget *title_bar;
	GtkWidget *tool_bar;
	GtkWidget *input_toolbar;
	GtkWidget *display_view;
	GtkWidget *input_view;
	LwqqBuddy *friend;
};

static void ic_chat_window_class_init(IcChatWindowClass *class);
static void ic_chat_window_init(IcChatWindow *chat_window);

static gboolean on_chatwindow_draw(GtkWidget *chat_window, gpointer data);
static gboolean on_mouse_press_event (GtkWidget* widget,
                GdkEventButton * event, gpointer data);

static void on_send_mesg(GtkWidget *widget);
static void on_chat_window_destroy(GtkWidget *widget);

static void ic_chatwindow_textview_create_tags(GtkWidget *widget);
static void ic_chatwindow_textview_add_mesg(GtkWidget *widget, gchar *name, 
                                            gint64 time, gchar *mesg, 
                                            gchar *color_tag);
static void ic_chatwindow_textview_clear(GtkWidget *widget);

GType ic_chat_window_get_type ()
{
	static GType chat_window_type = 0;
	if(!chat_window_type)
	{
		static const GTypeInfo chat_window_info = {
			sizeof(IcChatWindowClass),
			NULL,
			NULL,
			(GClassInitFunc)ic_chat_window_class_init,
			NULL,
			NULL,
			sizeof(IcChatWindow),
			0,
			(GInstanceInitFunc)ic_chat_window_init
		};
		chat_window_type = g_type_register_static (GTK_TYPE_WINDOW
		                                           , "IcChatWindow",
		                                           &chat_window_info, 0);
	}
	return chat_window_type;
}

static void ic_chat_window_class_init(IcChatWindowClass *chat_window_class)
{
	g_type_class_add_private(chat_window_class, sizeof(IcChatWindowPrivate));	
}

static void ic_chat_window_init(IcChatWindow *chat_window)
{
	chat_window->private = G_TYPE_INSTANCE_GET_PRIVATE(chat_window,
	                                                   IC_TYPE_CHAT_WINDOW,
	                                                   IcChatWindowPrivate);
	IcChatWindowPrivate *private = chat_window->private;
	
	GtkWidget *top_event_box;
	GtkWidget *display_scrolled;
	GtkWidget *input_scrolled;
	GtkWidget *status_bar;
	GtkWidget *forum_label;
	GtkWidget *close_button;
	GtkWidget *send_button;
	GtkWidget *face_frame;
	GtkWidget *img_event_box;
	GtkWidget *image;
	GdkPixbuf *pixbuf;

	GtkWidget *h_box1, *h_box2, *h_box3;
	GtkWidget *v_box1, *v_box2, *v_box3, *v_box4;
	GtkWidget *grid;
	
	gtk_window_set_decorated(GTK_WINDOW(chat_window), FALSE);
	
	gint default_width = 550;
	gint default_height = 500;
	gtk_window_set_default_size(GTK_WINDOW(chat_window), default_width, default_height);
	gtk_window_set_position(GTK_WINDOW(chat_window), GTK_WIN_POS_CENTER);
		
	h_box1 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	h_box2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	h_box3 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

	v_box1 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	v_box2 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	v_box3 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	v_box4 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

	top_event_box = gtk_event_box_new();
	GdkColor color1 = {0, 30313, 51153, 63408};
	gtk_widget_modify_bg(top_event_box, GTK_STATE_NORMAL, &color1);
	gtk_container_add(GTK_CONTAINER(top_event_box), h_box3);
	gtk_box_pack_start(GTK_BOX(v_box1), top_event_box, FALSE, FALSE, 0);

	face_frame = gtk_frame_new(NULL);
	pixbuf = gdk_pixbuf_new_from_file_at_scale(RESDIR"images/head.png", 60, 66, FALSE, NULL);
	image = gtk_image_new_from_pixbuf(pixbuf);
    g_object_unref(pixbuf);
	gtk_container_add(GTK_CONTAINER(face_frame), image);
	gtk_box_pack_end(GTK_BOX(v_box4), face_frame, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(h_box3), v_box4, FALSE, FALSE, 3);

	gtk_box_pack_start(GTK_BOX(h_box3), v_box3, TRUE, TRUE, 0);

	private->title_bar = ic_window_titlebar_new();
	gtk_box_pack_start(GTK_BOX(v_box3), private->title_bar, FALSE, FALSE, 0);

	private->tool_bar = ic_tool_bar_new();
	gtk_box_pack_start(GTK_BOX(v_box3), private->tool_bar, FALSE, FALSE, 0);

	grid = gtk_grid_new();
	gtk_box_pack_start(GTK_BOX(h_box1), grid, TRUE, TRUE, 0);

	private->input_view = gtk_text_view_new();
	gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW(private->input_view), GTK_WRAP_CHAR);
	gtk_text_view_set_left_margin (GTK_TEXT_VIEW(private->input_view), 5);
	gtk_widget_grab_focus(private->input_view);

	input_scrolled = gtk_scrolled_window_new(NULL, NULL);
	gtk_widget_set_hexpand(input_scrolled, TRUE);
	gtk_widget_set_vexpand(input_scrolled, FALSE);
	gtk_widget_set_size_request(input_scrolled, 400, 100);
	gtk_container_add(GTK_CONTAINER(input_scrolled), private->input_view);
	
	private->display_view = gtk_text_view_new();
	gtk_text_view_set_editable (GTK_TEXT_VIEW(private->display_view), FALSE);
	gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW(private->display_view), GTK_WRAP_CHAR);
	gtk_text_view_set_cursor_visible (GTK_TEXT_VIEW(private->display_view), FALSE);
	ic_chatwindow_textview_create_tags (private->display_view);
	
	display_scrolled = gtk_scrolled_window_new(NULL, NULL);
	gtk_widget_set_hexpand(display_scrolled, TRUE);
	gtk_widget_set_vexpand(display_scrolled, TRUE);
	gtk_container_add(GTK_CONTAINER(display_scrolled), private->display_view);
	
	gtk_grid_attach(GTK_GRID(grid), display_scrolled, 0, 0, 1, 4);

	private->input_toolbar = ic_input_toolbar_new();
	GdkColor color = {0, 52171, 57825, 61166};
	gtk_widget_modify_bg(private->input_toolbar, GTK_STATE_NORMAL, &color);

	gtk_grid_attach(GTK_GRID(grid), private->input_toolbar, 0, 4, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), input_scrolled, 0, 5, 1, 3);

	status_bar = gtk_event_box_new();
	gtk_widget_modify_bg(status_bar, GTK_STATE_NORMAL, &color);

	forum_label = gtk_label_new("<a href=\"http://gtk.awaysoft.com/\">"
	                               "Gtk+ forum</a>");
	gtk_label_set_use_markup (GTK_LABEL (forum_label), TRUE);

	close_button = gtk_button_new_with_label("关 闭");
	gtk_button_set_focus_on_click(GTK_BUTTON(close_button), FALSE);
	send_button = gtk_button_new_with_label("发 送");
	gtk_button_set_focus_on_click(GTK_BUTTON(send_button), FALSE);

	gtk_box_pack_start(GTK_BOX(h_box2), forum_label, FALSE, FALSE, 0);
	gtk_box_pack_end(GTK_BOX(h_box2), send_button, FALSE, FALSE, 5);
	gtk_box_pack_end(GTK_BOX(h_box2), close_button, FALSE, FALSE, 5);
	gtk_container_add(GTK_CONTAINER(status_bar), h_box2);
	gtk_grid_attach(GTK_GRID(grid), status_bar, 0, 8, 1, 1);

	img_event_box = gtk_event_box_new();
	image = gtk_image_new_from_file(RESDIR"images/test1.png");
	gtk_container_add(GTK_CONTAINER(img_event_box), image);
	gtk_box_pack_start(GTK_BOX(v_box2), img_event_box, FALSE, FALSE, 0);

	img_event_box = gtk_event_box_new();
	image = gtk_image_new_from_file(RESDIR"images/test2.png");
	gtk_container_add(GTK_CONTAINER(img_event_box), image);
	gtk_box_pack_end(GTK_BOX(v_box2), img_event_box, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(h_box1), v_box2, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(v_box1), h_box1, TRUE, TRUE, 0);
	gtk_container_add(GTK_CONTAINER(chat_window), v_box1);

	g_signal_connect_swapped(G_OBJECT(send_button), "clicked", 
	                 G_CALLBACK(on_send_mesg), (gpointer)chat_window);
	g_signal_connect_swapped(G_OBJECT(private->title_bar), "ic-titlebar-destroy", 
	                 G_CALLBACK(on_chat_window_destroy), (gpointer)chat_window);
	g_signal_connect(G_OBJECT(chat_window), "draw", 
	                 G_CALLBACK(on_chatwindow_draw), NULL);
	g_signal_connect(G_OBJECT(chat_window), "button_press_event",
	                 G_CALLBACK(on_mouse_press_event), NULL);
}

GtkWidget *ic_chat_window_new_with_info(LwqqBuddy *friend)
{
    GtkWidget *widget = ic_chat_window_new();
    ic_chat_window_set_info(widget, friend);
}

GtkWidget *ic_chat_window_new()
{
    GtkWidget *widget = GTK_WIDGET(g_object_new(IC_TYPE_CHAT_WINDOW, 0));  
    return widget;
}

static gboolean on_chatwindow_draw(GtkWidget *chat_window, gpointer data)
{
	cairo_t *cr;
	cairo_surface_t *surface;
	cairo_region_t *mask;

	surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, 
	                                      gtk_widget_get_allocated_width (chat_window),
	                                      gtk_widget_get_allocated_height (chat_window));
	cr = cairo_create(surface);
	gint width, height, r;
	gtk_window_get_size(GTK_WINDOW(chat_window), &width, &height);
	r = 5;

	/*----画圆角矩形，然后遮罩实现窗口圆角---*/
	cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_set_line_width (cr, 1);

	cairo_move_to (cr, r, 0);
	cairo_line_to (cr, width-r, 0);

	cairo_move_to (cr, width, r);
	cairo_line_to (cr, width, height-r);

	cairo_move_to (cr, width-r, height);
	cairo_move_to (cr, r, height);

	cairo_move_to (cr, 0, height-r);
	cairo_line_to (cr, 0, r);

	cairo_arc (cr, r, r, r, G_PI, 3 * G_PI / 2.0 );
	cairo_arc (cr, width-r, r, r, 3 * G_PI /2.0, 2 * G_PI);
	cairo_arc (cr, width-r, height-r, r, 0, G_PI / 2);
	cairo_arc (cr, r, height-r, r, G_PI / 2, G_PI);

	cairo_stroke_preserve (cr);
	cairo_set_source_rgb ( cr, 1 , 1 , 1) ;
	cairo_fill (cr) ;	

	mask = gdk_cairo_region_create_from_surface(surface);
	gtk_widget_shape_combine_region(chat_window, mask);

	cairo_destroy ( cr) ;
	cairo_region_destroy (mask);
	cairo_surface_destroy (surface);
	
	return TRUE;
}

static gboolean on_mouse_press_event (GtkWidget* widget,
                GdkEventButton * event, gpointer data)
{
	if (event->type == GDK_BUTTON_PRESS)
	{
		if (event->button == 1) {
			gint root_x, root_y;
			gdk_window_get_origin (gtk_widget_get_window (widget), &root_x, &root_y);
			if(event->y_root - root_y < 60)
			{
				gtk_window_begin_move_drag(GTK_WINDOW(gtk_widget_get_toplevel(widget)),
					event->button,
					event->x_root,
					event->y_root,
					event->time);
			}
		}
	}
	return FALSE;
}

void ic_chat_window_set_info(GtkWidget *chat_window, LwqqBuddy *friend)
{
	IcChatWindowPrivate *private = G_TYPE_INSTANCE_GET_PRIVATE(chat_window,
	                                                        IC_TYPE_CHAT_WINDOW,
	                                                        IcChatWindowPrivate);
	private->friend = friend;

	GtkWidget *titlebar = private->title_bar;
	ic_window_titlebar_set_label(titlebar, friend->nick);
	ic_window_titlebar_set_signatrue(titlebar, friend->long_nick);
}

void ic_chat_window_show(GtkWidget *widget)
{
	gtk_widget_show_all(widget);
}

static void on_chat_window_destroy(GtkWidget *chat_window)
{	
	IcChatWindowPrivate *private = G_TYPE_INSTANCE_GET_PRIVATE(chat_window,
	                                                        IC_TYPE_CHAT_WINDOW,
	                                                        IcChatWindowPrivate);
    LwqqBuddy *friend = private->friend;
    ic_chat_entity_destroy (friend->qqnumber);
}

static void ic_chatwindow_textview_create_tags(GtkWidget *widget)
{
	GtkTextBuffer *textbuffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW(widget));
	
	gtk_text_buffer_create_tag (GTK_TEXT_BUFFER(textbuffer), "green_fg",
	                            "foreground", "green", NULL);
	gtk_text_buffer_create_tag (GTK_TEXT_BUFFER(textbuffer), "blue_fg",
	                            "foreground", "blue", NULL);
	gtk_text_buffer_create_tag (GTK_TEXT_BUFFER(textbuffer), "underline",
	                            "underline", PANGO_UNDERLINE_SINGLE, NULL);
	gtk_text_buffer_create_tag (GTK_TEXT_BUFFER(textbuffer), "left_margin1",
	                            "left_margin", 5, NULL);
	gtk_text_buffer_create_tag (GTK_TEXT_BUFFER(textbuffer), "left_margin2",
	                            "left_margin", 20, NULL);
}

void ic_chatwindow_append_recvmesg(GtkWidget *widget, Message *mesg)
{
	IcChatWindowPrivate *private = G_TYPE_INSTANCE_GET_PRIVATE(widget,
	                                                           IC_TYPE_CHAT_WINDOW,
	                                                           IcChatWindowPrivate);
	GtkWidget *display_view = private->display_view;
	/*FriendInfo *friend_info = ic_get_friendinfo_by_name(mesg->sender);
	
	ic_chatwindow_textview_add_mesg(display_view, friend_info->nick_name, mesg->time, 
	                                mesg->content, "blue_fg");
    */
}

static void on_send_mesg(GtkWidget *widget)
{
	IcChatWindowPrivate *private = G_TYPE_INSTANCE_GET_PRIVATE(widget,
	                                                           IC_TYPE_CHAT_WINDOW,
	                                                           IcChatWindowPrivate);
	GtkWidget *input_view = private->input_view;
	GtkTextBuffer *buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW(input_view));
	GtkTextIter start_iter, end_iter;
	gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(buffer), &start_iter, &end_iter);
	gchar *mesg = gtk_text_buffer_get_text(GTK_TEXT_BUFFER(buffer), &start_iter, &end_iter, TRUE);
	if(mesg == NULL || strcmp(mesg, "")==0)
		return;

	/*UserInfo *user_info = ic_get_user_info ();
	FriendInfo *friend_info = private->friend_info;
	GtkWidget *display_view = private->display_view;

	GTimeVal cur_time;
	g_get_current_time (&cur_time);

	ic_chatwindow_textview_clear (input_view);
	ic_chatwindow_textview_add_mesg (display_view, user_info->nick_name,
	                                 cur_time.tv_sec, mesg, "blue_fg");
    */	
	g_free(mesg);
}

static void ic_chatwindow_textview_add_mesg(GtkWidget *widget, gchar *name, 
                                            gint64 time, gchar *mesg, 
                                            gchar *color_tag)
{
	GtkTextBuffer *textbuffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW(widget));
	GtkTextIter end_iter;
	GtkTextMark *end_mark;

	if(gtk_text_buffer_get_line_count (textbuffer) > 1)
	{
		gtk_text_buffer_get_end_iter (GTK_TEXT_BUFFER(textbuffer), &end_iter);
		gtk_text_buffer_insert(GTK_TEXT_BUFFER(textbuffer), &end_iter, "\n", -1);
	}
	
	gchar head[100];
	GDateTime *date_time = g_date_time_new_from_unix_local(time);
	g_snprintf(head, 100, "%s %d-%d-%d %d:%d:%d\n", name, 
	         	g_date_time_get_year (date_time),
	           	g_date_time_get_month (date_time),
	           	g_date_time_get_day_of_month (date_time),
	           	g_date_time_get_hour (date_time),
	           	g_date_time_get_minute (date_time),
	           	g_date_time_get_second (date_time));
	g_date_time_unref (date_time);

	gtk_text_buffer_get_end_iter(GTK_TEXT_BUFFER(textbuffer), &end_iter);
	gtk_text_buffer_insert_with_tags_by_name(GTK_TEXT_BUFFER(textbuffer), 
	                                         &end_iter, head, -1, color_tag, 
	                                         "left_margin1", NULL);
	gtk_text_buffer_insert_with_tags_by_name(GTK_TEXT_BUFFER(textbuffer), 
	                                         &end_iter, mesg, -1, "left_margin2",
	                                         NULL);
	
	end_mark = gtk_text_buffer_get_insert (textbuffer);
	gtk_text_buffer_move_mark (GTK_TEXT_BUFFER(textbuffer), end_mark, &end_iter);
	gtk_text_view_scroll_mark_onscreen (GTK_TEXT_VIEW(widget), end_mark);

}

static void ic_chatwindow_textview_clear(GtkWidget *widget)
{
	GtkTextBuffer *textbuffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW(widget));
	gtk_text_buffer_set_text (GTK_TEXT_BUFFER(textbuffer), "", -1);
}
