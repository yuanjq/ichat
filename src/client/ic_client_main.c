#include <gtk/gtk.h>
#include "ic_client_context.h"
#include "ic_tray_icon.h"

int main(int argc, char *argv[]) {
	if(!g_thread_supported())
	  g_thread_init(NULL);
	gdk_threads_init();
	
	gtk_init(&argc, &argv);
	
	ic_client_init();

	gdk_threads_enter();
	gtk_main();
	gdk_threads_leave();
	return 0;
}
