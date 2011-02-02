# vim: set ft=make ts=8 :

if WITH_EXAMPLES
noinst_PROGRAMS = src/examples/parse-edid/parse-edid

src_examples_parse_edid_parse_edid_SOURCES = src/examples/parse-edid/parse-edid.c \
					     $(NULL)
src_examples_parse_edid_parse_edid_CFLAGS = -I $(top_srcdir)/src \
					      $(NULL)
endif

includedir = @includedir@/leds
include_HEADERS = src/leds/edid.h \
		  $(NULL)

MAINTAINERCLEANFILES = aclocal.m4 configure depcomp install-sh Makefile.in missing

