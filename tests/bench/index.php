<?php

ini_set('display_errors', 1);
error_reporting(E_ALL);

if (function_exists("date_default_timezone_set")) {
	date_default_timezone_set("America/Sao_Paulo");
}

echo shell_exec('php test-class.php');
echo "\n";
echo shell_exec('php test-native.php');