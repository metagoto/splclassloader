<?php

set_include_path(
   __DIR__.PATH_SEPARATOR
  .__DIR__.DIRECTORY_SEPARATOR.'lib'.PATH_SEPARATOR
  .get_include_path()
);


$g = new SplClassLoader();
$g->register();


include 'batch_instances.php';

echo 'done';
