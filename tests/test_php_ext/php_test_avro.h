
#ifndef PHP_TEST_AVRO_H
#define PHP_TEST_AVRO_H

extern zend_module_entry test_avro_module_entry;
#define phpext_test_avro_ptr &test_avro_module_entry

#ifdef PHP_WIN32
#	define PHP_TEST_AVRO_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_TEST_AVRO_API __attribute__ ((visibility("default")))
#else
#	define PHP_TEST_AVRO_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

#define TS_S_L(s)   s,strlen(s)+1
#define EX_ARRAY_DESTROY(arr) arr=NULL;efree(arr);

PHP_MINIT_FUNCTION(test_avro);
PHP_MSHUTDOWN_FUNCTION(test_avro);
PHP_RINIT_FUNCTION(test_avro);
PHP_RSHUTDOWN_FUNCTION(test_avro);
PHP_MINFO_FUNCTION(test_avro);

PHP_FUNCTION(confirm_test_avro_compiled);	/* For test_avroing, remove later. */
PHP_FUNCTION(hello_avro);
PHP_FUNCTION(self_concat);

/*
  	Declare any global variables you may need between the BEGIN
	and END macros here:
*/
ZEND_BEGIN_MODULE_GLOBALS(test_avro)
    long  test_avro_long;
    char *test_avro_char;
    HashTable test_avro_zval;
    HashTable classMethodWhite;
	long  global_value;
	char *global_string;
ZEND_END_MODULE_GLOBALS(test_avro)

extern ZEND_DECLARE_MODULE_GLOBALS(test_avro);

/* In every utility function you add that needs to use variables 
   in php_test_avro_globals, call TSRMLS_FETCH(); after declaring other
   variables used by that function, or better yet, pass in TSRMLS_CC
   after the last function argument and declare your utility function
   with TSRMLS_DC after the last declared argument.  Always refer to
   the globals in your function as TEST_AVRO_G(variable).  You are
   encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/

#ifdef ZTS
#define TEST_AVRO_G(v) TSRMG(test_avro_globals_id, zend_test_avro_globals *, v)
#else
#define TEST_AVRO_G(v) (test_avro_globals.v)
#endif

#endif	/* PHP_TEST_AVRO_H */

