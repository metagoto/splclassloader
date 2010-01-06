--TEST--
splclassloader class creation test
--SKIPIF--
<?php if (!extension_loaded('splclassloader')) die('splclassloader not loaded'); ?>
--FILE--
<?php 
$s = new SplClassLoader();
var_dump($s);
?>
--EXPECT--
object(SplClassLoader)#1 (0) {
}
