# vim: set ft=make ts=8 :

if WITH_EXAMPLES
noinst_PROGRAMS = src/examples/parse-edid/parse-edid

src_examples_parse_edid_parse_edid_CFLAGS = -I $(top_srcdir)/src                    \
					    -I $(top_srcdir)/src/eds                \
					    -include $(top_srcdir)/src/eds/macros.h \
					    $(NULL)
src_examples_parse_edid_parse_edid_SOURCES = src/examples/parse-edid/parse-edid.c   \
					     $(NULL)
endif

edsincludedir = $(includedir)/eds
edsinclude_HEADERS = src/eds/cea861.h \
		     src/eds/edid.h   \
		     src/eds/hdmi.h   \
		     $(NULL)

MAINTAINERCLEANFILES = aclocal.m4       \
		       configure        \
		       depcomp          \
		       install-sh       \
		       libtool          \
		       Makefile.in      \
		       missing          \
		       $(NULL)

