--TEST--
splclassloader class creation test
--SKIPIF--
<?php if (!extension_loaded('splclassloader')) die('splclassloader not loaded'); ?>
--FILE--
<?php 
$s = new SplClassLoader();
var_dump($s);

$s = new SplClassLoader(1,2,3);
var_dump($s);
var_dump($s->getPath('Dummy'));

?>
--EXPECTF--
object(SplClassLoader)#1 (0) {
}

Warning: SplClassLoader::__construct() expects at most 2 parameters, 3 given in %s on line %s
object(SplClassLoader)#2 (0) {
}
string(9) "Dummy.php"
