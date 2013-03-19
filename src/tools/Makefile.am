AM_CFLAGS = \
	-DRESDIR=\"$(pkgdatadir)/resources/\" \
	-I../common \
	-I../gui \
	$(GTK3_CFLAGS) \
	$(XML2_CFLAGS) 
	
AM_LDFLAGS = \
	$(GTK3_LIBS) \
	$(XML2_LIBS) 

noinst_LTLIBRARIES = libxml_parser.la

libxml_parser_la_SOURCES = ic_xml_parser.c \
						ic_xml_parser.h
