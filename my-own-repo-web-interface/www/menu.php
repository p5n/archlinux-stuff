<FORM action='<?php echo $_SERVER['REQUEST_URI']; ?>' method=GET>
<TABLE class="menu-table">
    <TR><TD class="qlinks">
	<A HREF='search.php?repo=<?php echo $repoidx; ?>'>Search</A> :
	<A HREF='recent.php?repo=<?php echo $repoidx; ?>'>Recent</A> (
	    <A HREF='recent.php?repo=<?php echo $repoidx; ?>&a=1'>Added</A> |
	    <A HREF='recent.php?repo=<?php echo $repoidx; ?>&u=1'>Updated</A> |
	    <A HREF='recent.php?repo=<?php echo $repoidx; ?>&o=1'>Outdated</A> |
	    <A HREF='recent.php?repo=<?php echo $repoidx; ?>&d=1'>Deleted</A> )
    <TD class="qswitch">
	Quick switch: <SELECT name=repo class='input-select'>
    <?php
    for($i = 0; $i < count($repos); $i++)
    {
	if($i == $repoidx)
	{
	    echo "<OPTION value=$i selected>".$repos[$i]['name']."</OPTION>";
	}
	else
	{
	    echo "<OPTION value=$i>".$repos[$i]['name']."</OPTION>";
	}
    }
    ?>
    </SELECT>

    <?php
    foreach($_GET as $key => $val)
    {
	if($key != "repo")
	{
	    echo "<INPUT value=\"$val\" name=\"$key\" type=hidden>\n";
	}
    }
    ?>

    <INPUT class="input-submit" value="Switch" type=submit>
    </SPAN>
</TABLE>
</FORM>
<HR align=left>
