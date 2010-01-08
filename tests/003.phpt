--TEST--
splclassloader ns and include_path test
--SKIPIF--
<?php if (!extension_loaded('splclassloader')) die('splclassloader not loaded'); ?>
--FILE--
<?php 

$s = new SplClassLoader();
var_dump($s->getPath('Dummy'));
var_dump($s->getPath('ab\Dummy'));
var_dump($s->getPath('Dum_my'));
var_dump($s->getPath('ab\cd_\D_um_my'));
var_dump($s->getPath('ab\cd_\_D_um_my'));
var_dump($s->getPath('D_ummy_'));
var_dump($s->getPath(''));

echo "\n";

$s = new SplClassLoader('', 'path/to');
var_dump($s->getPath('Dummy'));
var_dump($s->getPath('ab\Dummy'));
var_dump($s->getPath('Dum_my'));
var_dump($s->getPath('ab\cd_\D_um_my'));
var_dump($s->getPath('ab\cd_\_D_um_my'));

echo "\n";

$s = new SplClassLoader('ab', 'path/to');
var_dump($s->getPath('Dummy'));
var_dump($s->getPath('ab\Dummy'));
var_dump($s->getPath('Dum_my'));
var_dump($s->getPath('ab\cd_\D_um_my'));
var_dump($s->getPath('ab\cd_\_D_um_my'));
var_dump($s->getPath('abc\Dummy'));

echo "\n";

$s = new SplClassLoader('ab\c_d');
var_dump($s->getPath('Dummy'));
var_dump($s->getPath('ab\c_d\Dummy'));
var_dump($s->getPath('Dum_my'));
var_dump($s->getPath('ab\c_d\D_um_my'));
var_dump($s->getPath('ab\c_d\ef\_D_um_my'));
var_dump($s->getPath('ab\c_d_\Dummy'));
var_dump($s->getPath('ab\c\Dummy'));

echo "\n";

$s = new SplClassLoader('ab\cd\ef');
var_dump($s->getPath('Dummy'));
var_dump($s->getPath('ab\cd\ef'));
var_dump($s->getPath('ab\cd\ef\rory'));

echo "\n";

$c = new SplClassLoader('', '');
var_dump($c->getPath('Dummy'));
var_dump($c->getPath('ab\cd\ef'));
var_dump($c->getPath('ab\cd\ef\rory\gallag_her'));
var_dump($c->getPath('D'));
var_dump($c->getPath(''));
var_dump($c->getPath('/'));
var_dump($c->getPath('_'));
var_dump($c->getPath(42));
var_dump($c->getPath());


?>
--EXPECTF--
string(9) "Dummy.php"
string(12) "ab/Dummy.php"
string(10) "Dum/my.php"
string(18) "ab/cd_/D/um/my.php"
string(19) "ab/cd_//D/um/my.php"
string(11) "D/ummy/.php"
bool(false)

string(17) "path/to/Dummy.php"
string(20) "path/to/ab/Dummy.php"
string(18) "path/to/Dum/my.php"
string(26) "path/to/ab/cd_/D/um/my.php"
string(27) "path/to/ab/cd_//D/um/my.php"

bool(false)
string(20) "path/to/ab/Dummy.php"
bool(false)
string(26) "path/to/ab/cd_/D/um/my.php"
string(27) "path/to/ab/cd_//D/um/my.php"
bool(false)

bool(false)
string(16) "ab/c_d/Dummy.php"
bool(false)
string(18) "ab/c_d/D/um/my.php"
string(22) "ab/c_d/ef//D/um/my.php"
bool(false)
bool(false)

bool(false)
bool(false)
string(17) "ab/cd/ef/rory.php"

string(9) "Dummy.php"
string(12) "ab/cd/ef.php"
string(28) "ab/cd/ef/rory/gallag/her.php"
string(5) "D.php"
bool(false)
string(5) "/.php"
string(5) "_.php"
string(6) "42.php"

Warning: SplClassLoader::getPath() expects exactly 1 parameter, 0 given in %s on line %s
bool(false)
