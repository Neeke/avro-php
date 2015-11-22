dnl $Id$
dnl config.m4 for extension test_avro

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

PHP_ARG_WITH(test_avro, for test_avro support,
dnl Make sure that the comment is aligned:
[  --with-test_avro             Include test_avro support])

dnl Otherwise use enable:

dnl PHP_ARG_ENABLE(test_avro, whether to enable test_avro support,
dnl Make sure that the comment is aligned:
dnl [  --enable-test_avro           Enable test_avro support])

if test "$PHP_TEST_AVRO" != "no"; then
  dnl Write more examples of test_avros here...

  dnl # --with-test_avro -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/test_avro.h"  # you most likely want to change this
  dnl if test -r $PHP_TEST_AVRO/$SEARCH_FOR; then # path given as parameter
  dnl   TEST_AVRO_DIR=$PHP_TEST_AVRO
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for test_avro files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       TEST_AVRO_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$TEST_AVRO_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the test_avro distribution])
  dnl fi

  dnl # --with-test_avro -> add include path
  PHP_ADD_INCLUDE($TEST_AVRO_DIR/avrolib/include)

  dnl # --with-test_avro -> check for lib and symbol presence
  LIBNAME=avro # you may want to change this
  LIBSYMBOL=avro_schema_from_json_length # you most likely want to change this

  PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  [
    PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $TEST_AVRO_DIR/avrolib/lib, TEST_AVRO_SHARED_LIBADD)
    AC_DEFINE(HAVE_TEST_AVROLIB,1,[ ])
  ],[
    AC_MSG_ERROR([wrong test_avro lib version or lib not found])
  ],[
    -L$TEST_AVRO_DIR/avrolib/lib -lavro
  ])

  PHP_SUBST(TEST_AVRO_SHARED_LIBADD)

  PHP_NEW_EXTENSION(test_avro, test_avro.c, $ext_shared)
fi
