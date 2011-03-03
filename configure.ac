dnl Copyright 2011 Saleem Abdulrasool <compnerd@compnerd.org>
dnl vim: set sw=4 sts=4 et fdm=marker fmr={{{,}}} :

dnl {{{ program, version
AC_PREREQ(2.59)
AC_INIT(eds, 0.0.1, compnerd@compnerd.org)
AM_INIT_AUTOMAKE([1.11.1 subdir-objects dist-bzip2])
dnl }}}

dnl {{{ options
AC_ARG_WITH([examples],
            AS_HELP_STRING([--with-examples], [build example program]),,
            [with_examples=yes])
AM_CONDITIONAL(WITH_EXAMPLES, [test x"$with_examples" = x"yes"])
dnl }}}

dnl {{{ basic toolchain checks
AC_PROG_CC_C99
AC_PROG_INSTALL
dnl }}}

dnl {{{ library checks
AC_CHECK_LIB([m], [sqrt])
dnl }}}

dnl {{{ output
AM_SILENT_RULES(yes)
AC_OUTPUT(Makefile)

echo "
Configuration:
    prefix:             ${prefix}
    with examples:      ${with_examples}
"
dnl }}}

