#include "ic_input_toolbar.h"
#include "ic_button.h"

GtkWidget *ic_input_toolbar_new()
{
	GtkWidget *input_toolbar;
	GtkWidget *hbox;
	GtkWidget *typeface_btn;
	GtkWidget *face_btn;
	GtkWidget *flash_btn;
	GtkWidget *shake_btn;
	GtkWidget *handwriting_btn;
	GtkWidget *pic_btn;
	GtkWidget *more_btn;
	GtkWidget *record_btn;
	GdkPixbuf *pixbuf;

	hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	input_toolbar = gtk_event_box_new();
	gtk_container_add(GTK_CONTAINER(input_toolbar), hbox);

	typeface_btn = ic_button_new();
	gtk_widget_set_size_request(typeface_btn, 22, 22);
	pixbuf = gdk_pixbuf_new_from_file_at_scale(RESDIR"images/typeface.png", 22, 22, TRUE, NULL);
	ic_button_set_image(GTK_WIDGET(typeface_btn), pixbuf);

	face_btn = ic_button_new();
	gtk_widget_set_size_request(face_btn, 22, 22);
	pixbuf = gdk_pixbuf_new_from_file_at_scale(RESDIR"images/face.png", 22, 22, TRUE, NULL);
	ic_button_set_image(GTK_WIDGET(face_btn), pixbuf);

	flash_btn = ic_button_new();
	gtk_widget_set_size_request(flash_btn, 22, 22);
	pixbuf = gdk_pixbuf_new_from_file_at_scale(RESDIR"images/flash.png", 22, 22, TRUE, NULL);
	ic_button_set_image(GTK_WIDGET(flash_btn), pixbuf);

	shake_btn = ic_button_new();
	gtk_widget_set_size_request(shake_btn, 22, 22);
	pixbuf = gdk_pixbuf_new_from_file_at_scale(RESDIR"images/shake.png", 22, 22, TRUE, NULL);
	ic_button_set_image(GTK_WIDGET(shake_btn), pixbuf);

	handwriting_btn = ic_button_new();
	gtk_widget_set_size_request(handwriting_btn, 22, 22);
	pixbuf = gdk_pixbuf_new_from_file_at_scale(RESDIR"images/handwriting.png", 22, 22, TRUE, NULL);
	ic_button_set_image(GTK_WIDGET(handwriting_btn), pixbuf);

	pic_btn = ic_button_new();
	gtk_widget_set_size_request(pic_btn, 22, 22);
	pixbuf = gdk_pixbuf_new_from_file_at_scale(RESDIR"images/pic.png", 22, 22, TRUE, NULL);
	ic_button_set_image(GTK_WIDGET(pic_btn), pixbuf);

	more_btn = ic_button_new();
	gtk_widget_set_size_request(more_btn, 22, 22);
	pixbuf = gdk_pixbuf_new_from_file_at_scale(RESDIR"images/more.png", 22, 22, TRUE, NULL);
	ic_button_set_image(GTK_WIDGET(more_btn), pixbuf);

	record_btn = ic_button_new();
	gtk_widget_set_size_request(record_btn, 22, 22);
	pixbuf = gdk_pixbuf_new_from_file_at_scale(RESDIR"images/record.png", 22, 22, TRUE, NULL);
	ic_button_set_image(GTK_WIDGET(record_btn), pixbuf);

	gtk_box_pack_start(GTK_BOX(hbox), typeface_btn, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(hbox), face_btn, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(hbox), flash_btn, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(hbox), shake_btn, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(hbox), handwriting_btn, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(hbox), pic_btn, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(hbox), more_btn, FALSE, FALSE, 0);
	gtk_box_pack_end(GTK_BOX(hbox), record_btn, FALSE, FALSE, 0);

	return input_toolbar;
}
