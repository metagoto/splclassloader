--TEST--
splclassloader clone test
--SKIPIF--
<?php if (!extension_loaded('splclassloader')) die('splclassloader not loaded'); ?>
--FILE--
<?php 
$s = new SplClassLoader();

$c = clone $s;
?>
--EXPECTF--
Fatal error: Trying to clone an uncloneable object of class SplClassLoader in %s on line %s
