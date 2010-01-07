<?php

set_include_path(
   __DIR__.PATH_SEPARATOR
  .__DIR__.DIRECTORY_SEPARATOR.'lib'.PATH_SEPARATOR
  .get_include_path()
);

include 'SplClassLoaderUser.php';


$v1 = new SplClassLoaderUser('vendor0');
$v1->register();

$v2 = new SplClassLoaderUser('vendor1'); 
$v2->register();

$v3 = new SplClassLoaderUser('vendor2');
$v3->register();

//$v4 = new SplClassLoaderUser('vendor3');
//$v4->register();


$g = new SplClassLoader();
$g->register();


include 'batch_instances.php';

echo 'done';
