--TEST--
splclassloader register test
--INI--
include_path=.
--SKIPIF--
<?php if (!extension_loaded('splclassloader')) die('splclassloader not loaded'); ?>
--FILE--
<?php 

$s = new SplClassLoader();
var_dump($s->register());

var_dump(spl_autoload_functions());

$d = new Dummy;
var_dump(class_exists('Dummy', false));

?>
--EXPECT--
bool(true)
array(1) {
  [0]=>
  array(2) {
    [0]=>
    object(SplClassLoader)#1 (0) {
    }
    [1]=>
    string(9) "loadClass"
  }
}
bool(true)

