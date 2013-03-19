#include "ic_toolbar.h"
#include "ic_button.h"

GtkWidget *ic_tool_bar_new() {
	GtkWidget *tool_bar;
	GtkWidget *video_button;
	GtkWidget *voice_button;
	GtkWidget *send_file_button;
	GtkWidget *create_group_button;
	GdkPixbuf *pixbuf;

	tool_bar = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

	video_button = ic_button_new();
	gtk_widget_set_size_request(video_button, 35, 35);
	pixbuf = gdk_pixbuf_new_from_file_at_scale(RESDIR"images/video.png", 35, 35, TRUE, NULL);
	ic_button_set_image(video_button, pixbuf);

	voice_button = ic_button_new();
	gtk_widget_set_size_request(voice_button, 35, 35);
	pixbuf = gdk_pixbuf_new_from_file_at_scale(RESDIR"images/voice.png", 35, 35, TRUE, NULL);
	ic_button_set_image(voice_button, pixbuf);

	send_file_button = ic_button_new();
	gtk_widget_set_size_request(send_file_button, 35, 35);
	pixbuf = gdk_pixbuf_new_from_file_at_scale(RESDIR"images/sendfile.png", 35, 35, TRUE, NULL);
	ic_button_set_image(send_file_button, pixbuf);

	create_group_button = ic_button_new();
	gtk_widget_set_size_request(create_group_button, 35, 35);
	pixbuf = gdk_pixbuf_new_from_file_at_scale(RESDIR"images/creategroup.png", 35, 35, TRUE, NULL);
	ic_button_set_image(create_group_button, pixbuf);

	gtk_box_pack_start(GTK_BOX(tool_bar), video_button, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(tool_bar), voice_button, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(tool_bar), send_file_button, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(tool_bar), create_group_button, FALSE, FALSE, 0);

	return tool_bar;
}
