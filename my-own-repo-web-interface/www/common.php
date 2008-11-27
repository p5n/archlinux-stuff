<?php
$repoidx = $_GET['repo'];
if(empty($repoidx))
{
    $repoidx = 0;
}
$REPODB = $repos[$repoidx]['db'];

$OP_ADD = 1;
$OP_DEL = 2;
$OP_UPD = 3;
$OP_OFD = 4; // out of dated
?>
