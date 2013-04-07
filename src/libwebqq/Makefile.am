SUBDIRS=libghttp
noinst_LTLIBRARIES=libwebqq.la
libwebqq_la_LIBADD = ./libghttp/libghttp.la 

AM_CFLAGS = -I./libghttp/ \
			$(GLIB_CFLAGS)

AM_LDFLAGS = -lev \
			 -lghttp \
			 $(GLIB_LIBS) 
			  
libwebqq_la_SOURCES=http.c \
					info.c \
					json.c \
					logger.c \
					login.c \
					lwdb.c \
					md5.c \
					msg.c \
					smemory.c \
					swsqlite.c \
					type.c \
					unicode.c \
					url.c

