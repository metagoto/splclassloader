--TEST--
splclassloader include path test
--SKIPIF--
<?php if (!extension_loaded('splclassloader')) die('splclassloader not loaded'); ?>
--FILE--
<?php 

$s = new SplClassLoader();
var_dump($s->getIncludePath());
echo $s->setIncludePath('path/to/here')."\n";
echo $s->getIncludePath()."\n";

echo $s->setIncludePath('')."\n";
var_dump($s->getIncludePath());

echo $s->setIncludePath('/another/PATH')."\n";
echo $s->getIncludePath()."\n";


$s = new SplClassLoader();
var_dump($s->getIncludePath());
var_dump($s->setIncludePath());

?>
--EXPECTF--
string(0) ""
1
path/to/here
1
string(0) ""
1
/another/PATH
string(0) ""

Warning: SplClassLoader::setIncludePath() expects exactly 1 parameter, 0 given in %s on line %s
bool(false)
