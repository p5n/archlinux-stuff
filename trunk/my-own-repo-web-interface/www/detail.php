<?php
include 'config.php';
include 'common.php';

    $unflag = 0;
    $pkgid = "";
    $pkgname = "";
    if(!empty($_GET['id'])) $pkgid =  urlencode($_GET['id']);
    if(!empty($_GET['n'])) $pkgname =  urlencode($_GET['n']);
    if(!empty($_GET['nver'])) $nver =  urlencode($_GET['nver']);
    if(!empty($_GET['msg'])) $msg =  urlencode($_GET['msg']);
    if(!empty($_GET['unflag'])) $unflag =  urlencode($_GET['unflag']);

    try
    {
	$dbHandle = new PDO($REPODB);
    }
    catch( PDOException $exception )
    {
	die($exception->getMessage());
    }

    if($unflag == 1)
    {
        $timestamp = time();
        $dbHandle->exec("UPDATE packages SET newver='' WHERE id='$pkgid'") or die($dbHandle->errorInfo());
        $dbHandle->exec("INSERT INTO log VALUES (NULL, $timestamp, $OP_UPD, '$pkgname')") or die($dbHandle->errorInfo());
	if($enableMailNotification)
	{
	    mail($repoOwnerEmail, "MY-OWN-REPO: $pkgname(unflagged)", "Package $pkgname unflagged");
	}
	header("Location: detail.php?repo=$repoidx&n=$pkgname&id=$pkgid");
	exit(0);
    }

    if(!empty($msg) || !empty($nver))
    {
	if($enableMailNotification)
	{
	    mail($repoOwnerEmail, "MY-OWN-REPO: $pkgname($nver)", $msg);
	}
	if(!empty($nver))
	{
	    $timestamp = time();
	    $rr1 = $dbHandle->exec("UPDATE packages SET newver='$nver' WHERE pkgname='$pkgname'");
	    $rr2 = $dbHandle->exec("INSERT INTO log VALUES (NULL, $timestamp, $OP_OFD, '$pkgname')");
	}
	header("Location: detail.php?repo=$repoidx&n=$pkgname&id=$pkgid");
	exit(0);
    }

?>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN" "http://www.w3.org/TR/REC-html40/loose.dtd">
<HTML>
<HEAD>
    <TITLE>Packages</TITLE>
    <META http-equiv="Content-Type" content="text/html; charset=UTF-8"/>
    <LINK rel="stylesheet" type="text/css" href="theme.css" media="screen">
</HEAD>
<BODY>

<?php include 'menu.php'; ?>

<?php


    if(empty($pkgid))
    {
	$wherecond = "pkgname='$pkgname'";
    }
    else
    {
	$wherecond = "id=$pkgid";
    }

    foreach ($dbHandle->query("SELECT id, pkgname, pkgver, pkgdesc, url, builddate, packager, size, arch, license, depend, backup, filelist, lastupdated, newver from packages WHERE $wherecond") as $row)
    {
	$pkgid = $row[0];
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
	$newver = $row[14];

	echo "<P><SPAN class=\"field-title\">Name:</SPAN> <SPAN class=\"field-data\">$pkgname</SPAN>\n";
	echo "<P><SPAN class=\"field-title\">Version:</SPAN> <SPAN class=\"field-data\">$pkgver</SPAN>\n";
	echo "<P><SPAN class=\"field-title\">Description:</SPAN> <SPAN class=\"field-data\">$pkgdesc</SPAN>\n";
	if($url != "unknown")
	{
	    echo "<P><SPAN class=\"field-title\">Home page:</SPAN> <SPAN class=\"field-data\"><A HREF='$url'>$url</A></SPAN>\n";
	}
	else
	{
	    echo "<P><SPAN class=\"field-title\">Home page:</SPAN> <SPAN class=\"field-data\">$url</SPAN>\n";
	}
	echo "<P><SPAN class=\"field-title\">Build date:</SPAN> <SPAN class=\"field-data\">$builddate</SPAN>\n";
	echo "<P><SPAN class=\"field-title\">Packager:</SPAN> <SPAN class=\"field-data\">$packager</SPAN>\n";
	echo "<P><SPAN class=\"field-title\">Size:</SPAN> <SPAN class=\"field-data\">$size</SPAN>\n";
	echo "<P><SPAN class=\"field-title\">Arch:</SPAN> <SPAN class=\"field-data\">$arch</SPAN>\n";
	echo "<P><SPAN class=\"field-title\">License:</SPAN> <SPAN class=\"field-data\">$license</SPAN>\n";
	echo "<P><SPAN class=\"field-title\">Depends on:</SPAN> <SPAN class=\"field-data\">$depend</SPAN>\n";
	echo "<P><SPAN class=\"field-title\">Backup files:</SPAN> <SPAN class=\"field-data\">$backup</SPAN>\n";
	echo "<P><SPAN class=\"field-title\">File list:</SPAN> <SPAN class=\"field-data\"><PRE>$filelist</PRE></SPAN>\n";
	echo "<P><SPAN class=\"field-title\">Last updated in DB:</SPAN> <SPAN class=\"field-data\">$lastupdated</SPAN>\n";

	if(empty($newver))
	{
	    if($enableMarkOutOfDate)
	    {
		echo "<DIV class='feedback-form'><P><SPAN class='field-title'>Feedback form</SPAN><P><SPAN class='span-form'>";
		echo "<FORM action='detail.php' method=GET>";
		echo "<INPUT type=hidden name=n value=\"$pkgname\">";
		echo "<INPUT type=hidden name=repo value=\"$repoidx\">";
		if($enableMailNotification)
		{
		    echo "<P>Enter new version number (leave empty if you want just send message):&nbsp;<INPUT class='input-text' type=text size=10 name=nver>";
		    echo "<P>Enter message to repo owner:<BR><TEXTAREA name=msg WRAP=virtual COLS=80 ROWS=10 class='input-text'></TEXTAREA><BR>";
		    echo "<INPUT class='input-submit' type=submit name=submit value='Send or Flag out of date'>";
		}
		else
		{
		    echo "<P>Enter new version number:&nbsp;<INPUT class='input-text' type=text size=10 name=nver>";
		    echo "<INPUT class='input-submit' type=submit name=submit value='Flag out of date'>";
		}
		echo "</FORM></SPAN></DIV>";
	    }
	}
	else
	{
	    echo "<P class=red>Out of dated: new version: $newver\n";
	    if($enableMarkOutOfDate)
	    {
		echo "<P class=red><A HREF='detail.php?n=$pkgname&id=$pkgid&unflag=1&repo=$repoidx'>Unflag out of date</A>\n";
	    }
	}
    }
?>
</BODY>
</HTML>
