<?php

// id room region order lifecnt ccm_type cmope fval

$c = "/home/staff/horimoto/bin/a1.py 1 2 3 4 10 InAirTemp 5 15.0";
exec($c,$r);

echo "<pre>\n";
var_dump($r);

?>
