<?php

$dir = __DIR__.DIRECTORY_SEPARATOR.'lib';


$v1 = new SplClassLoader('vendor0', $dir);
$v1->register();

$v2 = new SplClassLoader('vendor1', $dir);
$v2->register();

$v3 = new SplClassLoader('vendor2', $dir);
$v3->register();

//$v4 = new SplClassLoader('vendor3', $dir);
//$v4->register();


$g = new SplClassLoader('', $dir);
$g->register();



include __DIR__.DIRECTORY_SEPARATOR.'batch_instances.php';

echo 'done';
