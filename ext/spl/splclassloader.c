/*
   +----------------------------------------------------------------------+
   | PHP Version 5.4                                                        |
   +----------------------------------------------------------------------+
   | Copyright (c) 1997-2013 The PHP Group                                |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.php.net/license/3_01.txt                                  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
   | Authors: metagoto <runpac314 gmail com>                              |
   +----------------------------------------------------------------------+
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "zend_interfaces.h"
#include "php_splclassloader.h"


#ifdef COMPILE_DL_SPLCLASSLOADER
ZEND_GET_MODULE(splclassloader)
#endif

/* zend_API.h: #define ZEND_NS_NAME(ns, name)			ns"\\"name */
#define SPLCLASSLD_NS_SEPARATOR '\\'


static zend_class_entry* splclassloader_ce;
zend_object_handlers splclassloader_object_handlers;


typedef struct _splclassloader_object { 
    zend_object std;
    char* ns;
    int   ns_len;
    char* inc_path;
    int   inc_path_len;
    char* file_ext;
    int   file_ext_len;
} splclassloader_object;


static void splclassloader_object_dtor(void* object, zend_object_handle handle TSRMLS_DC)
{ 
    splclassloader_object* obj = (splclassloader_object*)object;
    if (obj->ns) {
        efree(obj->ns);
        obj->ns = NULL;
    }
    if (obj->inc_path) {
        efree(obj->inc_path);
        obj->inc_path = NULL;
    }
    if (obj->file_ext) {
        efree(obj->file_ext);
        obj->file_ext = NULL;
    }
    zend_object_std_dtor(&obj->std TSRMLS_CC);
    efree(obj);
}


zend_object_value splclassloader_create_object(zend_class_entry* class_type TSRMLS_DC)
{
    zend_object_value retval;

    splclassloader_object* obj = (splclassloader_object*)emalloc(sizeof(splclassloader_object));
    memset(obj, 0, sizeof(splclassloader_object));

    zend_object_std_init(&obj->std, class_type TSRMLS_CC);
    zend_hash_copy(obj->std.properties, &class_type->default_properties, (copy_ctor_func_t) zval_add_ref, (void*)0, sizeof(zval*));

    obj->file_ext = estrndup(".php", sizeof(".php")-1);
    obj->file_ext_len = sizeof(".php")-1;
    
    retval.handle = zend_objects_store_put(obj, (zend_objects_store_dtor_t)zend_objects_destroy_object, (void*)splclassloader_object_dtor, NULL TSRMLS_CC);
    retval.handlers = &splclassloader_object_handlers;

    return retval;
}


/* {{{ proto void SplClassLoader::__construct([string $namespace [, string $include_path]])
       Constructor */
PHP_METHOD(SplClassLoader, __construct)
{    
    char* ns = NULL;
    int   ns_len = 0;
    char* inc_path = NULL;
    int   inc_path_len = 0;
    splclassloader_object* obj;
    
    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|ss", &ns, &ns_len, &inc_path, &inc_path_len)) {
        return; /* should throw ? */
    }
    
    obj = (splclassloader_object *) zend_object_store_get_object(getThis() TSRMLS_CC);
    
    if (ns_len) {
        obj->ns = estrndup(ns, ns_len);
        obj->ns_len = ns_len;
    }
    if (inc_path_len) {
        obj->inc_path = estrndup(inc_path, inc_path_len);
        obj->inc_path_len = inc_path_len;
    }    
} /* }}} */


/* {{{ proto bool SplClassLoader::register()
   Installs this class loader on the SPL autoload stack */
PHP_METHOD(SplClassLoader, register)
{   
    zval* arr;
    zval* retval = NULL;
    zval* pthis = getThis();
    int res = 0;
    
    MAKE_STD_ZVAL(arr);
    array_init(arr);
    Z_ADDREF_P(pthis);

    add_next_index_zval(arr, pthis);
    add_next_index_string(arr, estrndup("loadClass", sizeof("loadClass")-1), 0);

    zend_call_method_with_1_params(NULL, NULL, NULL, "spl_autoload_register", &retval, arr);

    zval_ptr_dtor(&arr);
    
    if (retval) {
        res = i_zend_is_true(retval);
        zval_ptr_dtor(&retval);
    }
    RETURN_BOOL(res);
} /* }}} */


/* {{{ proto bool SplClassLoader::unregister()
   Uninstalls this class loader from the SPL autoloader stack */
PHP_METHOD(SplClassLoader, unregister)
{   
    zval* arr;
    zval* retval = NULL;
    zval* pthis = getThis();
    int res = 0;
    
    MAKE_STD_ZVAL(arr);
    array_init(arr);
    Z_ADDREF_P(pthis);

    add_next_index_zval(arr, pthis);
    add_next_index_string(arr, estrndup("loadClass", sizeof("loadClass")-1), 0);

    zend_call_method_with_1_params(NULL, NULL, NULL, "spl_autoload_unregister", &retval, arr);

    zval_ptr_dtor(&arr);
    
    if (retval) {
        res = i_zend_is_true(retval);
        zval_ptr_dtor(&retval);
    }
    RETURN_BOOL(res);
} /* }}} */


/* compute filename if (partial) ns and class match. returns filename len including '\0' or 0 */
static int get_filename(splclassloader_object* obj, char* cl, int cl_len, char* filename TSRMLS_DC)
{
    char* ccl = cl;
    char* cns = obj->ns;
    char* cur = &cl[cl_len-1];
    int   len = 0;
    
    if (cl_len < obj->ns_len) {
        return 0; 
    }
    
    /* we have a ns to compare to the qualified class name */
    if (obj->ns) {
        int i = 0; /* compare and transform ns separator in place */
        for ( ; i < obj->ns_len; ++i) {
            if (*cns != *ccl) {
                return 0;
            }
            if (*ccl == SPLCLASSLD_NS_SEPARATOR) {
                *ccl = PHP_DIR_SEPARATOR;
            }
            ++ccl;
            ++cns;
        }
        if (*ccl != SPLCLASSLD_NS_SEPARATOR) {
            return 0;
        }
        *ccl = PHP_DIR_SEPARATOR;
    }
    
    /* transform '_' in the class name from right to left */
    for ( ; cur != ccl; --cur) {
        if (*cur == SPLCLASSLD_NS_SEPARATOR) {
            break;
        }
        if (*cur == '_') {
            *cur = PHP_DIR_SEPARATOR;
        }
    }
    
    /* tranform the remaining ns separator from right to left */
    for ( ; cur != ccl; --cur) {
        if (*cur == SPLCLASSLD_NS_SEPARATOR) {
            *cur = PHP_DIR_SEPARATOR;
        }
    }
    
    if (obj->inc_path) {  
        len = obj->inc_path_len + 1 + cl_len + obj->file_ext_len + 1;
        if (len > MAXPATHLEN) {
            return 0;
        }
        /* sprintf: we know the len */
        sprintf(filename, "%s%c%s%s", obj->inc_path, PHP_DIR_SEPARATOR, cl, obj->file_ext);
        return len;
    }
    len = cl_len + obj->file_ext_len + 1;
    if (len > MAXPATHLEN) {
        return 0;
    }
    /* sprintf: we know the len */
    sprintf(filename, "%s%s", cl, obj->file_ext);
    return len;
}


/* {{{ proto bool SplClassLoader::loadClass(string $class_name)
   Loads the given class or interface */
PHP_METHOD(SplClassLoader, loadClass)
{
    char* cl;
    int   cl_len = 0;
    char  filename[MAXPATHLEN];
    int   len = 0;
    splclassloader_object* obj;
    
    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s/", &cl, &cl_len) /* zval separation is required */
        || !cl_len) {
        RETURN_FALSE;
    }
   
    obj = (splclassloader_object*) zend_object_store_get_object(getThis() TSRMLS_CC);
    
    len = get_filename(obj, cl, cl_len, filename TSRMLS_CC);
    if (len) {
        zend_file_handle fh;
        fh.filename = filename;
        fh.opened_path = NULL;
        fh.free_filename = 0;
        fh.type = ZEND_HANDLE_FILENAME;
        zend_execute_scripts(ZEND_INCLUDE TSRMLS_CC, NULL, 1, &fh);
        RETURN_TRUE;
    }
    RETURN_FALSE;
} /* }}} */


/* {{{ proto bool SplClassLoader::setIncludePath(string $include_path)
   Sets the base include path for all class files in the namespace of this class loader */
PHP_METHOD(SplClassLoader, setIncludePath)
{
    char* inc_path;
    int   inc_path_len;
    splclassloader_object* obj;
    
    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &inc_path, &inc_path_len)) {
        RETURN_FALSE;
    }
    
    obj = (splclassloader_object*) zend_object_store_get_object(getThis() TSRMLS_CC);
    
    if (obj->inc_path) {
        efree(obj->inc_path);
        obj->inc_path = NULL;
    }
    if (inc_path_len) {
        obj->inc_path = estrndup(inc_path, inc_path_len);    
    }
    obj->inc_path_len = inc_path_len;
    RETURN_TRUE;
} /* }}} */


/* {{{ proto string SplClassLoader::getIncludePath()
   Gets the base include path for all class files in the namespace of this class loader */
PHP_METHOD(SplClassLoader, getIncludePath)
{
    splclassloader_object* obj = (splclassloader_object*) zend_object_store_get_object(getThis() TSRMLS_CC);
    
    if (obj->inc_path) {
        RETURN_STRINGL(obj->inc_path, obj->inc_path_len, 1);
    }
    RETURN_EMPTY_STRING();
} /* }}} */


/* {{{ proto bool SplClassLoader::setFileExtension(string $file_extension)
   Sets the file extension of class files in the namespace of this class loader */
PHP_METHOD(SplClassLoader, setFileExtension)
{
    char* ext;
    int   ext_len;
    splclassloader_object* obj;
    
    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &ext, &ext_len)) {
        RETURN_FALSE;
    }
    
    obj = (splclassloader_object*) zend_object_store_get_object(getThis() TSRMLS_CC);
    
    if (obj->file_ext) {
        efree(obj->file_ext);
        obj->file_ext = NULL;
    }
    if (ext_len) {
        obj->file_ext = estrndup(ext, ext_len);    
    }
    obj->file_ext_len = ext_len;
    RETURN_TRUE;
} /* }}} */


/* {{{ proto string SplClassLoader::getFileExtension()
   Gets the file extension of class files in the namespace of this class loader */
PHP_METHOD(SplClassLoader, getFileExtension)
{
    splclassloader_object* obj = (splclassloader_object*) zend_object_store_get_object(getThis() TSRMLS_CC);
    
    if (obj->file_ext) {
        RETURN_STRINGL(obj->file_ext, obj->file_ext_len, 1);
    }
    RETURN_EMPTY_STRING();
} /* }}} */


/* {{{ proto mixed SplClassLoader::getPath(string $class_name)
   Gets the path for the given class. Does not load anything. This is just a handy method */
PHP_METHOD(SplClassLoader, getPath)
{
    char* cl;
    int   cl_len = 0;
    char  filename[MAXPATHLEN];
    int   len = 0;
    splclassloader_object* obj;
    
    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &cl, &cl_len)
        || !cl_len) {
        RETURN_FALSE;
    }
    
    obj = (splclassloader_object*) zend_object_store_get_object(getThis() TSRMLS_CC);
    
    len = get_filename(obj, cl, cl_len, filename TSRMLS_CC);
    if (len) {
        RETURN_STRINGL(filename, --len, 1); /* cl_len > 0 and len includes '\0' */
    }
    RETURN_FALSE;
} /* }}} */


zend_function_entry splclassloader_methods[] = {
    PHP_ME(SplClassLoader, __construct, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_ME(SplClassLoader, register, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(SplClassLoader, unregister, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(SplClassLoader, loadClass, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(SplClassLoader, setIncludePath, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(SplClassLoader, getIncludePath, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(SplClassLoader, setFileExtension, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(SplClassLoader, getFileExtension, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(SplClassLoader, getPath, NULL, ZEND_ACC_PUBLIC)
    { NULL, NULL, NULL }
};


/*const zend_function_entry splclassloader_functions[] = {
    {NULL, NULL, NULL}
};*/


PHP_MINIT_FUNCTION(splclassloader)
{   
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, "SplClassLoader", splclassloader_methods);
    splclassloader_ce = zend_register_internal_class(&ce TSRMLS_CC);
    splclassloader_ce->create_object = splclassloader_create_object;

    memcpy(&splclassloader_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
    splclassloader_object_handlers.clone_obj = NULL; /* no cloning! */

    return SUCCESS;
}

PHP_MINFO_FUNCTION(splclassloader)
{
    php_info_print_table_start ();
    php_info_print_table_header(2, "SplClassLoader support", "enabled");
    php_info_print_table_row   (2, "Conformance", "PSR-0");
    php_info_print_table_end   ();
}


zend_module_entry splclassloader_module_entry = {
    STANDARD_MODULE_HEADER,
    "SplClassLoader",
    NULL, /*splclassloader_functions, */
    PHP_MINIT(splclassloader),
    NULL,
    NULL,
    NULL,
    PHP_MINFO(splclassloader),
    "0.1",
    STANDARD_MODULE_PROPERTIES
};
