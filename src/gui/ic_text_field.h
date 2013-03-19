#ifndef IC_TEXT_FIELD_H
#define IC_TEXT_FIELD_H

#include <gtk/gtk.h>

#define IC_TYPE_TEXT_FIELD			(ic_text_field_get_type())
#define IC_TEXT_FIELD(obj)			(G_TYPE_CHECK_INSTANCE_CAST((obj), IC_TYPE_TEXT_FIELD, IcTextField))
#define IC_TEXT_FIELD_CLASS(class)	(G_TYPE_CHECK_CLASS_CAST((class), IC_TYPE_TEXT_FIELD, IcTextFieldClass))

typedef struct _IcTextField 		IcTextField;
typedef struct _IcTextFieldClass 	IcTextFieldClass;

struct _IcTextField
{
	GtkVBox parent;
};

struct _IcTextFieldClass
{
	GtkVBoxClass parent_class;
};

GType ic_text_field_get_type();
GtkWidget *ic_text_field_new();
void ic_text_field_set_text(IcTextField *textfield, gchar *text);

#endif
