#include "ic_button.h"

#define DEFAULT_SIZE 16
#define IC_BUTTON_GET_PRIVATE(obj)	(G_TYPE_INSTANCE_GET_PRIVATE((obj), \
		ic_button_get_type(), IcButtonPrivate))

struct _IcButtonPrivate
{
	GdkPixbuf *normal_img;
	GdkPixbuf *hovered_img;
	GdkPixbuf *pressed_img;
	gboolean use_hovered_bg;
	gint btn_status;
};

G_DEFINE_TYPE(IcButton, ic_button, GTK_TYPE_BUTTON);

enum {
	PRESSED,
	RELEASED,
	CLICKED,
	ENTER,
	LEAVE,
	ACTIVATE,
	LAST_SIGNAL
};

static void ic_button_class_init(IcButtonClass *button_class);
static void ic_button_init(IcButton *button);
static void ic_button_size_allocate(GtkWidget *widget, GtkAllocation *allocation);

static gboolean ic_button_draw(GtkWidget *widget, cairo_t *cr);
static gboolean ic_button_enter(GtkWidget *widget, gpointer data);
static gboolean ic_button_leave(GtkWidget *widget, gpointer data);
static gboolean ic_button_pressed(GtkWidget *widget, gpointer data);
static gboolean ic_button_released(GtkWidget *widget, gpointer data);

static void ic_button_class_init(IcButtonClass *button_class)
{
	g_type_class_add_private(button_class, sizeof(IcButtonPrivate));
	GtkWidgetClass *object_class;
	object_class = (GtkWidgetClass*)button_class;
	object_class->draw = ic_button_draw;
	object_class->size_allocate = ic_button_size_allocate;
}

static void ic_button_init(IcButton *button)
{
	IcButtonPrivate *private;

	button->private = G_TYPE_INSTANCE_GET_PRIVATE(button,
			IC_TYPE_BUTTON,
			IcButtonPrivate);

	private = button->private;
	private->normal_img = NULL;
	private->hovered_img = NULL;
	private->pressed_img = NULL;
	private->use_hovered_bg = FALSE;
	private->btn_status = -1;

	gtk_widget_add_events(GTK_WIDGET(button), GDK_ENTER_NOTIFY_MASK |
                                              GDK_LEAVE_NOTIFY_MASK |
                                              GDK_BUTTON_PRESS_MASK |
                                              GDK_BUTTON_RELEASE_MASK);

	g_signal_connect(G_OBJECT(button), "enter-notify-event",
			G_CALLBACK(ic_button_enter), NULL);
	g_signal_connect(G_OBJECT(button), "leave-notify-event",
			G_CALLBACK(ic_button_leave), NULL);
	g_signal_connect(G_OBJECT(button), "button-press-event",
			G_CALLBACK(ic_button_pressed), NULL);
	g_signal_connect(G_OBJECT(button), "button-release-event",
			G_CALLBACK(ic_button_released), NULL);
}

GtkWidget *ic_button_new(void)
{
	return g_object_new(IC_TYPE_BUTTON, NULL);
}

GtkWidget *ic_button_new_with_pixbuf(GdkPixbuf *pixbuf)
{
	GtkWidget * widget = g_object_new(IC_TYPE_BUTTON, NULL);
    ic_button_set_image(widget, pixbuf);

    return widget;
}

void ic_button_set_image(GtkWidget *button, GdkPixbuf *pixbuf)
{
	IcButtonPrivate *private = NULL;

	private = IC_BUTTON(button)->private;
	private->normal_img = pixbuf;
	private->hovered_img = pixbuf;
	private->pressed_img = pixbuf;
}

void ic_button_set_normal_image(GtkWidget *button, GdkPixbuf *pixbuf)
{
    IcButtonPrivate *private = NULL;

    private = IC_BUTTON(button)->private;
    private->normal_img = pixbuf;
}

void ic_button_set_hovered_image(GtkWidget *button, GdkPixbuf *pixbuf)
{
	IcButtonPrivate *private = NULL;

	private = IC_BUTTON(button)->private;
	private->hovered_img = pixbuf;
	private->use_hovered_bg = FALSE;
}

void ic_button_set_pressed_image(GtkWidget *button, GdkPixbuf *pixbuf)
{
	IcButtonPrivate *private = NULL;

	private = IC_BUTTON(button)->private;
	private->pressed_img = pixbuf;
}

void ic_button_set_hovered_bg(GtkWidget *button, gboolean use_bg)
{
   IcButtonPrivate *private = NULL;

   private = IC_BUTTON(button)->private;
   private->use_hovered_bg = use_bg;
}

static void ic_button_size_allocate(GtkWidget *widget, GtkAllocation *allocation)
{
	GtkButton *button = GTK_BUTTON (widget);
	gtk_widget_set_allocation(widget, allocation);

	if (gtk_widget_get_realized (widget))
	    gdk_window_move_resize (gtk_button_get_event_window(button),
	                            allocation->x,
	                            allocation->y,
	                            allocation->width,
	                            allocation->height);
}

static gboolean ic_button_draw(GtkWidget *widget, cairo_t *cr)
{
	IcButtonPrivate *private = NULL;
	GtkAllocation alloc;
	gint x, y, width, height, r;

	private = IC_BUTTON(widget)->private;
	gtk_widget_get_allocation(widget, &alloc);
	x = alloc.x;
	y = alloc.y;
	width = alloc.width;
	height = alloc.height;
	r = 3;
	
	cr = gdk_cairo_create(gtk_widget_get_window(widget));
	if(private->btn_status != LEAVE && private->btn_status != -1 && private->use_hovered_bg)
	{
		cairo_set_line_width (cr, 1);
		cairo_move_to (cr, x+r, y);
		cairo_line_to (cr, x+width-r, y);
		cairo_move_to (cr, x+width, y+r);
		cairo_line_to (cr, x+width, y+height-r);
		cairo_move_to (cr, x+width-r, y+height);
		cairo_move_to (cr, x+r, y+height);
		cairo_move_to (cr, x, y+height-r);
		cairo_line_to (cr, x, y+r);
		cairo_arc (cr, x+r, y+r, r, G_PI, 3 * G_PI / 2.0 );
		cairo_arc (cr, x+width-r, y+r, r, 3 * G_PI /2.0, 2 * G_PI);
		cairo_arc (cr, x+width-r, y+height-r, r, 0, G_PI / 2);
		cairo_arc (cr, x+r, y+height-r, r, G_PI / 2, G_PI);
		cairo_set_source_rgba(cr, 1, 1, 1, 0.4);
		cairo_fill(cr);
	}

	switch(private->btn_status)
	{
	case PRESSED :
		gdk_cairo_set_source_pixbuf(cr, private->pressed_img, alloc.x, alloc.y);
		cairo_paint(cr);
		break;
	case RELEASED :
		gdk_cairo_set_source_pixbuf(cr, private->hovered_img, alloc.x, alloc.y);
		cairo_paint(cr);
		break;
	case ENTER :
		gdk_cairo_set_source_pixbuf(cr, private->hovered_img, alloc.x, alloc.y);
		cairo_paint(cr);
		break;
	case LEAVE :
		gdk_cairo_set_source_pixbuf(cr, private->normal_img, alloc.x, alloc.y);
		cairo_paint(cr);
		break;
	default :
		gdk_cairo_set_source_pixbuf(cr, private->normal_img, alloc.x, alloc.y);
		cairo_paint(cr);
		break;
	}

	cairo_destroy(cr);
	return TRUE;
}

static gboolean ic_button_enter(GtkWidget *widget, gpointer data)
{
	IcButton *button = IC_BUTTON(widget);
	button->private->btn_status = ENTER;
	ic_button_draw(widget, NULL);
	return FALSE;
}

static gboolean ic_button_leave(GtkWidget *widget, gpointer data)
{
	IcButton *button = IC_BUTTON(widget);
	button->private->btn_status = LEAVE;
	ic_button_draw(widget, NULL);
	return FALSE;
}

static gboolean ic_button_pressed(GtkWidget *widget, gpointer data)
{
	IcButton *button = IC_BUTTON(widget);
	button->private->btn_status = PRESSED;
	ic_button_draw(widget, NULL);
	return FALSE;
}

static gboolean ic_button_released(GtkWidget *widget, gpointer data)
{
	IcButton *button = IC_BUTTON(widget);
	button->private->btn_status = RELEASED;
	ic_button_draw(widget, NULL);
	return FALSE;
}

