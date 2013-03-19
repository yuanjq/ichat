#include "ic_text_field.h"

typedef struct _IcTextFieldPriv IcTextFieldPriv;

struct _IcTextFieldPriv
{
	GtkWidget *button;
	GtkWidget *label;
	GtkWidget *entry;
};

static void ic_text_field_show_all(GtkWidget *widget);
static void on_button_clicked(GtkWidget *widget);
static void on_entry_focus_out_event(GtkWidget *widget);
static void on_entry_activate(GtkWidget *widget);
static gboolean on_entry_key_pressed(GtkWidget *widget, GdkEventKey *event);

G_DEFINE_TYPE(IcTextField, ic_text_field, GTK_TYPE_VBOX);

static void ic_text_field_class_init(IcTextFieldClass *textfield_class)
{
	g_type_class_add_private (textfield_class, sizeof(IcTextFieldPriv));

	GtkWidgetClass *widget_class;
	widget_class = (GtkWidgetClass *)textfield_class;
	widget_class->show_all = ic_text_field_show_all;
}

static void ic_text_field_init(IcTextField *textfield)
{
	IcTextFieldPriv *priv = G_TYPE_INSTANCE_GET_PRIVATE(textfield, IC_TYPE_TEXT_FIELD,
	                                                    IcTextFieldPriv);
	priv->button = gtk_button_new();
	gtk_button_set_relief (GTK_BUTTON(priv->button), GTK_RELIEF_NONE);
	gtk_button_set_focus_on_click (GTK_BUTTON(priv->button), FALSE);
	gtk_button_set_alignment (GTK_BUTTON(priv->button), 0.0, 0.5);
	gtk_widget_set_size_request(priv->button, -1, 22);
	g_signal_connect_swapped(G_OBJECT(priv->button), "clicked", 
	                 G_CALLBACK(on_button_clicked), textfield);

	priv->label = gtk_label_new("set you signature...");
	gtk_label_set_justify(GTK_LABEL(priv->label), GTK_JUSTIFY_LEFT);
	gtk_container_add(GTK_CONTAINER(priv->button), priv->label);

	priv->entry = gtk_entry_new ();
	gtk_entry_set_max_length(GTK_ENTRY(priv->entry), 100);
	g_signal_connect_swapped(G_OBJECT(priv->entry), "activate",
	                         G_CALLBACK(on_entry_activate), textfield);
	g_signal_connect_swapped(G_OBJECT(priv->entry), "focus-out-event", 
	                         G_CALLBACK(on_entry_focus_out_event), textfield);
	g_signal_connect_swapped(G_OBJECT(priv->entry), "key-press-event",
	                         G_CALLBACK(on_entry_key_pressed), textfield);
	
	gtk_box_pack_start (GTK_BOX(textfield), priv->button, TRUE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX(textfield), priv->entry, TRUE, FALSE, 0);
}

GtkWidget *ic_text_field_new()
{
	return GTK_WIDGET(g_object_new (IC_TYPE_TEXT_FIELD, NULL));
}

static void ic_text_field_show_all(GtkWidget *textfield)
{
	IcTextFieldPriv *priv = G_TYPE_INSTANCE_GET_PRIVATE(textfield, IC_TYPE_TEXT_FIELD,
	                                                    IcTextFieldPriv);
	gtk_widget_show (priv->button);
	gtk_widget_show (priv->label);
	gtk_widget_hide (priv->entry);
	gtk_widget_show (textfield);
}

void ic_text_field_set_text(IcTextField *textfield, gchar *text)
{
	IcTextFieldPriv *priv = G_TYPE_INSTANCE_GET_PRIVATE(textfield, IC_TYPE_TEXT_FIELD,
	                                                    IcTextFieldPriv);
	gtk_label_set_text(GTK_LABEL(priv->label), text);
}

static void on_button_clicked(GtkWidget *widget)
{
	IcTextFieldPriv *priv = G_TYPE_INSTANCE_GET_PRIVATE(widget, IC_TYPE_TEXT_FIELD,
	                                                    IcTextFieldPriv);
	gchar *text = gtk_label_get_text (GTK_LABEL(priv->label));
	
	gtk_widget_hide (priv->button);
	gtk_widget_show (priv->entry);
	gtk_widget_grab_focus (priv->entry);
}

static void on_entry_focus_out_event(GtkWidget *widget)
{
	IcTextFieldPriv *priv = G_TYPE_INSTANCE_GET_PRIVATE(widget, IC_TYPE_TEXT_FIELD,
	                                                    IcTextFieldPriv);
	gtk_widget_hide (priv->entry);
	gtk_widget_show (priv->button);
}

static void on_entry_activate(GtkWidget *widget)
{
	IcTextFieldPriv *priv = G_TYPE_INSTANCE_GET_PRIVATE(widget, IC_TYPE_TEXT_FIELD,
	                                                    IcTextFieldPriv);
	gtk_widget_hide (priv->entry);
	gtk_widget_show (priv->button);
	
}

static gboolean on_entry_key_pressed(GtkWidget *widget, GdkEventKey *event)
{
	if(event->keyval == GDK_KEY_Escape)
	{
		IcTextFieldPriv *priv = G_TYPE_INSTANCE_GET_PRIVATE(widget, IC_TYPE_TEXT_FIELD,
	                                                    IcTextFieldPriv);
		gtk_widget_hide (priv->entry);
		gtk_widget_show (priv->button);
		return TRUE;
	}

	return FALSE;
}
