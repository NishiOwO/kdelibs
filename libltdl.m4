
AC_DEFUN(AC_LIBLTDL,
[
AC_REQUIRE([AC_PROG_CC])
AC_C_CONST
AC_C_INLINE
AC_REQUIRE([AM_PROG_LIBTOOL])
AC_SUBST(LIBTOOL_DEPS)

AC_CACHE_CHECK([which extension is used for shared libraries],
  libltdl_cv_shlibext, [dnl
(
  rm -f conftest
  ./libtool --config > conftest
  . ./conftest
  last=
  for spec in $library_names_spec; do
    last="$spec"
  done
  rm -f conftest
changequote(, )
  echo "$last" | sed 's/^[^.]*//;s/\$.*$//;s/\.$//' > conftest
changequote([, ])
)
libltdl_cv_shlibext=`cat conftest`
rm -f conftest
])
if test -n "$libltdl_cv_shlibext"; then
  AC_DEFINE_UNQUOTED(LTDL_SHLIB_EXT, "$libltdl_cv_shlibext",
    [Define to the extension used for shared libraries, say, ".so". ])
fi

AC_CACHE_CHECK([which variable specifies run-time library path],
  libltdl_cv_shlibpath_var, [dnl
(
  rm -f conftest
  ./libtool --config > conftest
  . ./conftest
  rm -f conftest
  echo "$shlibpath_var" > conftest
)
libltdl_cv_shlibpath_var=`cat conftest`
rm -f conftest
])
if test -n "$libltdl_cv_shlibpath_var"; then
  AC_DEFINE_UNQUOTED(LTDL_SHLIBPATH_VAR, "$libltdl_cv_shlibpath_var",
    [Define to the name of the environment variable that determines the dynamic library search path. ])
fi

AC_CACHE_CHECK([for objdir],
  libltdl_cv_objdir, [dnl
(
  rm -f conftest
  ./libtool --config > conftest
  . ./conftest
  rm -f conftest
  echo "$objdir" > conftest
)
libltdl_cv_objdir=`cat conftest`
rm -f conftest
])
test -z "$libltdl_cv_objdir" && libltdl_cv_objdir=".libs"
AC_DEFINE_UNQUOTED(LTDL_OBJDIR, "$libltdl_cv_objdir/",
  [Define to the sub-directory in which libtool stores uninstalled libraries. ])

AC_HEADER_STDC
AC_CHECK_HEADERS(malloc.h memory.h stdlib.h stdio.h ctype.h dlfcn.h dl.h dld.h)
AC_CHECK_HEADERS(string.h strings.h, break)
AC_CHECK_FUNCS(strdup strchr strrchr index rindex)

AC_CACHE_CHECK([whether libtool supports -dlopen/-dlpreopen],
       libltdl_cv_preloaded_symbols, [dnl
  (
    rm -f conftest
    ./libtool --config > conftest
    . ./conftest
    rm -f conftest
    if test -n "$global_symbol_pipe"; then
      echo yes > conftest
    else
      echo no > conftest
    fi
  )
  libltdl_cv_preloaded_symbols=`cat conftest`
  rm -f conftest
])
if test x"$libltdl_cv_preloaded_symbols" = x"yes"; then
  AC_DEFINE(HAVE_PRELOADED_SYMBOLS, 1,
    [Define if libtool can extract symbol lists from object files. ])
fi

test_dlerror=no
LIBADD_DL=
AC_CHECK_FUNCS(dlopen, [AC_DEFINE(HAVE_LIBDL, 1,
			  [Define if you have the libdl library or equivalent. ]) test_dlerror=yes],
  [AC_CHECK_LIB(dl, dlopen, [AC_DEFINE(HAVE_LIBDL, 1,
			       [Define if you have the libdl library. ]) dnl
			     LIBADD_DL="-ldl" test_dlerror=yes],
    [AC_CHECK_LIB(dld, dld_link, [AC_DEFINE(HAVE_DLD, 1,
				    [Define if you have the GNU dld library. ]) dnl
				  LIBADD_DL="-ldld"],
      [AC_CHECK_FUNCS(shl_load, [AC_DEFINE(HAVE_SHL_LOAD, 1,
				   [Define if you have the shl_load function. ])])]
    )]
  )]
)
AC_SUBST(LIBADD_DL)

if test "$test_dlerror" = yes; then
 LIBS_SAVE="$LIBS"
 LIBS="$LIBS $LIBADD_DL"
 AC_CHECK_FUNCS(dlerror)
 LIBS="$LIBS_SAVE"
fi

AC_SYS_SYMBOL_UNDERSCORE
if test x"$USE_SYMBOL_UNDERSCORE" = xyes; then
  if test x"$ac_cv_func_dlopen" = xyes ||
     test x"$ac_cv_lib_dl_dlopen" = xyes ; then
	AC_CACHE_CHECK([whether we have to add an underscore for dlsym],
		libltdl_cv_need_uscore, [dnl
		AC_TRY_RUN([
#if HAVE_DLFCN_H
#include <dlfcn.h>
#endif

#include <stdio.h>

#ifdef RTLD_GLOBAL
# define LTDL_GLOBAL	RTLD_GLOBAL
#else
# ifdef DL_GLOBAL
#  define LTDL_GLOBAL	DL_GLOBAL
# else
#  define LTDL_GLOBAL	0
# endif
#endif

/* We may have to define LTDL_LAZY_OR_NOW in the command line if we
   find out it does not work in some platform. */
#ifndef LTDL_LAZY_OR_NOW
# ifdef RTLD_LAZY
#  define LTDL_LAZY_OR_NOW	RTLD_LAZY
# else
#  ifdef DL_LAZY
#   define LTDL_LAZY_OR_NOW	DL_LAZY
#  else
#   ifdef RTLD_NOW
#    define LTDL_LAZY_OR_NOW	RTLD_NOW
#   else
#    ifdef DL_NOW
#     define LTDL_LAZY_OR_NOW	DL_NOW
#    else
#     define LTDL_LAZY_OR_NOW	0
#    endif
#   endif
#  endif
# endif
#endif

fnord() { int i=42;}
main() { void *self, *ptr1, *ptr2; self=dlopen(0,LTDL_GLOBAL|LTDL_LAZY_OR_NOW);
    if(self) { ptr1=dlsym(self,"fnord"); ptr2=dlsym(self,"_fnord");
	       if(ptr1 && !ptr2) exit(0); } exit(1); } 
],	libltdl_cv_need_uscore=no, libltdl_cv_need_uscore=yes,
	libltdl_cv_need_uscore=cross
)])
  fi
fi

if test x"$libltdl_cv_need_uscore" = xyes; then
  AC_DEFINE(NEED_USCORE, 1,
    [Define if dlsym() requires a leading underscode in symbol names. ])
fi

])
