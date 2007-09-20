<?php 
$cryptinstall="./cryptographp.fct.php";
include $cryptinstall; 
?>


<html>
<?php
  if (chk_crypt($_POST['code'])) 
     echo "<a><font color='#009700'>=> OK !</font></a>" ;
     else echo "<a><font color='#FF0000'>=> Error</font></a>" ;
?>
</html>
