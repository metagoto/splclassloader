/*
   +----------------------------------------------------------------------+
   | PHP Version 5                                                        |
   +----------------------------------------------------------------------+
   | Copyright (c) 1997-2009 The PHP Group                                |
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
 
#ifndef PHP_SPLCLASSLOADER_H
#define PHP_SPLCLASSLOADER_H

extern zend_module_entry splclassloader_module_entry;
#define phpext_splclassloader_ptr &splclassloader_module_entry

#ifdef ZTS
#include "TSRM.h"
#endif

#ifdef ZTS
#define SPLCLASSLD_G(v) TSRMG(splclassloader_globals_id, zend_splclassloader_globals *, v)
#else
#define SPLCLASSLD_G(v) (splclassloader_globals.v)
#endif

PHP_MINIT_FUNCTION(splclassloader);
PHP_MINFO_FUNCTION(splclassloader);

#endif /* PHP_SPLCLASSLOADER_H */
