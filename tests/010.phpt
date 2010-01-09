--TEST--
splclassloader more ns autoload test
--INI--
include_path=.
--SKIPIF--
<?php if (!extension_loaded('splclassloader')) die('splclassloader not loaded'); ?>
--FILE--
<?php 


$x = new SplClassLoader('ns_1b\ns2b_');
$x->register();

$b = new SplClassLoader('ns_1b');
$b->register();

$a = new SplClassLoader('ns1');
$a->register();

$z = new SplClassLoader('');
$z->register();


$d = new Dummy;

$d = new \ns1\ns2\Dummy2;

$d = new \ns_1b\ns2b_\Dummy2b;

$d = new \ns_1b\NS2C_NS3B;

$d = new \ns_1b\NS2C_NS3B_Name;

?>
--EXPECT--
