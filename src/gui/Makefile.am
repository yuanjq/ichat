AM_CFLAGS = \
	-DRESDIR=\"$(pkgdatadir)/resources/\" \
	-I../common \
	-I../tools \
	$(GTK3_CFLAGS) \
	$(XML2_CFLAGS) 

AM_LDFLAGS = \
	$(GTK3_LIBS) \
	$(XML2_LIBS) 

noinst_LTLIBRARIES = libgui.la
libgui_la_LIBADD = ../tools/libxml_parser.la

libgui_la_SOURCES = ic_chat_manager.c 	\
				ic_button.c				\
				ic_button.h				\
				ic_chat_manager.c		\
				ic_chat_manager.h		\
				ic_chat_window.c		\
				ic_chat_window.h		\
				ic_chatwindow_titlebar.c\
				ic_chatwindow_titlebar.h\
				ic_client_context.c		\
				ic_client_context.h		\
				ic_input_toolbar.c		\
				ic_input_toolbar.h		\
				ic_loading_panel.c		\
				ic_loading_panel.h		\
				ic_login_window.c		\
				ic_login_window.h		\
				ic_main_panel.c			\
				ic_main_panel.h			\
				ic_status_box.c			\
				ic_status_box.h			\
				ic_text_field.c			\
				ic_text_field.h			\
				ic_titlebar.c			\
				ic_titlebar.h			\
				ic_toolbar.c			\
				ic_toolbar.h			\
				ic_tray_icon.c			\
				ic_tray_icon.h
