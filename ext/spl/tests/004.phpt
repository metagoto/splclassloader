--TEST--
splclassloader file extension test
--SKIPIF--
<?php if (!extension_loaded('splclassloader')) die('splclassloader not loaded'); ?>
--FILE--
<?php 

$s = new SplClassLoader();
echo $s->getFileExtension()."\n";
echo $s->setFileExtension(".class.php")."\n";
echo $s->getFileExtension()."\n";

echo $s->setFileExtension('')."\n";
var_dump($s->getFileExtension());

$s = new SplClassLoader();
echo $s->getFileExtension()."\n";
var_dump($s->setFileExtension());

?>
--EXPECTF--
.php
1
.class.php
1
string(0) ""
.php

Warning: SplClassLoader::setFileExtension() expects exactly 1 parameter, 0 given in %s on line %s
bool(false)
