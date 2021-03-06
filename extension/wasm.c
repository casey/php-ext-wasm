/*
  +----------------------------------------------------------------------+
  | PHP Version 7                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2018 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: Ivan Enderlin                                                |
  +----------------------------------------------------------------------+
*/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "php.h"
#include "ext/standard/info.h"
#include "php_wasm.h"
#include "php-ext-wasm.h"

static void wasm_binary_destructor(zend_resource *resource)
{
    // noop
}

PHP_FUNCTION(wasm_read_binary)
{
    char *file_path;
    size_t file_path_length;
    zend_string *retval;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &file_path, &file_path_length) == FAILURE) {
        return;
    }

    const Vec_u8 *wasm_binary = wasm_read_binary(file_path);

    int resource_number = zend_register_list_destructors_ex(
        wasm_binary_destructor,
        NULL,
        "wasm_binary",
        42
    );
    zend_resource *resource  = zend_register_resource((void *) wasm_binary, resource_number);

    RETURN_RES(resource);
}

PHP_RINIT_FUNCTION(wasm)
{
#if defined(ZTS) && defined(COMPILE_DL_WASM)
    ZEND_TSRMLS_CACHE_UPDATE();
#endif

    return SUCCESS;
}

PHP_MINFO_FUNCTION(wasm)
{
    php_info_print_table_start();
    php_info_print_table_header(2, "wasm support", "enabled");
    php_info_print_table_end();
}

ZEND_BEGIN_ARG_INFO(arginfo_wasm_read_binary, 0)
    ZEND_ARG_INFO(0, str)
    ZEND_END_ARG_INFO()

    static const zend_function_entry wasm_functions[] = {
                                                         PHP_FE(wasm_read_binary,		arginfo_wasm_read_binary)
                                                         PHP_FE_END
};

zend_module_entry wasm_module_entry = {
                                       STANDARD_MODULE_HEADER,
                                       "wasm",					/* Extension name */
                                       wasm_functions,			/* zend_function_entry */
                                       NULL,							/* PHP_MINIT - Module initialization */
                                       NULL,							/* PHP_MSHUTDOWN - Module shutdown */
                                       PHP_RINIT(wasm),			/* PHP_RINIT - Request initialization */
                                       NULL,							/* PHP_RSHUTDOWN - Request shutdown */
                                       PHP_MINFO(wasm),			/* PHP_MINFO - Module info */
                                       PHP_WASM_VERSION,		/* Version */
                                       STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_WASM
# ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
# endif
    ZEND_GET_MODULE(wasm)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 */
