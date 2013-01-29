<?php

set_include_path(
   __DIR__.PATH_SEPARATOR
  .__DIR__.DIRECTORY_SEPARATOR.'lib'.PATH_SEPARATOR
  .get_include_path()
);


$v1 = new SplClassLoader('vendor0');
$v1->register();

$v2 = new SplClassLoader('vendor1');
$v2->register();

$v3 = new SplClassLoader('vendor2');
$v3->register();

//$v4 = new SplClassLoader('vendor3');
//$v4->register();


$g = new SplClassLoader();
$g->register();


include 'batch_instances.php';

echo 'done';
