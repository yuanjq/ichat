#include "ic_status_box.h"

typedef struct _IcStatusBoxPriv IcStatusBoxPriv;
struct _IcStatusBoxPriv
{
	GtkWidget *popup_menu;
	gint status;
};

enum
{
	IC_STATUS_CHANGED_SIGNAL,
	IC_STATUS_BOX_SIGNAL_N
};

enum{
	COL_PIXBUF=0,
    COL_STRING,
    COLS_N
};

static int ic_status_box_signals[IC_STATUS_BOX_SIGNAL_N] = {0};
static const gchar *status[] = {"在线", "Q我吧", "隐身", 
								"忙碌", "离开", "离线"};
static const gchar *files[] = { RESDIR"images/status/online.png",
				RESDIR"images/status/callme.png",
				RESDIR"images/status/hidden.png" ,
				RESDIR"images/status/busy.png",
				RESDIR"images/status/away.png",
				RESDIR"images/status/offline.png"};

static void ic_status_box_class_init(IcStatusBoxClass *class);
static void ic_status_box_init(IcStatusBox *status_box);
static void ic_status_box_popup(GtkWidget *widget, GdkEvent *event, gpointer data);
static void ic_status_box_get_position(GtkMenu *menu, gint *x, gint *y, 
                                  gboolean *push_in, gpointer data);
static gboolean ic_status_box_draw(GtkWidget *widget, cairo_t *cr, gpointer data);
static void ic_menu_item_activate(GtkWidget *widget, gpointer data);

G_DEFINE_TYPE(IcStatusBox, ic_status_box, GTK_TYPE_BUTTON);

static void ic_status_box_class_init(IcStatusBoxClass *class)
{
	g_type_class_add_private (class, sizeof(IcStatusBoxPriv));

	GtkWidgetClass *object_class = (GtkWidgetClass *)class;
	ic_status_box_signals[IC_STATUS_CHANGED_SIGNAL]
		= g_signal_new ("ic-status-changed",
		                G_TYPE_FROM_CLASS(object_class),
		                G_SIGNAL_RUN_FIRST,
		                0,
		                NULL, NULL,
						g_cclosure_marshal_VOID__VOID,
						G_TYPE_NONE, 0);	
}

static void ic_status_box_init(IcStatusBox *self)
{
	IcStatusBoxPriv *priv = G_TYPE_INSTANCE_GET_PRIVATE(self, IC_TYPE_STATUS_BOX,     
	                                                   IcStatusBoxPriv);
	gtk_widget_set_size_request(GTK_WIDGET(self), 35, 15);	

	GtkWidget *image;
	GtkWidget *menu_item;
	gint i;
	
	priv->popup_menu = gtk_menu_new ();
	for(i=0; i<6; i++)
	{
		menu_item = gtk_image_menu_item_new_with_label (status[i]);
		gint *position = (gint *)g_malloc(sizeof(gint));
		*position = i;
		g_object_set_data (G_OBJECT(menu_item), "position", (gpointer)position);

		image = gtk_image_new_from_file (files[i]);
		gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM(menu_item), image);
		gtk_image_menu_item_set_always_show_image (GTK_IMAGE_MENU_ITEM(menu_item), TRUE);
		gtk_menu_shell_append (GTK_MENU_SHELL(priv->popup_menu), menu_item);
		
		g_signal_connect(G_OBJECT(menu_item), "activate",
		                 G_CALLBACK(ic_menu_item_activate), self);
	}
	gtk_widget_show_all (priv->popup_menu);

	g_signal_connect(G_OBJECT(self), "draw", G_CALLBACK(ic_status_box_draw), NULL);
	g_signal_connect(G_OBJECT(self), "button-press-event", 
	                 G_CALLBACK(ic_status_box_popup), NULL);
	
}

GtkWidget *ic_status_box_new()
{
	return g_object_new(IC_TYPE_STATUS_BOX, NULL);
}


static void ic_status_box_popup(GtkWidget *widget, GdkEvent *event, gpointer data)
{
	IcStatusBoxPriv *priv = G_TYPE_INSTANCE_GET_PRIVATE(widget, 
	                                                   IC_TYPE_STATUS_BOX,
	                                                   IcStatusBoxPriv);
	GdkEventButton *event_button = (GdkEventButton *)event;
	gtk_menu_popup(GTK_MENU(priv->popup_menu), NULL, NULL, ic_status_box_get_position, 
	               widget, event_button->button, event_button->time);
}

static void ic_status_box_get_position(GtkMenu *menu, gint *x, gint *y, 
                                  gboolean *push_in, gpointer data)
{
	GtkAllocation alloc;
	gint root_x, root_y;
	
	GtkWidget *widget = GTK_WIDGET(data);
	gtk_widget_get_allocation (widget, &alloc);
	gdk_window_get_origin (gtk_widget_get_window (widget), &root_x, &root_y);
	*x = root_x + alloc.x;
	*y = root_y + alloc.y;
}

static gboolean ic_status_box_draw(GtkWidget *widget, cairo_t *cr, gpointer data)
{
	IcStatusBoxPriv *priv = G_TYPE_INSTANCE_GET_PRIVATE(widget, 
	                                                   IC_TYPE_STATUS_BOX,
	                                                   IcStatusBoxPriv);
	cr = gdk_cairo_create(gtk_widget_get_window(widget));
	GdkPixbuf *arrow = gdk_pixbuf_new_from_file_at_scale(RESDIR"images/status/downarrow.png", 
	                                                     10, 7, TRUE, NULL);	
	GdkPixbuf *status = gdk_pixbuf_new_from_file_at_scale(files[priv->status], 
	                                                      12, 12, TRUE, NULL);
	
	gint arrow_w, status_w, status_h;
	GtkAllocation alloc;
	
	gtk_widget_get_allocation(widget, &alloc);
	status_w = gdk_pixbuf_get_width(status);
	status_h = gdk_pixbuf_get_height(status);
	arrow_w = gdk_pixbuf_get_width(arrow);

	gint alignx, aligny;
	alignx = (alloc.width-status_w-arrow_w) / 2 - 3;
	aligny = (alloc.height-status_h) / 2;
	
	gdk_cairo_set_source_pixbuf(cr, status, alloc.x+alignx, alloc.y+aligny);
	cairo_paint(cr);
	
	alignx = alloc.width-arrow_w-alignx - 3;
	aligny += 4;
	gdk_cairo_set_source_pixbuf(cr, arrow, alloc.x+alignx, alloc.y+aligny);
	cairo_paint(cr);
	cairo_destroy (cr);

	g_object_unref (arrow);
	g_object_unref (status);

	return TRUE;
}

static void ic_menu_item_activate(GtkWidget *widget, gpointer data)
{
	IcStatusBoxPriv *priv = G_TYPE_INSTANCE_GET_PRIVATE(data, 
	                                                   IC_TYPE_STATUS_BOX,
	                                                   IcStatusBoxPriv);
												   
	gint *position = (gint *)g_object_get_data (G_OBJECT(widget), "position");
	priv->status = *position;
	g_signal_emit (data, ic_status_box_signals[IC_STATUS_CHANGED_SIGNAL], 0);
}

gint ic_status_box_get_status(IcStatusBox *self)
{
	IcStatusBoxPriv *priv = G_TYPE_INSTANCE_GET_PRIVATE(self, IC_TYPE_STATUS_BOX,
	                                                    IcStatusBoxPriv);
	return priv->status;
}
