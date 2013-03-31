#ifndef IC_TRAY_ICON_H
#define IC_TRAY_ICON_H

#define IC_TYPE_TRAY_ICON 			(ic_tray_icon_get_type())
#define IC_TRAY_ICON(obj)			(G_TYPE_CHECK_INSTANCE_CAST(obj, \
									IC_TYPE_TRAY_ICON, IcTrayIcon))
#define IC_TRAY_ICON_CLASS(class)	(G_TYPE_CHECK_CLASS_CAST(class, \
									IC_TYPE_TRAY_ICON, IcTrayIconClass))

#include <gtk/gtk.h>

typedef struct _IcTrayIcon IcTrayIcon;
typedef struct _IcTrayIconClass IcTrayIconClass;
typedef struct _IcTrayIconPriv IcTrayIconPriv;

struct _IcTrayIcon
{
	GtkStatusIcon parent;
};

struct _IcTrayIconClass
{
	GtkStatusIconClass parent_class;
};

GType ic_tray_icon_get_type();
GtkStatusIcon *ic_tray_icon_new();
void ic_tray_icon_set_blinking(IcTrayIcon *tray_icon);

#endif
