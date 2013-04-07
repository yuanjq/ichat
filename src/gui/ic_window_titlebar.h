#ifndef IC_WINDOW_TITLEBAR_H
#define IC_WINDOW_TITLEBAR_H

#include <gtk/gtk.h>
#include "ic_titlebar.h"

#define IC_TYPE_WINDOW_TITLEBAR (ic_window_titlebar_get_type())
#define IC_WINDOW_TITLEBAR(obj) (G_TYPE_CHECK_INSTANCE_CAST(obj, IC_TYPE_WINDOW_TITLEBAR, IcWindowTitlebar))

typedef struct _IcWindowTitlebar IcWindowTitlebar;
typedef struct _IcWindowTitlebarClass IcWindowTitlebarClass;
typedef struct _IcWindowTitlebarPriv IcWindowTitlebarPriv;

struct _IcWindowTitlebar {
    IcTitlebar titlebar;
	GtkWidget *menu_btn;
	GtkWidget *signature;
};

struct _IcWindowTitlebarClass {
	IcTitlebarClass parent_class;
};

GType ic_window_titlebar_get_type(void);
GtkWidget *ic_window_titlebar_new(void);
void ic_window_titlebar_set_label(GtkWidget *widget, gchar *label);
void ic_window_titlebar_set_signatrue(GtkWidget *widget, gchar *label);
GtkWidget *ic_window_titlebar_new_with_label(gchar *label);

#endif
