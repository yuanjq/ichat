#ifndef IC_TITLEBAR_H
#define IC_TITLEBAR_H

#include <gtk/gtk.h>

#define IC_TYPE_TITLEBAR			(ic_titlebar_get_type())
#define IC_TITLEBAR(obj)			(G_TYPE_CHECK_INSTANCE_CAST((obj), IC_TYPE_TITLEBAR, IcTitlebar))
#define IC_TITLEBAR_CLASS(class)	(G_TYPE_CHECK_CLASS_CAST((class), IC_TYPE_TITLEBAR, IcTitlebarClass))

typedef struct _IcTitlebar 		IcTitlebar;
typedef struct _IcTitlebarClass IcTitlebarClass;

struct _IcTitlebar
{
	GtkHBox parent;
    GtkWidget *label;
	GtkWidget *min_btn;
	GtkWidget *max_btn;
	GtkWidget *close_btn;
};

struct _IcTitlebarClass
{
	GtkHBoxClass parent_class;
};

GType ic_titlebar_get_type();
GtkWidget *ic_titlebar_new();
GtkWidget *ic_titlebar_new_with_label(gchar *label);
void ic_titlebar_set_label(GtkWidget *widget, gchar *label);

#endif
