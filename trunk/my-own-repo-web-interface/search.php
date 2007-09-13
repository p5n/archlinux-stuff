<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN" "http://www.w3.org/TR/REC-html40/loose.dtd">
<HTML>
<HEAD>
    <TITLE>Packages<TITLE>
    <META http-equiv="Content-Type" content="text/html; charset=UTF-8"/>
</HEAD>
<BODY>
<TABLE border=1>
<TR><TH>Name<TH>Version<TH>Description</TR>
<?php
    define(PDO_URL, "sqlite:/tmp/test.db");

    try
    {
	$dbHandle = new PDO(PDO_URL);
    }
    catch( PDOException $exception )
    {
	die($exception->getMessage());
    }

    foreach ($dbHandle->query('SELECT id, pkgname, pkgver, pkgdesc, url, builddate, packager, size, arch, license, depend, filelist from packages') as $row)
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
	$filelist = $row[11];
	echo "<TR><TD>$pkgname<TD>$pkgver<TD>$pkgdesc</TR>\n";
    }
?>
</TABLE>
</BODY>
</HTML>
