<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN" "http://www.w3.org/TR/REC-html40/loose.dtd">
<HTML>
<HEAD>
    <TITLE>Packages<TITLE>
    <META http-equiv="Content-Type" content="text/html; charset=UTF-8"/>
    <LINK rel="stylesheet" type="text/css" href="theme.css" media="screen">
</HEAD>
<BODY>

<?php
include 'config.php';
include 'common.php';
include 'menu.php';
?>

<?php
    // init
    try
    {
	$dbHandle = new PDO($REPODB);
    }
    catch( PDOException $exception )
    {
	die($exception->getMessage());
    }

    $upd = $_GET['u'];
    $del = $_GET['d'];
    $add = $_GET['a'];
    $out = $_GET['o'];

    $where = "";
    $started = "";

    if(!empty($add))
    {
	$where .= "op = $OP_ADD";
	$started = " OR ";
    }

    if(!empty($del))
    {
	$where .= $started."op = $OP_DEL";
	$started = " OR ";
    }

    if(!empty($upd))
    {
	$where .= $started."op = $OP_UPD";
	$started = " OR ";
    }

    if(!empty($out))
    {
	$where .= $started."op = $OP_OFD";
	$started = " OR ";
    }

    if(!empty($where))
    {
	$where = "AND ($where)";
    }
?>

<TABLE class="txt-table" width='100%'>
<TR><TH>Operation<TH>Name<TH>Version<TH>Description<TH>Last updated</TR>
<?php
    $counter = 1;
    foreach ($dbHandle->query("SELECT p.pkgname, p.pkgver, p.pkgdesc, p.lastupdated, log.op from packages as p, log WHERE p.pkgname = log.pkgname $where ORDER BY log.time DESC LIMIT 10") as $row)
    {
	$pkgname = $row[0];
	$pkgver = $row[1];
	$pkgdesc = $row[2];
	$lastupdated = date("Y.m.d H.i.s", $row[3]);
	$op = $row[4];
	if($op == 1)
	{
	    $op = "ADD";
	}
	else if($op == 2)
	{
	    $op = "DEL";
	}
	else if($op == 3)
	{
	    $op = "UPD";
	}
	$rowclass = ($counter++ % 2) ? "even-row" : "odd-row";
	echo "<TR class=\"$rowclass\"><TD>$op<TD><A HREF='detail.php?n=$pkgname&repo=$repoidx'>$pkgname</A><TD>$pkgver<TD>$pkgdesc<TD>$lastupdated</TR>\n";
    }
?>
</TABLE>
</BODY>
</HTML>
