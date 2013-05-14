#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ic_main_panel.h"
#include "ic_data_type.h"
#include "ic_chat_manager.h"
#include "ic_text_field.h"
#include "ic_status_box.h"
#include "ic_titlebar.h"

#include "queue.h"

enum{
	COL_PIXBUF=0,
    COL_STRING,
	COL_USERNAME,
    COLS_N
};

static GtkWidget *ic_contact_list_new(LwqqClient *lc);
static GtkTreeModel *ic_contact_tree_model_new(LwqqClient *lc);
static gboolean ic_status_changed_event(GtkWidget *widget, gpointer data);
static void on_row_activated(GtkTreeView *view, GtkTreePath *path,
                         GtkTreeViewColumn *column, gpointer data);
static gboolean on_mouse_press_event (GtkWidget* widget,
                GdkEventButton * event, gpointer data);
static void on_main_panel_close(GtkWidget *widget);

static gboolean ic_status_changed_event(GtkWidget *widget, gpointer data) {
	gint status = ic_status_box_get_status(IC_STATUS_BOX(widget));
	UserInfo *user = (UserInfo *)data;
	user->status = status;

	return TRUE;
}

static GtkTreeModel *ic_contact_tree_model_new(LwqqClient *lc) {
	GtkTreeStore *contactTreeStore;
	GtkTreeIter root, child;
	GdkPixbuf *pixbuf;
	GList *iter = NULL;

	contactTreeStore = gtk_tree_store_new(COLS_N, GDK_TYPE_PIXBUF, 
	                                      G_TYPE_STRING, G_TYPE_STRING);

    LwqqFriendCategory *category = NULL;
    LIST_FOREACH(category, &lc->categories, entries)
    {
        gtk_tree_store_append(contactTreeStore, &root, NULL);
        gtk_tree_store_set(contactTreeStore, &root, COL_STRING, category->name, -1);

        LwqqBuddy *friend = NULL;
        LIST_FOREACH(friend, &lc->friends, entries)
        {
            if(category->index == atoi(friend->cate_index))
            {
                gtk_tree_store_append(contactTreeStore, &child, &root);
                GdkPixbufLoader *loader = gdk_pixbuf_loader_new();
                gdk_pixbuf_loader_write(loader, friend->avatar, friend->avatar_len, NULL);
                pixbuf = gdk_pixbuf_loader_get_pixbuf(loader);
                gtk_tree_store_set(contactTreeStore, &child, COL_PIXBUF, pixbuf, 
                                    COL_STRING, friend->nick, COL_USERNAME, 
                                    friend->qqnumber, -1);
            }
        }
        
    }

	return GTK_TREE_MODEL(contactTreeStore);
}

static GtkWidget *ic_contact_list_new(LwqqClient *lc) {
	GtkWidget* contactView;
	GtkTreeModel *contactTreeModel;
	GtkTreeViewColumn *column;
	GtkCellRenderer *renderer;

	contactView = gtk_tree_view_new();
	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(contactView), FALSE);

	column = gtk_tree_view_column_new();
	gtk_tree_view_append_column(GTK_TREE_VIEW(contactView), column);
	
	renderer = gtk_cell_renderer_pixbuf_new();
	gtk_tree_view_column_pack_start(column, renderer, FALSE);
	gtk_cell_renderer_set_alignment(GTK_CELL_RENDERER(renderer), 0, 0);
	gtk_cell_renderer_set_fixed_size(GTK_CELL_RENDERER(renderer), 40, 40);

	gtk_tree_view_column_add_attribute(column, renderer, "pixbuf", COL_PIXBUF);
	renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_column_pack_start(column, renderer, TRUE);
	gtk_tree_view_column_add_attribute(column, renderer, "text", COL_STRING);

	contactTreeModel = ic_contact_tree_model_new(lc);
	gtk_tree_view_set_model(GTK_TREE_VIEW(contactView), contactTreeModel);
	g_object_unref(contactTreeModel);	
	gtk_tree_view_expand_all(GTK_TREE_VIEW(contactView));

	g_signal_connect(G_OBJECT(contactView), "row-activated", 
	                 G_CALLBACK(on_row_activated), NULL);
		
	return contactView;
}

static void on_row_activated(GtkTreeView *treeview, GtkTreePath *path,
                         GtkTreeViewColumn *column, gpointer data)
{
	GtkTreeModel *model;
	GtkTreeIter iter;

	model = gtk_tree_view_get_model(treeview);
	if (gtk_tree_model_get_iter(model, &iter, path))
	{
		gchar *qqnumber = NULL;
		gtk_tree_model_get(model, &iter, COL_USERNAME, &qqnumber, -1);
		if(qqnumber != NULL)
		{
			ic_chat_entity_realize(qqnumber);
            g_free(qqnumber);
		}
	}
}

GtkWidget *ic_main_panel_new(LwqqClient *lwqq_client) {
	GtkWidget *mainWindow;
	GtkWidget *title_bar;
	GtkWidget *statusCombo;
	GtkWidget *nickName;
	GtkWidget *signature;
	GtkWidget *hbox1, *hbox2, *hbox3, *hbox4;
	GtkWidget *vbox1, *vbox2;
	GtkWidget *photoFrame;
	GtkWidget *photo;
	GtkWidget *search_entry, *search_btn;
	GtkWidget *notebook;
	GtkWidget *contactView;
	GtkWidget *toolbar;
	GtkWidget *start_btn;
	GtkWidget *label;
	GtkWidget *image;
	
	GdkPixbuf *photoPixbuf;
	GdkScreen *scr;
	gint scrWidth;
	GdkScreen *screen;
	GdkVisual *visual;
	
	mainWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_decorated(GTK_WINDOW(mainWindow), FALSE);
	gtk_window_set_default_size(GTK_WINDOW(mainWindow), 280, 580);
	gtk_window_set_keep_above (GTK_WINDOW(mainWindow), TRUE);

	screen = gtk_widget_get_screen(mainWindow);
	visual = gdk_screen_get_rgba_visual(screen);
	gtk_widget_set_visual(mainWindow, visual);
	
	GdkColor color = {0, 30313, 51153, 63408};
	gtk_widget_modify_bg(mainWindow, GTK_STATE_NORMAL, &color);	
	scr = gdk_screen_get_default();
	scrWidth = gdk_screen_get_width(scr);
	gtk_window_move(GTK_WINDOW(mainWindow), scrWidth-350, 50);

	hbox1 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	hbox2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	hbox3 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	hbox4 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	vbox1 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	vbox2 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
		
	title_bar = ic_titlebar_new();
	gtk_box_pack_start (GTK_BOX(vbox1), title_bar, FALSE, FALSE, 0);
	
	nickName = gtk_label_new(lwqq_client->myself->nick);
	photoFrame = gtk_frame_new(NULL);
	gtk_frame_set_shadow_type(GTK_FRAME(photoFrame), GTK_SHADOW_IN);
	gtk_widget_set_size_request(photoFrame, 60, 60);

    GdkPixbufLoader *loader = gdk_pixbuf_loader_new();
    gdk_pixbuf_loader_write(loader, lwqq_client->myself->avatar, lwqq_client->myself->avatar_len, NULL);
    photoPixbuf = gdk_pixbuf_loader_get_pixbuf(loader);
    GdkPixbuf *myface = gdk_pixbuf_scale_simple(photoPixbuf, 40, 40, GDK_INTERP_BILINEAR);

  	photo = gtk_image_new_from_pixbuf(myface);
	gtk_container_add(GTK_CONTAINER(photoFrame), photo);
    g_object_unref(photoPixbuf);
    g_object_unref(myface);

	statusCombo = ic_status_box_new();
	/*g_signal_connect(G_OBJECT(statusCombo), "ic-status-changed", 
	                 G_CALLBACK(ic_status_changed_event), (gpointer)user);
    */
	signature = ic_text_field_new ();
	ic_text_field_set_text(IC_TEXT_FIELD(signature), lwqq_client->myself->long_nick);
	gtk_widget_set_size_request(signature, 198, 30);
	
	gtk_box_pack_start(GTK_BOX(hbox1), statusCombo, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(hbox1), nickName, FALSE, FALSE, 15);
	gtk_box_pack_start(GTK_BOX(vbox2), hbox1, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox2), signature, TRUE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(hbox2), photoFrame, FALSE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(hbox2), vbox2, FALSE, FALSE, 0);
	
	search_entry = gtk_entry_new();
	gtk_entry_set_icon_from_stock (GTK_ENTRY(search_entry), 
	                               GTK_ENTRY_ICON_SECONDARY, 
	                               GTK_STOCK_CLEAR);
	search_btn = gtk_button_new();
	image = gtk_image_new_from_stock (GTK_STOCK_FIND, GTK_ICON_SIZE_MENU);
	gtk_button_set_image(GTK_BUTTON(search_btn), image);
	gtk_box_pack_start (GTK_BOX(hbox3), search_entry, TRUE, TRUE, 2);
	gtk_box_pack_end (GTK_BOX(hbox3), search_btn, FALSE, FALSE, 4);
	
	notebook = gtk_notebook_new();
	image = gtk_image_new_from_file(RESDIR"images/contactTabButton1.png");
	gtk_widget_set_size_request(image, 79, 25);	

    GtkWidget *contact_scroll = gtk_scrolled_window_new(NULL, NULL);
	contactView = ic_contact_list_new(lwqq_client);
    gtk_container_add(GTK_CONTAINER(contact_scroll), contactView);
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), contact_scroll, image);

	image = gtk_image_new_from_file(RESDIR"images/groupTabButton1.png");
	gtk_widget_set_size_request(image, 79, 25);
	label = gtk_label_new("group");
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), label, image);

	image = gtk_image_new_from_file(RESDIR"images/recentTabButton1.png");
	gtk_widget_set_size_request(image, 79, 25);
	label = gtk_label_new("recent");
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), label, image);	

	toolbar = gtk_event_box_new();
	GdkColor color2 = {0, 52171, 57825, 61166};
	gtk_widget_modify_bg(toolbar, GTK_STATE_NORMAL, &color2);
	start_btn = ic_button_new ();
	GdkPixbuf *start_img = gdk_pixbuf_new_from_file_at_scale (RESDIR"images/head.png",
	                                                          35, 40, FALSE, NULL);
	gtk_widget_set_size_request (start_btn, 35, 40);
	ic_button_set_image (start_btn, start_img);
	image = gtk_image_new_from_file(RESDIR"images/tools.png");
	gtk_box_pack_start (GTK_BOX(hbox4), start_btn, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX(hbox4), image, FALSE, FALSE, 5);
	gtk_container_add(GTK_CONTAINER(toolbar), hbox4);
	
	gtk_box_pack_start(GTK_BOX(vbox1), hbox2, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox1), hbox3, FALSE, FALSE, 2);
	gtk_box_pack_start(GTK_BOX(vbox1), notebook, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(vbox1), toolbar, FALSE, FALSE, 0);

	gtk_widget_set_events(mainWindow, GDK_EXPOSURE_MASK | GDK_LEAVE_NOTIFY_MASK
			| GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK | GDK_POINTER_MOTION_MASK
			| GDK_POINTER_MOTION_HINT_MASK);
	g_signal_connect(G_OBJECT(mainWindow), "button_press_event",
			G_CALLBACK(on_mouse_press_event), NULL);
	g_signal_connect_swapped(G_OBJECT(title_bar), "ic-titlebar-destroy",
	       	G_CALLBACK(on_main_panel_close), mainWindow);
	
	gtk_container_add(GTK_CONTAINER(mainWindow), vbox1); 

	return mainWindow;
}

void ic_main_panel_show(GtkWidget *widget) {
	gtk_widget_show_all(widget);
}

void ic_main_panel_destroy(GtkWidget *widget) {
	gtk_widget_destroy(widget);
}

static void on_main_panel_close(GtkWidget *widget)
{
	gtk_widget_hide (widget);
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

