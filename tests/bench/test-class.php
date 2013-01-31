<?php

require_once 'functions.php';

$t0 = start_test();
$startMemory = memory_get_usage();

require_once 'functions.php';
require_once 'SplClassLoader.php';

$classLoader = new SplClassLoader('Yeap', realpath('vendor'));
$classLoader->register();

$piggy = new \Yeap\Piggy();
$piggy->speak();

$cow = new \Yeap\MilkCow;
$cow->speak();

$sheep = new \Yeap\BigHornSheep;
$sheep->speak();

end_test($t0, "SplClassLoader PHP Class");
echo "Memory Usage: " . (memory_get_usage() - $startMemory) . " bytes\n";