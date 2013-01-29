<?php

include __DIR__.DIRECTORY_SEPARATOR.'SplClassLoaderUser.php';

$dir = __DIR__.DIRECTORY_SEPARATOR.'lib';


$v1 = new SplClassLoaderUser('vendor0', $dir);
$v1->register();

$v2 = new SplClassLoaderUser('vendor1', $dir);
$v2->register();

$v3 = new SplClassLoaderUser('vendor2', $dir);
$v3->register();

//$v4 = new SplClassLoaderUser('vendor3', $dir);
//$v4->register();


$g = new SplClassLoader('', $dir);
$g->register();


include __DIR__.DIRECTORY_SEPARATOR.'batch_instances.php';

echo 'done';
