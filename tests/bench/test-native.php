<?php

require_once 'functions.php';

if (!extension_loaded('splclassloader')) {
	dl('splclassloader.so');
}

$t0 = start_test();
$startMemory = memory_get_usage();

$classLoader = new SplClassLoader('Yeap', realpath('vendor'));
$classLoader->register();

$piggy = new \Yeap\Piggy();
$piggy->speak();

$cow = new \Yeap\MilkCow;
$cow->speak();

$sheep = new \Yeap\BigHornSheep;
$sheep->speak();

end_test($t0, "SplClassLoader PHP Native");
echo "Memory Usage: " . (memory_get_usage() - $startMemory) . " bytes\n";