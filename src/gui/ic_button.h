#ifndef IC_BUTTON_H
#define IC_BUTTON_H

#include <gtk/gtk.h>

#define IC_TYPE_BUTTON			(ic_button_get_type())
#define IC_BUTTON(obj)			(G_TYPE_CHECK_INSTANCE_CAST((obj), IC_TYPE_BUTTON, IcButton))
#define IC_BUTTON_CLASS(class)	(G_TYPE_CHECK_CLASS_CAST((class), IC_TYPE_BUTTON, IcButtonClass))
#define IC_IS_BUTTON(obj)		(G_TYPE_CHECK_INSTANCE_TYPE((obj), IC_TYPE_BUTTON))
#define IC_IS_BUTTON_CLASS(class)(G_TYPE_CHECK_CLASS_TYPE((class), IC_TYPE_BUTTON))
#define IC_BUTTON_GET_CLASS(obj)	(G_TYPE_INSTANCE_GET_CLASS((obj), IC_TYPE_BUTTON, IcButtonClass))

typedef struct _IcButton 		IcButton;
typedef struct _IcButtonClass 	IcButtonClass;
typedef struct _IcButtonPrivate IcButtonPrivate;

struct _IcButton {
	GtkButton button;

	IcButtonPrivate *private;
};

struct _IcButtonClass {
	GtkButtonClass button_class;
};

GType ic_button_get_type(void);
GtkWidget *ic_button_new(void);
GtkWidget *ic_button_new_with_pixbuf(GdkPixbuf *pixbuf);
void ic_button_set_image(GtkWidget *button, GdkPixbuf *pixbuf);
void ic_button_set_normal_image(GtkWidget *button, GdkPixbuf *pixbuf);
void ic_button_set_hovered_image(GtkWidget *button, GdkPixbuf *pixbuf);
void ic_button_set_pressed_image(GtkWidget *button, GdkPixbuf *pixbuf);
void ic_button_set_hovered_bg(GtkWidget *button, gboolean use_bg);

#endif
