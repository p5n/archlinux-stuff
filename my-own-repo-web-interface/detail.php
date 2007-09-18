<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN" "http://www.w3.org/TR/REC-html40/loose.dtd">
<HTML>
<HEAD>
    <TITLE>Packages<TITLE>
    <META http-equiv="Content-Type" content="text/html; charset=UTF-8"/>
    <LINK rel="stylesheet" type="text/css" href="theme.css" media="screen">
</HEAD>
<BODY>

<?php include 'menu.php'; ?>

<?php

if(empty($_SERVER['HTTP_REFERER']))
{
    echo "<P><A HREF='search.php'>&lt;&lt;back</A>\n";
}
else
{
    echo "<P><A HREF='".$_SERVER['HTTP_REFERER']."'>&lt;&lt;back</A>\n";
}

?>

<?php
    define(PDO_URL, $REPODB);

    $pkgid =  urlencode($_GET['id']);
    $pkgname =  urlencode($_GET['n']);

    try
    {
	$dbHandle = new PDO(PDO_URL);
    }
    catch( PDOException $exception )
    {
	die($exception->getMessage());
    }

    if(empty($pkgid))
    {
	$wherecond = "pkgname='$pkgname'";
    }
    else
    {
	$wherecond = "id=$pkgid";
    }

    foreach ($dbHandle->query("SELECT id, pkgname, pkgver, pkgdesc, url, builddate, packager, size, arch, license, depend, backup, filelist, lastupdated from packages WHERE $wherecond") as $row)
    {
	$pkgname = $row[1];
	$pkgver = $row[2];
	$pkgdesc = $row[3];
	$url = $row[4];
	$builddate = $row[5];
	$packager = $row[6];
	$size = $row[7];
	$arch = $row[8];
	$license = $row[9];
	$depend = $row[10];
	$backup = $row[11];
	$filelist = $row[12];
	$lastupdated = date("Y.m.d H.i.s", $row[13]);

	echo "<P><B>Name:</B> $pkgname\n";
	echo "<P><B>Version:</B> $pkgver\n";
	echo "<P><B>Description:</B> $pkgdesc\n";
	echo "<P><B>Home page:</B> <A HREF='$url'>$url</A>\n";
	echo "<P><B>Build date:</B> $builddate\n";
	echo "<P><B>Packager:</B> $packager\n";
	echo "<P><B>Size:</B> $size\n";
	echo "<P><B>Arch:</B> $arch\n";
	echo "<P><B>License:</B> $license\n";
	echo "<P><B>Depends on:</B> $depend\n";
	echo "<P><B>Backup files:</B> $backup\n";
	echo "<P><B>File list:</B> <PRE>$filelist</PRE>\n";
	echo "<P><B>Last updated in DB:</B> $lastupdated\n";
    }
?>
</BODY>
</HTML>
