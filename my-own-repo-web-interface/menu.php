<?php
include 'config.php';
$repoidx = $_GET['repo'];
if(empty($repoidx))
{
    $repoidx = 0;
}
?>
<DIV><SPAN>
<A HREF='search.php?repo=<?php echo $repoidx; ?>'>Search</A> :
<A HREF='recent.php?repo=<?php echo $repoidx; ?>'>Recent</A> (
    <A HREF='recent.php?repo=<?php echo $repoidx; ?>&a=1'>Added</A> |
    <A HREF='recent.php?repo=<?php echo $repoidx; ?>&u=1'>Updated</A> |
    <A HREF='recent.php?repo=<?php echo $repoidx; ?>&d=1'>Deleted</A>
)
</SPAN><SPAN style="float: right; padding: 10px; border-width: 1px; border-color: #000000">
    <FORM action='<?php echo $_SERVER['REQUEST_URI']; ?>' method=GET>
    Quick switch: <SELECT name=repo>
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
    $REPODB = $repos[$repoidx]['db'];
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
    </FORM>
</SPAN>
</DIV>
<HR align=left>
