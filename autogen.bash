#!/bin/bash
# Copyright 2010 Saleem Abdulrasool <compnerd@compnerd.org>

: ${ACLOCAL:=aclocal}
: ${AUTOCONF:=autoconf}
: ${AUTOMAKE:=automake}

run()
{
   echo ">>> ${@}" >&2
   if ! ${@} ; then
      echo "failed to configure autotools" >&2
      exit 127
   fi
}

srcdir=$(dirname "${0}")
[[ -z ${srcdir} ]] && srcdir='.'

( "${AUTOCONF}" --version ) </dev/null >/dev/null 2>&1 || {
   echo
   echo "You must have autoconf installed to compile ${PROJECT}."
   echo "Download the appropriate package for your distribution,"
   echo "or get the source tarball at ftp://ftp.gnu.org/pub/gnu/"
   echo

   DIE=1
}

( "${AUTOMAKE}" --version ) </dev/null >/dev/null 2>&1 || {
   echo
   echo "You must have automake installed to compile ${PROJECT}."
   echo "Download the appropriate package for your distribution,"
   echo "or get the source tarball at ftp://ftp.gnu.org/pub/gnu/"
   echo

   DIE=1
}

if [[ ${DIE} == 1 ]] ; then
   exit 127
fi

run "${ACLOCAL}"
rm -f config.cache
run "${AUTOCONF}"
run "${AUTOMAKE}" --add-missing --copy --foreign

