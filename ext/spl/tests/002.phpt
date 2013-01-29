--TEST--
splclassloader basic autoload test
--INI--
include_path=.
--SKIPIF--
<?php if (!extension_loaded('splclassloader')) die('splclassloader not loaded'); ?>
--FILE--
<?php 

$s = new SplClassLoader();
$s->register();

$d = new Dummy;
var_dump(class_exists('Dummy', false));

?>
--EXPECT--
bool(true)
