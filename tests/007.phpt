--TEST--
splclassloader ns autoload test
--INI--
include_path=.
--SKIPIF--
<?php if (!extension_loaded('splclassloader')) die('splclassloader not loaded'); ?>
--FILE--
<?php 

$s = new SplClassLoader('ns1', __DIR__);
$s->register();

$sc = new SplClassLoader();
$sc->setFileExtension('.class.php');
$sc->register();


$d = new \ns1\Dummy1;
$d = new \ns1\ns2\Dummy2;
$d = new Dummyc;


var_dump(class_exists('ns1\Dummy1', false));
var_dump(class_exists('ns1\ns2\Dummy2', false));
var_dump(class_exists('Dummyc', false));

?>
--EXPECTF--
bool(true)
bool(true)
bool(true)
