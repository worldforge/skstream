# Configure paths for skstream
# Stolen by Michael Koch
# Adapted by Al Riddoch
# stolen from Sam Lantinga of SDL
# stolen from Manish Singh
# stolen back from Frank Belew
# stolen from Manish Singh
# Shamelessly stolen from Owen Taylor

dnl AM_PATH_SKSTREAM([MINIMUM-VERSION, [ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]]])
dnl Test for skstream, and define SKSTREAM_CFLAGS and SKSTREAM_LIBS
dnl
AC_DEFUN(AM_PATH_SKSTREAM,
[dnl 
dnl Get the cflags and libraries from the skstream-config script
dnl
AC_LANG_CPLUSPLUS
AC_ARG_WITH(skstream-prefix,[  --with-skstream-prefix=PREFIX
                          Prefix where skstream is installed (optional)],
            skstream_prefix="$withval", skstream_prefix="")
AC_ARG_WITH(skstream-exec-prefix,[  --with-skstream-exec-prefix=PREFIX
                          Exec prefix where skstream is installed (optional)],
            skstream_exec_prefix="$withval", skstream_exec_prefix="")
AC_ARG_ENABLE(skstreamtest, [  --disable-skstreamtest     Do not try to compile and run a test skstream program],
		    , enable_skstreamtest=yes)

  if test x$skstream_exec_prefix != x ; then
     skstream_args="$skstream_args --exec-prefix=$skstream_exec_prefix"
     if test x${SKSTREAM_CONFIG+set} != xset ; then
        SKSTREAM_CONFIG=$skstream_exec_prefix/bin/skstream-config
     fi
  fi
  if test x$skstream_prefix != x ; then
     skstream_args="$skstream_args --prefix=$skstream_prefix"
     if test x${SKSTREAM_CONFIG+set} != xset ; then
        SKSTREAM_CONFIG=$skstream_prefix/bin/skstream-config
     fi
  fi

  AC_PATH_PROG(SKSTREAM_CONFIG, skstream-config, no)
  min_skstream_version=ifelse([$1], ,0.3.11,$1)
  AC_MSG_CHECKING(for skstream - version >= $min_skstream_version)
  no_skstream=""
  if test "$SKSTREAM_CONFIG" = "no" ; then
    no_skstream=yes
  else
    SKSTREAM_CFLAGS=`$SKSTREAM_CONFIG $skstreamconf_args --cflags`
    SKSTREAM_LIBS=`$SKSTREAM_CONFIG $skstreamconf_args --libs`

    skstream_major_version=`$SKSTREAM_CONFIG $skstream_args --version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\1/'`
    skstream_minor_version=`$SKSTREAM_CONFIG $skstream_args --version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\2/'`
    skstream_micro_version=`$SKSTREAM_CONFIG $skstream_config_args --version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\3/'`
    if test "x$enable_skstreamtest" = "xyes" ; then
      ac_save_CFLAGS="$CFLAGS"
      ac_save_LIBS="$LIBS"
      CFLAGS="$CFLAGS $SKSTREAM_CFLAGS"
      LIBS="$LIBS $SKSTREAM_LIBS"
dnl
dnl Now check if the installed skstream is sufficiently new. (Also sanity
dnl checks the results of skstream-config to some extent)
dnl
      rm -f conf.skstreamtest
      AC_TRY_RUN([
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char*
my_strdup (char *str)
{
  char *new_str;
  
  if (str)
    {
      new_str = (char *)malloc ((strlen (str) + 1) * sizeof(char));
      strcpy (new_str, str);
    }
  else
    new_str = NULL;
  
  return new_str;
}

int main (int argc, char *argv[])
{
  int major, minor, micro;
  char *tmp_version;

  /* This hangs on some systems (?)
  system ("touch conf.skstreamtest");
  */
  { FILE *fp = fopen("conf.skstreamtest", "a"); if ( fp ) fclose(fp); }

  /* HP/UX 9 (%@#!) writes to sscanf strings */
  tmp_version = my_strdup("$min_skstream_version");
  if (sscanf(tmp_version, "%d.%d.%d", &major, &minor, &micro) != 3) {
     printf("%s, bad version string\n", "$min_skstream_version");
     exit(1);
   }

   if (($skstream_major_version > major) ||
      (($skstream_major_version == major) && ($skstream_minor_version > minor)) ||
      (($skstream_major_version == major) && ($skstream_minor_version == minor) && ($skstream_micro_version >= micro)))
    {
      return 0;
    }
  else
    {
      printf("\n*** 'skstream-config --version' returned %d.%d.%d, but the minimum version\n", $skstream_major_version, $skstream_minor_version, $skstream_micro_version);
      printf("*** of skstream required is %d.%d.%d. If skstream-config is correct, then it is\n", major, minor, micro);
      printf("*** best to upgrade to the required version.\n");
      printf("*** If skstream-config was wrong, set the environment variable SKSTREAM_CONFIG\n");
      printf("*** to point to the correct copy of skstream-config, and remove the file\n");
      printf("*** config.cache before re-running configure\n");
      return 1;
    }
}

],, no_skstream=yes,[echo $ac_n "cross compiling; assumed OK... $ac_c"])
       CFLAGS="$ac_save_CFLAGS"
       LIBS="$ac_save_LIBS"
     fi
  fi
  if test "x$no_skstream" = x ; then
     AC_MSG_RESULT(yes)
     ifelse([$2], , :, [$2])     
  else
     AC_MSG_RESULT(no)
     if test "$SKSTREAM_CONFIG" = "no" ; then
       echo "*** The skstream-config script installed by skstream could not be found"
       echo "*** If skstream was installed in PREFIX, make sure PREFIX/bin is in"
       echo "*** your path, or set the SKSTREAM_CONFIG environment variable to the"
       echo "*** full path to skstream-config."
     else
       if test -f conf.skstreamtest ; then
        :
       else
          echo "*** Could not run skstream test program, checking why..."
          CFLAGS="$CFLAGS $SKSTREAM_CFLAGS"
          LIBS="$LIBS $SKSTREAM_LIBS"
          AC_TRY_LINK([
#include <stdio.h>
],      [ return 0; ],
        [ echo "*** The test program compiled, but did not run. This usually means"
          echo "*** that the run-time linker is not finding skstream or finding the wrong"
          echo "*** version of skstream. If it is not finding skstream, you'll need to set your"
          echo "*** LD_LIBRARY_PATH environment variable, or edit /etc/ld.so.conf to point"
          echo "*** to the installed location  Also, make sure you have run ldconfig if that"
          echo "*** is required on your system"
	  echo "***"
          echo "*** If you have an old version installed, it is best to remove it, although"
          echo "*** you may also be able to get things to work by modifying LD_LIBRARY_PATH"],
        [ echo "*** The test program failed to compile or link. See the file config.log for the"
          echo "*** exact error that occured. This usually means skstream was incorrectly installed"
          echo "*** or that you have moved skstream since it was installed. In the latter case, you"
          echo "*** may want to edit the skstream-config script: $SKSTREAM_CONFIG" ])
          CFLAGS="$ac_save_CFLAGS"
          LIBS="$ac_save_LIBS"
       fi
     fi
     SKSTREAM_CFLAGS=""
     SKSTREAM_LIBS=""
     ifelse([$3], , :, [$3])
  fi
  AC_SUBST(SKSTREAM_CFLAGS)
  AC_SUBST(SKSTREAM_LIBS)
  rm -f conf.skstreamtest
])
