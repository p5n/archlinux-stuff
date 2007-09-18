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

	echo "<P><SPAN class=\"field-title\">Name:</SPAN> <SPAN class=\"field-data\">$pkgname</SPAN>\n";
	echo "<P><SPAN class=\"field-title\">Version:</SPAN> <SPAN class=\"field-data\">$pkgver</SPAN>\n";
	echo "<P><SPAN class=\"field-title\">Description:</SPAN> <SPAN class=\"field-data\">$pkgdesc</SPAN>\n";
	echo "<P><SPAN class=\"field-title\">Home page:</SPAN> <SPAN class=\"field-data\"><A HREF='$url'>$url</A></SPAN>\n";
	echo "<P><SPAN class=\"field-title\">Build date:</SPAN> <SPAN class=\"field-data\">$builddate</SPAN>\n";
	echo "<P><SPAN class=\"field-title\">Packager:</SPAN> <SPAN class=\"field-data\">$packager</SPAN>\n";
	echo "<P><SPAN class=\"field-title\">Size:</SPAN> <SPAN class=\"field-data\">$size</SPAN>\n";
	echo "<P><SPAN class=\"field-title\">Arch:</SPAN> <SPAN class=\"field-data\">$arch</SPAN>\n";
	echo "<P><SPAN class=\"field-title\">License:</SPAN> <SPAN class=\"field-data\">$license</SPAN>\n";
	echo "<P><SPAN class=\"field-title\">Depends on:</SPAN> <SPAN class=\"field-data\">$depend</SPAN>\n";
	echo "<P><SPAN class=\"field-title\">Backup files:</SPAN> <SPAN class=\"field-data\">$backup</SPAN>\n";
	echo "<P><SPAN class=\"field-title\">File list:</SPAN> <SPAN class=\"field-data\"><PRE>$filelist</PRE></SPAN>\n";
	echo "<P><SPAN class=\"field-title\">Last updated in DB:</SPAN> <SPAN class=\"field-data\">$lastupdated</SPAN>\n";
    }
?>
</BODY>
</HTML>
