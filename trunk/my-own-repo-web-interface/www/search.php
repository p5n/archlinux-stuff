<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN" "http://www.w3.org/TR/REC-html40/loose.dtd">
<HTML>
<HEAD>
    <TITLE>Packages</TITLE>
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

    $keyword = "";
    if(!empty($_GET['k'])) $keyword = urlencode($_GET['k']);

    $where = "";
    if(!empty($_GET['s'])) $where =  urlencode($_GET['s']);

    $page = 0;
    if(!empty($_GET['p'])) $page =  urlencode($_GET['p']);

    $perpage = 100;
    if(!empty($_GET['pp'])) $perpage =  urlencode($_GET['pp']);

    if(empty($perpage))
    {
	$perpage = 100;
    }
    if($perpage < 2)
    {
	$perpage = 2;
    }
    if(empty($page))
    {
	$page = 0;
    }

    $wherecond = "";
    if($where == "n")
    {
	$wherecond = "WHERE pkgname LIKE '%$keyword%'";
    }
    else if($where == "d")
    {
	$wherecond = "WHERE pkgdesc LIKE '%$keyword%'";
    }
    else if( ($where == "dn") || ($where == "nd") )
    {
	$wherecond = "WHERE pkgname LIKE '%$keyword%' OR pkgdesc LIKE '%$keyword%'";
    }

    // total count
    $total_count = 0;
    foreach ($dbHandle->query("SELECT count(*) from packages $wherecond") as $row)
    {
	$total_count = $row[0];
    }

    if($total_count < $page*$perpage)
    {
	$page = 0;
    }

    // page
    $pagecond = ($page*$perpage).",".$perpage;
?>

<FORM action='search.php' method=GET>
<TABLE class="dlg-table">
<TR>
<TH>Keyword: <INPUT size=48 class="input-text" type=text name=k value='<?php echo $keyword?>'>
<TH>Search in: <SELECT name=s class="input-select">
<OPTION value=n<?php if($where == "n") echo " selected"?>>name</OPTION>
<OPTION value=d<?php if($where == "d") echo " selected"?>>description</OPTION>
<OPTION value=nd<?php if($where == "dn" || $where == "nd") echo " selected"?>>name & description</OPTION>
</SELECT>

<TH>Page: <SELECT name=p class="input-select">
<?php
for($i=0; $i<=$total_count/$perpage; $i++)
{
    if($page == $i)
    {
	echo "<OPTION value=$i selected>$i</OPTION>\n";
    }
    else
    {
	echo "<OPTION value=$i>$i</OPTION>\n";
    }
}
?>
</SELECT>
<TH>Per page: <INPUT size=4 class="input-text" type=text name=pp value='<?php echo $perpage?>'>

<TH><INPUT class="input-submit" value="Search" type=submit>
</TR>
</TABLE>
<INPUT type=hidden name=repo value='<?php echo $repoidx; ?>'>
</FORM>

<TABLE class="txt-table" width='100%'>
<TR><TH>Name<TH>Version<TH>Description<TH>Last updated</TR>
<?php
    $counter = 1;
    foreach ($dbHandle->query("SELECT id, pkgname, pkgver, pkgdesc, url, builddate, packager, size, arch, license, depend, backup, filelist, lastupdated, newver from packages $wherecond ORDER BY pkgname LIMIT $pagecond") as $row)
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

	$rowclass = ($counter++ % 2) ? "even-row" : "odd-row";
	if(!empty($newver))
	{
	    $rowclass="red-row";
	}
	echo "<TR class=\"$rowclass\"><TD><A HREF='detail.php?n=$pkgname&repo=$repoidx'>$pkgname</A><TD>$pkgver<TD>$pkgdesc<TD>$lastupdated</TR>\n";
    }
?>
</TABLE>
</BODY>
</HTML>
