
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <avro.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "php.h"
#include "php_ini.h"
#include "main/SAPI.h"
#include "ext/standard/info.h"
#include "ext/standard/php_smart_str.h"
#include "php_test_avro.h"
#include "tools/log_udp.c"

/* If you declare any globals in php_test_avro.h uncomment this: */
ZEND_DECLARE_MODULE_GLOBALS(test_avro)
static PHP_GINIT_FUNCTION(test_avro);

/* True global resources - no need for thread safety here */
static int le_test_avro;

/* {{{ test_avro_functions[]
 *
 * Every user visible function must have an entry in test_avro_functions[].
 */
const zend_function_entry test_avro_functions[] = {
	PHP_FE(confirm_test_avro_compiled,	NULL)		/* For test_avroing, remove later. */
	PHP_FE(hello_avro,	NULL)
	PHP_FE(self_concat,	NULL)
	PHP_FE_END	/* Must be the last line in test_avro_functions[] */
};
/* }}} */


#if ZEND_MODULE_API_NO >= 20050922
zend_module_dep test_avro_deps[] = {
//	ZEND_MOD_REQUIRED("curl")
	ZEND_MOD_REQUIRED("json")
	{NULL, NULL, NULL}
};
#endif

#ifdef COMPILE_DL_TEST_AVRO
ZEND_GET_MODULE(test_avro)
#endif

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini */
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("test_avro.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_test_avro_globals, test_avro_globals)
    STD_PHP_INI_ENTRY("test_avro.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_test_avro_globals, test_avro_globals)
PHP_INI_END()

/* }}} */

PHP_GINIT_FUNCTION(test_avro)
{
//    php_printf("GINIT START\n");

    memset(test_avro_globals, 0, sizeof(zend_test_avro_globals));
//    initGlobals(TSRMLS_C);
    test_avro_globals->test_avro_long = 15;
    test_avro_globals->test_avro_char = "char is a test_avro.";

    zend_hash_init(&test_avro_globals->test_avro_zval, 0, NULL, NULL, 1);
    zend_hash_add_empty_element(&test_avro_globals->test_avro_zval,TS_S_L("curl_init"));
    zend_hash_add_empty_element(&test_avro_globals->test_avro_zval,TS_S_L("curl_setopt"));
    zend_hash_add_empty_element(&test_avro_globals->test_avro_zval,TS_S_L("curl_setopt_array"));
    zend_hash_add_empty_element(&test_avro_globals->test_avro_zval,TS_S_L("curl_exec"));
    zend_hash_add_empty_element(&test_avro_globals->test_avro_zval,TS_S_L("file_get_contents"));

    //初始化类的白名单
    zend_hash_init(&test_avro_globals->classMethodWhite, 0, NULL, NULL, 1);

    //PDO
    HashTable pdoMethod; zend_hash_init(&pdoMethod, 0, NULL, NULL, 1);
    zend_hash_add_empty_element(&pdoMethod,TS_S_L("__construct"));
    zend_hash_add_empty_element(&pdoMethod,TS_S_L("query"));
    zend_hash_add_empty_element(&pdoMethod,TS_S_L("prepare"));
    zend_hash_add(&test_avro_globals->classMethodWhite, "PDO",strlen("PDO")+1, (void *)&pdoMethod, sizeof(HashTable), NULL);

    //mysqli
    HashTable mysqliMethod; zend_hash_init(&mysqliMethod, 0, NULL, NULL, 1);
    zend_hash_add_empty_element(&mysqliMethod,TS_S_L("prepare"));
    zend_hash_add(&test_avro_globals->classMethodWhite, "mysqli",strlen("mysqli")+1, (void *)&mysqliMethod, sizeof(HashTable), NULL);

    //Memcache
    HashTable memcacheMethod; zend_hash_init(&memcacheMethod, 0, NULL, NULL, 1);
    zend_hash_add_empty_element(&memcacheMethod,TS_S_L("connect"));
    zend_hash_add(&test_avro_globals->classMethodWhite, "Memcache",strlen("Memcache")+1, (void *)&memcacheMethod, sizeof(HashTable), NULL);

    //Redis
    HashTable redisMethod; zend_hash_init(&redisMethod, 0, NULL, NULL, 1);
    zend_hash_add_empty_element(&redisMethod,TS_S_L("connect"));
    zend_hash_add(&test_avro_globals->classMethodWhite, "Redis",strlen("Redis")+1, (void *)&redisMethod, sizeof(HashTable), NULL);

    //Predis\Client
    HashTable PredisClientMethod; zend_hash_init(&PredisClientMethod, 0, NULL, NULL, 1);
    zend_hash_add_empty_element(&PredisClientMethod,TS_S_L("connect"));
    zend_hash_add(&test_avro_globals->classMethodWhite, "Predis\\Client",strlen("Predis\\Client")+1, (void *)&PredisClientMethod, sizeof(HashTable), NULL);

    //Mongo
    HashTable MongoMethod; zend_hash_init(&MongoMethod, 0, NULL, NULL, 1);
    zend_hash_add_empty_element(&MongoMethod,TS_S_L("__construct"));
    zend_hash_add(&test_avro_globals->classMethodWhite, "Mongo",strlen("Mongo")+1, (void *)&MongoMethod, sizeof(HashTable), NULL);


//    php_printf("GINIT END\n");
}

PHP_GSHUTDOWN_FUNCTION(test_avro)
{
//    php_printf("GSHUTDOWN START\n");
    zend_hash_destroy(&test_avro_globals->test_avro_zval);
//    php_printf("GSHUTDOWN END\n");
}

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(test_avro)
{
//    php_printf("MINIT START\n");

	/* If you have INI entries, uncomment these lines */
	REGISTER_INI_ENTRIES();

//    php_printf("MINIT END\n");
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(test_avro)
{
//    php_printf("MSHUTDOWN START\n");
	/* uncomment this line if you have INI entries */
	UNREGISTER_INI_ENTRIES();

//    php_printf("MSHUTDOWN END\n");
	return SUCCESS;
}
/* }}} */


/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(test_avro)
{
//    php_printf("RINIT START\n");

//    php_printf("TEST_AVRO_G(global_value) %d\n",TEST_AVRO_G(global_value));
//    php_printf("TEST_AVRO_G(test_avro_long) %d\n",TEST_AVRO_G(test_avro_long));
//    php_printf("TEST_AVRO_G(test_avro_char) %s\n",TEST_AVRO_G(test_avro_char));
//    if (TEST_AVRO_G(test_avro_zval)) {
////        php_printf("TEST_AVRO_G(test_avro_zval) 's type is %ld\n",Z_TYPE_P(TEST_AVRO_G(test_avro_zval)));
////        php_var_dump(TEST_AVRO_G(test_avro_zval),1 TSRMLS_CC);
//    }

//    php_printf("TEST_AVRO_G(global_value) %d\n",TEST_AVRO_G(global_value));

//    php_printf("RINIT END\n");
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(test_avro)
{
//    php_printf("SG(sapi_headers).mimetype is %s\n",SG(sapi_headers).mimetype);
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(test_avro)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "test_avro support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini */
	DISPLAY_INI_ENTRIES();
}
/* }}} */

zend_module_entry test_avro_module_entry =
{
#if ZEND_MODULE_API_NO >= 20050922
	STANDARD_MODULE_HEADER_EX, NULL,
	test_avro_deps,
#else
	STANDARD_MODULE_HEADER,
#endif
    "test_avro",
    test_avro_functions,
    PHP_MINIT(test_avro),
    PHP_MSHUTDOWN(test_avro),
    PHP_RINIT(test_avro),
    PHP_RSHUTDOWN(test_avro),
    PHP_MINFO(test_avro),
    "0.1",
    PHP_MODULE_GLOBALS(test_avro),
    PHP_GINIT(test_avro),
    PHP_GSHUTDOWN(test_avro),
    NULL,
    STANDARD_MODULE_PROPERTIES_EX
};


/* Remove the following function when you have succesfully modified config.m4
   so that your module can be compiled into PHP, it exists only for test_avroing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_test_avro_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(confirm_test_avro_compiled)
{
	char *arg = NULL;
	int arg_len, len;
	char *strg;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &arg, &arg_len) == FAILURE) {
		return;
	}

	len = spprintf(&strg, 0, "Congratulations! You have successfully modified ext/%.78s/config.m4. Module %.78s is now compiled into PHP.", "test_avro", arg);
	RETURN_STRINGL(strg, len, 0);
}
/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and
   unfold functions in source code. See the corresponding marks just before
   function definition, where the functions purpose is also documented. Please
   follow this convention for the convenience of others editing your code.
*/


void init_rand(void)
{
	srand(time(NULL));
}

double rand_number(double from, double to)
{
	double range = to - from;
	return from + ((double)rand() / (RAND_MAX + 1.0)) * range;
}

int64_t rand_int64(void)
{
	return (int64_t) rand_number(LONG_MIN, LONG_MAX);
}

int32_t rand_int32(void)
{
	return (int32_t) rand_number(INT_MIN, INT_MAX);
}

/* {{{ proto int hello(int a, int b)
    */
PHP_FUNCTION(hello_avro)
{
	int argc = ZEND_NUM_ARGS();
	long a;
	long b;

	if (zend_parse_parameters(argc TSRMLS_CC, "ll", &a, &b) == FAILURE)
		return;


//    php_printf("hello start\n");

    php_printf("hello_avro start\n");
    init_rand();

 	static const char  *schema_json =
 		"{"
 		"  \"type\": \"record\","
 		"  \"name\": \"test\","
 		"  \"fields\": ["
 		"    { \"name\": \"i\", \"type\": \"int\" },"
 		"    { \"name\": \"l\", \"type\": \"long\" },"
 		"    { \"name\": \"s\", \"type\": \"string\" },"
 		"    {"
 		"      \"name\": \"subrec\","
 		"      \"type\": {"
 		"        \"type\": \"record\","
 		"        \"name\": \"sub\","
 		"        \"fields\": ["
 		"          { \"name\": \"f\", \"type\": \"float\" },"
 		"          { \"name\": \"d\", \"type\": \"double\" }"
 		"        ]"
 		"      }"
 		"    }"
 		"  ]"
 		"}";

 	static const char *strings[] = {
 		"Four score and seven years ago",
 		"our father brought forth on this continent",
 		"a new nation", "conceived in Liberty",
 		"and dedicated to the proposition that all men are created equal."
 	};
 	static const unsigned int  NUM_STRINGS = sizeof(strings) / sizeof(strings[0]);

 	int  rc;
 	static char  buf[4096];
 	avro_reader_t  reader = avro_reader_memory(buf, sizeof(buf));
 	avro_writer_t  writer = avro_writer_memory(buf, sizeof(buf));

 	avro_schema_t  schema = NULL;
 	avro_schema_error_t  error = NULL;
 	avro_schema_from_json(schema_json, strlen(schema_json), &schema, &error);

 	unsigned long  i;

 	avro_datum_t  in = avro_datum_from_schema(schema);

        char  *json_in = NULL;
        char  *json_out = NULL;

 	for (i = 0; i < 100; i++) {
 		avro_record_set_field_value(rc, in, int32, "i", rand_int32());
 		avro_record_set_field_value(rc, in, int64, "l", rand_int64());
 		avro_record_set_field_value(rc, in, givestring, "s", strings[i % NUM_STRINGS], NULL);

 		avro_datum_t  subrec = NULL;
 		avro_record_get(in, "subrec", &subrec);
 		avro_record_set_field_value(rc, in, float, "f", rand_number(-1e10, 1e10));
 		avro_record_set_field_value(rc, in, double, "d", rand_number(-1e10, 1e10));

 		avro_writer_reset(writer);
 		avro_write_data(writer, schema, in);

        avro_datum_to_json(in, 1, &json_in);
        php_printf("%s\n",json_in);

        udp_send(writer,sizeof(writer));

 		avro_datum_t  out = NULL;

 		avro_reader_reset(reader);
 		avro_read_data(reader, schema, schema, &out);

        avro_datum_to_json(out, 1, &json_out);
        php_printf("%s\n",json_out);


 		avro_datum_equal(in, out);
 		avro_datum_decref(out);
 	}

  	avro_datum_decref(in);
  	avro_schema_decref(schema);
  	avro_writer_free(writer);
  	avro_reader_free(reader);

//    php_printf("TEST_AVRO_G(global_value) %d\n",TEST_AVRO_G(global_value));
//
//    php_printf("hello end\n");


//	php_error(E_WARNING, "hello: not yet implemented");

//    RETURN_ZVAL(TEST_AVRO_G(test_avro_zval), 1, 0);

    RETURN_LONG(a+b);
}
/* }}} */

/* {{{ proto string self_concat(string str, int n)

*/

PHP_FUNCTION(self_concat)
{

    char *str = NULL;

    int argc = ZEND_NUM_ARGS();

    int str_len;

    long n;

    char *result; /* Points to resulting string */

    char *ptr; /* Points at the next location we want to copy to */

    int result_length; /* Length of resulting string */

    if (zend_parse_parameters(argc TSRMLS_CC, "sl", &str, &str_len, &n) == FAILURE)

    return;

    /* Calculate length of result */

    result_length = (str_len * n);

    /* Allocate memory for result */

    result = (char *) emalloc(result_length + 1);

    /* Point at the beginning of the result */

    ptr = result;

    while (n--) {

        /* Copy str to the result */

        memcpy(ptr, str, str_len);

        /* Increment ptr to point at the next position we want to write to */

        ptr += str_len;

    }

    /* Null terminate the result. Always null-terminate your strings

    even if they are binary strings */

    *ptr = '\0';

    /* Return result to the scripting engine without duplicating it*/

    RETURN_STRINGL(result, result_length, 0);

}

/* }}} */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
