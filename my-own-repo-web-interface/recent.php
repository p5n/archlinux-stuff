<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN" "http://www.w3.org/TR/REC-html40/loose.dtd">
<HTML>
<HEAD>
    <TITLE>Packages<TITLE>
    <META http-equiv="Content-Type" content="text/html; charset=UTF-8"/>
</HEAD>
<BODY>

<?php include 'menu.php'; ?>

<?php
    // config
    define(PDO_URL, $REPODB);

    // init
    try
    {
	$dbHandle = new PDO(PDO_URL);
    }
    catch( PDOException $exception )
    {
	die($exception->getMessage());
    }

    $upd = $_GET['u'];
    $del = $_GET['d'];
    $add = $_GET['a'];

    $where = "";
    $started = "";

    if(!empty($add))
    {
	$where .= "op = 1";
	$started = " OR ";
    }

    if(!empty($del))
    {
	$where .= $started."op = 2";
	$started = " OR ";
    }

    if(!empty($upd))
    {
	$where .= $started."op = 3";
	$started = " OR ";
    }

    if(!empty($where))
    {
	$where = "AND ($where)";
    }
?>

<TABLE border=1 width='100%'>
<TR><TH>Operation<TH>Name<TH>Version<TH>Description<TH>Last updated</TR>
<?php
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
	echo "<TR><TD>$op<TD><A HREF='detail.php?id=$pkgid'>$pkgname</A><TD>$pkgver<TD>$pkgdesc<TD>$lastupdated</TR>\n";
    }
?>
</TABLE>
</BODY>
</HTML>
