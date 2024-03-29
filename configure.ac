dnl Process this file with autoconf to produce a configure script.

AC_INIT([tux_aqfh],[1.0.14])
AC_CONFIG_SRCDIR([src/tux.cxx])
AM_INIT_AUTOMAKE([1.16])

dnl Checks for programs.
AC_PROG_CC
AC_PROG_CPP
AC_PROG_CXX
AC_PROG_CXXCPP
AC_PROG_INSTALL
AC_PROG_RANLIB

AC_CHECK_FUNCS([floor])
AC_CHECK_FUNCS([gettimeofday])
AC_CHECK_FUNCS([pow])
AC_CHECK_FUNCS([select])
AC_CHECK_FUNCS([sqrt])
AC_CHECK_FUNCS([strtol])
AC_CHECK_HEADERS([sys/time.h])
AC_CHECK_HEADERS([unistd.h])
AC_C_INLINE
AC_FUNC_STRTOD

plib_suffix="-lplibsl -lplibssg -lplibpu -lplibfnt -lplibsg -lplibul"

AC_PREREQ([2.71])

dnl List files of a given extension in a given directory
file_list() {
  for f in $top_srcdir/$1/*.$2; do
    echo $(basename $f)
  done
}

dnl # Create the list of files
AC_SUBST([DAT_FILES], [$(file_list data dat)])
AC_SUBST([HTML_FILES], [$(file_list doc html)])
AC_SUBST([PNG_FILES], [$(file_list doc png)])
AC_SUBST([TXF_FILES], [$(file_list fonts txf)])
AC_SUBST([RGB_FILES], [$(file_list images rgb)])
AC_SUBST([RGBA_FILES], [$(file_list images rgba)])
AC_SUBST([INT_FILES], [$(file_list images int)])
AC_SUBST([INTA_FILES], [$(file_list images inta)])
AC_SUBST([AC_FILES], [$(file_list models ac)])
AC_SUBST([MOD_FILES], [$(file_list mods mod)])
AC_SUBST([WAV_FILES], [$(file_list wavs wav)])
AC_SUBST([PENGUIN_AC_FILES], [$(file_list penguin ac)])
AC_SUBST([PENGUIN_RGB_FILES], [$(file_list penguin rgb)])
AC_SUBST([PENGUIN_RGBA_FILES], [$(file_list penguin rgba)])
AC_SUBST([SLAM_FILES], [$(file_list slamcode slam)])

dnl Checks for library functions.
dnl check for OpenGL related libraries

AC_CHECK_HEADER(windows.h, AC_DEFINE(WIN32))

if test "x$ac_cv_header_windows_h" = "xno" ; then

  dnl Check for X11

  AC_PATH_XTRA

  x_suffix="$X_LIBS $X_PRE_LIBS -lpthread -lX11 -lXi -lXext -lXmu $X_EXTRA_LIBS -lm"

  dnl Reasonable stuff non-windoze variants ... :-)

  AC_CHECK_LIB(GL, glNewList,,,$x_suffix)

  if test "x$ac_cv_lib_GL_glNewList" = "xno" ; then
    dnl if no GL, check for MesaGL
    AC_CHECK_LIB(MesaGL, glNewList,,,$x_suffix)
  fi

  AC_CHECK_LIB(GLU, gluLookAt,,,$x_suffix)

  if test "x$ac_cv_lib_GLU_gluLookAt" = "xno" ; then
    dnl if no GLU, check for MesaGLU
    AC_CHECK_LIB(MesaGLU, gluLookAt,,,$x_suffix)
  fi

  opengl_LIBS="$LIBS $x_suffix"

  AC_CHECK_LIB(glut, glutGetModifiers,,,$opengl_LIBS)

  if test "x$ac_cv_lib_glut_glutGetModifiers" = "xno" ; then
    dnl if no GLUT, check for freeglut
    AC_CHECK_LIB(freeglut, glutGetModifiers,,,$opengl_LIBS)
  fi

  LIBS="$plib_suffix $LIBS $x_suffix"

else

  dnl CygWin under Windoze.

  LIBS="$plib_suffix $LIBS -lm -lwinmm -lglut32 -lglu32 -lopengl32 -luser32 -lgdi32"

fi


dnl Check SGI audio library

AC_CHECK_LIB(audio, ALopenport)

if test "x$ac_cv_lib_audio_ALopenport" = "xyes" ; then
  dnl this is an SGI machine...
  LIBS="$LIBS -laudio"
fi

dnl Checks for header files.

AC_CHECK_HEADERS(GL/gl.h GL/glu.h GL/glut.h)

AC_CHECK_HEADER(windows.h, AC_DEFINE(WIN32))
AC_CHECK_HEADER(joystick.h, AC_DEFINE(JOYSTICK_IS_PRESENT))
AC_CHECK_HEADER(linux/joystick.h, AC_DEFINE(LINUX_JOYSTICK_IS_PRESENT))

dnl Check for audio support

AM_CONDITIONAL(ENABLE_AUDIO, \
        test -r /usr/include/soundcard.h \
                -o -r /usr/include/linux/soundcard.h \
                -o -r /usr/include/machine/soundcard.h \
                -o -r /usr/include/audio.h \
                -o "x$ac_cv_header_windows_h" = "xyes" )

AM_CONDITIONAL(ENABLE_IRIX_AUDIO, test -r /usr/include/audio.h)

AM_CONDITIONAL(ENABLE_WIN32_AUDIO, test "x$ac_cv_header_windows_h" = "xyes")

if test "x$ac_cv_header_windows_h" != "xyes" ; then
  if test "x$GXX" != "xyes" ; then
    CFLAGS="$CFLAGS -I/usr/local/include -L/usr/local/lib"
    CXXFLAGS="$CXXFLAGS -I/usr/local/include -L/usr/local/lib"
  else
    CFLAGS="$CFLAGS"
    CXXFLAGS="$CXXFLAGS"
  fi
else
  CFLAGS="$CFLAGS $MACH -malign-double -I/usr/local/include -L/usr/local/lib"
  CXXFLAGS="$CXXFLAGS $MACH -malign-double -I/usr/local/include -L/usr/local/lib"
fi

MY_EXPAND_DIR(tux_aqfh_datadir, "$datadir/games/$PACKAGE")
AC_DEFINE_UNQUOTED(TUX_AQFH_DATADIR, "$tux_aqfh_datadir")

AC_CONFIG_FILES([Makefile          \
           contrib/Makefile  \
           data/Makefile     \
           doc/Makefile     \
           fonts/Makefile    \
           images/Makefile   \
           models/Makefile   \
           mods/Makefile     \
           penguin/Makefile  \
           slamcode/Makefile \
           src/Makefile      \
           wavs/Makefile     \
          ])
AC_OUTPUT

