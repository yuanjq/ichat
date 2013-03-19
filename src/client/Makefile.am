bin_PROGRAMS = ichat
ichat_SOURCES = ic_client_main.c
ichat_LDADD = ../gui/libgui.la 

AM_CFLAGS = \
	-DRESDIR=\"$(pkgdatadir)/\" \
	-I../gui \
	-I../common \
	$(GTK3_CFLAGS) 

AM_LDFLAGS = \
	$(GTK3_LIBS) 
