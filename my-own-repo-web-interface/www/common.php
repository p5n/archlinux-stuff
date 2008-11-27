<?php
$repoidx = 0;
if(!empty($_GET['repo'])) $repoidx = $_GET['repo'];
$REPODB = $repos[$repoidx]['db'];

$OP_ADD = 1;
$OP_DEL = 2;
$OP_UPD = 3;
$OP_OFD = 4; // out of dated
?>
