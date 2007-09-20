<?php 
$cryptinstall="./cryptographp.fct.php";
include $cryptinstall; 
?>


<html>
<div align="center">
<form action="verifier.php?<?PHP echo SID; ?>" method="post">
<table cellpadding=1>
  <tr><td align="center"><?php dsp_crypt("bluesky.cfg.php",1); ?></td></tr>
  <tr><td align="center"><input type="text" name="code"></td></tr>
  <tr><td align="center"><input type="submit" name="submit" value="Send"></td></tr>
</table>
</form>

</div>
</html>
