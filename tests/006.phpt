--TEST--
splclassloader more register test
--INI--
include_path=.
--SKIPIF--
<?php if (!extension_loaded('splclassloader')) die('splclassloader not loaded'); ?>
--FILE--
<?php 

var_dump(spl_autoload_functions());

$s = new SplClassLoader();
var_dump($s->register());

$d = new Dummy;

var_dump($s->unregister());

var_dump(spl_autoload_functions());

var_dump($s->unregister());

$d = new Dummyb;


?>
--EXPECTF--
bool(false)
bool(true)
bool(true)
array(0) {
}
bool(false)

Fatal error: Class 'Dummyb' not found in %ssplclassloader/tests/%s
