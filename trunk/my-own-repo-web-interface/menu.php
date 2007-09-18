<?php include 'config.php'; ?>
<DIV>
<SPAN align=left>
<A HREF='search.php'>Search</A> :
<A HREF='recent.php'>Recent</A> (
    <A HREF='recent.php?a=1'>Added</A> |
    <A HREF='recent.php?u=1'>Updated</A> |
    <A HREF='recent.php?d=1'>Deleted</A>
)
</SPAN>
<SPAN align=right>
    <FORM action='search.php' method=GET>
    Quick switch: <SELECT name=repo>
    <?php
    $repoidx = $_GET['repo'];
    if(empty($repoidx))
    {
	$repoidx = 0;
    }
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
    <INPUT class="input-submit" value="Switch" type=submit>
    </FORM>
</SPAN>
</DIV>
<HR width='70%'>
