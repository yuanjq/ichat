#include <gtk/gtk.h>
#include <string.h>
#include "ic_titlebar.h"
#include "ic_login_window.h"
#include "ic_client_context.h"
#include "ic_status_box.h"
#include "ic_data_type.h"

static GtkWidget *login_window;
static GtkWidget *user_entry, *pwd_entry;
static GtkWidget *statusCombo;
static void login_button_clicked(GtkWidget *widget, gpointer data);
static gboolean mouse_button_press_event (GtkWidget* widget, 
                                          GdkEventButton * event, 
                                          gpointer data);

GtkWidget *ic_login_window_new() {
	GtkWidget *title_bar;
	GtkWidget *user_label, *pwd_label;
	GtkWidget *loginButton, *registButton, *setupButton;
	GtkWidget *hbox1, *hbox2, *hbox3, *hbox4, *hbox5;
	GtkWidget *vbox, *vbox1;
	GtkWidget *frame1;
	GtkWidget *image1, *image2;
	GtkWidget *pwdCheckBox;
	GtkWidget *hButtonBox1, *hButtonBox2;

	GdkScreen *screen;
	GdkVisual *visual;

	GtkCssProvider *provider;
	GdkDisplay *display;

	login_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_widget_set_name (login_window, "login_window");
	gtk_window_set_decorated (GTK_WINDOW(login_window), FALSE);
	gtk_window_set_keep_above (GTK_WINDOW(login_window), TRUE);
	
	gtk_widget_set_size_request(login_window, 330, 240);
	gtk_window_set_resizable(GTK_WINDOW(login_window), FALSE);
	gtk_window_set_position(GTK_WINDOW(login_window), GTK_WIN_POS_CENTER);
	
	hbox1 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	hbox2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	hbox3 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	hbox4 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	hbox5 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	vbox1 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
	
	title_bar = ic_titlebar_new();
	gtk_box_pack_start (GTK_BOX(vbox), title_bar, FALSE, FALSE, 0);

	image1 = gtk_image_new_from_file(RESDIR"images/ichat.png");
	image2 = gtk_image_new_from_file(RESDIR"images/head.png");
	frame1 = gtk_frame_new(NULL);
	gtk_frame_set_shadow_type(GTK_FRAME(frame1), GTK_SHADOW_IN);
	gtk_widget_set_size_request(frame1, 80, 80);
	
	gtk_container_add(GTK_CONTAINER(frame1), image2);
	gtk_container_set_border_width(GTK_CONTAINER(hbox4), 5);
	
	user_label = gtk_label_new("用户名:");
	user_entry = gtk_entry_new();
	gtk_entry_set_text(GTK_ENTRY(user_entry), "nixingfeng");
	
	pwd_label = gtk_label_new("密  码:");
	pwd_entry = gtk_entry_new();
	gtk_entry_set_text(GTK_ENTRY(pwd_entry), "1234");
	gtk_entry_set_visibility(GTK_ENTRY(pwd_entry), FALSE);

	statusCombo = ic_status_box_new();
	pwdCheckBox = gtk_check_button_new_with_label("记住密码");
		
	loginButton = gtk_button_new_with_label("登录");
	registButton = gtk_button_new_with_label("注册");	
	setupButton = gtk_button_new_with_label("设置");
	
	gtk_box_pack_start (GTK_BOX(hbox5), statusCombo, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX(hbox5), pwdCheckBox, FALSE, FALSE, 0);

	hButtonBox1 = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
	gtk_button_box_set_layout(GTK_BUTTON_BOX(hButtonBox1), GTK_BUTTONBOX_START);
	gtk_container_add(GTK_CONTAINER(hButtonBox1), registButton);
	gtk_container_add(GTK_CONTAINER(hButtonBox1), setupButton);
	
	hButtonBox2 = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
	gtk_button_box_set_layout(GTK_BUTTON_BOX(hButtonBox2), GTK_BUTTONBOX_END);
	gtk_container_add(GTK_CONTAINER(hButtonBox2), loginButton);
	
	g_signal_connect(G_OBJECT(loginButton), "clicked",
			G_CALLBACK(login_button_clicked), NULL);
	g_signal_connect(G_OBJECT(title_bar), "ic-titlebar-destroy",
			G_CALLBACK(gtk_main_quit), NULL);	

	gtk_box_pack_start(GTK_BOX(hbox1), user_label, TRUE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(hbox1), user_entry, TRUE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(hbox2), pwd_label, TRUE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(hbox2), pwd_entry, TRUE, FALSE, 0);

	gtk_box_pack_start(GTK_BOX(vbox1), hbox1, TRUE, TRUE, 2);
	gtk_box_pack_start(GTK_BOX(vbox1), hbox2, TRUE, TRUE, 2);
	gtk_box_pack_start(GTK_BOX(vbox1), hbox5, FALSE, FALSE, 2);

	gtk_box_pack_start(GTK_BOX(hbox3), frame1, TRUE, FALSE, 2);
	gtk_box_pack_start(GTK_BOX(hbox3), vbox1, TRUE, FALSE, 0);
	
	gtk_box_pack_start(GTK_BOX(hbox4), hButtonBox1, FALSE, FALSE, 0);
	gtk_box_pack_end(GTK_BOX(hbox4), hButtonBox2, FALSE, FALSE, 0);
			
	gtk_box_pack_start(GTK_BOX(vbox), image1, FALSE, FALSE, 3);
	gtk_box_pack_start(GTK_BOX(vbox), hbox3, FALSE, FALSE, 3);
	gtk_box_pack_start(GTK_BOX(vbox), hbox4, FALSE, FALSE, 0);

	gtk_widget_set_events(login_window, GDK_EXPOSURE_MASK | GDK_LEAVE_NOTIFY_MASK
			| GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK | GDK_POINTER_MOTION_MASK
			| GDK_POINTER_MOTION_HINT_MASK);
	g_signal_connect(G_OBJECT(login_window), "button-press-event",
			G_CALLBACK(mouse_button_press_event), login_window);
	
	screen = gtk_widget_get_screen(login_window);
	visual = gdk_screen_get_rgba_visual(screen);
	gtk_widget_set_visual(login_window, visual);
	
	provider = gtk_css_provider_new ();
	display = gdk_display_get_default ();
	screen = gdk_display_get_default_screen (display);
	gtk_style_context_add_provider_for_screen(screen,
				                      GTK_STYLE_PROVIDER(provider),
				                      GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

	gtk_css_provider_load_from_data(GTK_CSS_PROVIDER(provider), 
						"GtkWindow {"
						"border-radius: 5;"									    
						"}"	
						"#login_window {"
						"background-image: url('"
						RESDIR"images/background.png');" 
						, -1, NULL);	

	gtk_container_add(GTK_CONTAINER(login_window), vbox);

	return login_window;
}

static void login_button_clicked(GtkWidget *widget, gpointer data) {
	const gchar *name, *pwd;

	name = gtk_entry_get_text(GTK_ENTRY(user_entry));
	pwd = gtk_entry_get_text(GTK_ENTRY(pwd_entry));

	ic_user_login(name, pwd);
}

void ic_login_window_dialog_show(GtkWidget *window, gint error_type) {
    char error_msg[200] = {0};

    switch(error_type)
    {
        case ERROR_TYPE_CONN_TIMEOUT:
            strcpy(error_msg, "网络连接超时，请检查网络连接!"); 
            break;
        case 2:
            strcpy(error_msg, "用户名或密码错误，请重新输入!");
            break;
        default:
            strcpy(error_msg, "登陆失败， 请重试！");
            break;
    }
    
	GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(window), GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_WARNING, GTK_BUTTONS_CLOSE, error_msg);
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
}

void ic_login_window_show(GtkWidget *window) {
	gtk_widget_show_all(window);
}

void ic_login_window_destroy(GtkWidget *window) {
	gtk_widget_destroy(window);
}

void ic_login_window_hide(GtkWidget *window) {
	gtk_widget_hide(window);
}

static gboolean mouse_button_press_event (GtkWidget* widget, GdkEventButton * event, 
                                          gpointer data)
{
	if (event->type == GDK_BUTTON_PRESS)
	{
		if (event->button == 1) {
			gtk_window_begin_move_drag(GTK_WINDOW(gtk_widget_get_toplevel(widget)),
				event->button,
				event->x_root,
				event->y_root,
				event->time);
		}
	}
	return FALSE;
}
