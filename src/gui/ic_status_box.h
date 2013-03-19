#ifndef IC_STATUS_BOX_H
#define IC_STATUS_BOX_H

#include <gtk/gtk.h>

#define IC_TYPE_STATUS_BOX 			(ic_status_box_get_type())
#define IC_STATUS_BOX(obj)			(G_TYPE_CHECK_INSTANCE_CAST((obj), IC_TYPE_STATUS_BOX, IcStatusBox))
#define IC_STATUS_BOX_CLASS(class)	(G_TYPE_CHECK_CLASS_CAST((class), IC_TYPE_STATUS_BOX, IcStatusBoxClass))

typedef struct _IcStatusBox 		IcStatusBox;
typedef struct _IcStatusBoxClass 	IcStatusBoxClass;

struct _IcStatusBox
{
	GtkButton parent;
};

struct _IcStatusBoxClass
{
	GtkButtonClass parent_class;
};

GType ic_status_box_get_type();
GtkWidget *ic_status_box_new();
gint ic_status_box_get_status(IcStatusBox *self);

#endif