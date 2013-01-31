<?php

function getmicrotime() {
    $t = gettimeofday();
    return ($t['sec'] + $t['usec'] / 1000000);
}

function start_test() {
    ob_start();
    return getmicrotime();
}

function end_test($start, $name) {
    global $total;
    $end = getmicrotime();
    ob_end_clean();
    $total += $end-$start;
    $num = number_format($end-$start,3);
    $len = (24 - strlen($name) - strlen($num));
    $pad = str_repeat(" ", ($len < 0 ? 0 : $len) );

    echo $name.' '.$pad.$num."\n";
    ob_start();
    return getmicrotime();
}