<?php
include 'config.php';
include 'common.php';

header('Content-type: application/rss+xml; charset=utf-8');
echo "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
?>


<rss version="2.0">
	<channel>
	<title><?php echo $repos[$repoidx]['name']; ?></title>
	<link><?php echo "http://".$_SERVER['HTTP_HOST'].substr($_SERVER['SCRIPT_NAME'], 0, -14)."search.php?repo=$repoidx"; ?></link>
	<language>en</language>
	<pubDate><?php echo date('r'); ?></pubDate>
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

    $upd = empty($_GET['u']) ? 0 : $_GET['u'];
    $del = empty($_GET['d']) ? 0 : $_GET['d'];
    $add = empty($_GET['a']) ? 0 : $_GET['a'];
    $out = empty($_GET['o']) ? 0 : $_GET['o'];

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
	else if($op == 4)
	{
	    $op = "OUT";
	}
	echo "	<item>\n<guid>$pkgname</guid>\n";
	echo "		<title>".$op.":".$pkgname."-".$pkgver."</title>\n";
	echo "		<link>http://".$_SERVER['HTTP_HOST'].substr($_SERVER['SCRIPT_NAME'], 0, -14)."detail.php?n=$pkgname&amp;repo=$repoidx</link>\n";
	echo "		<pubDate>".date('r', $row[3])."</pubDate>\n";
	echo "		<description>".$pkgdesc."</description>\n";
	echo "	</item>\n";
    }
?>
</channel>
</rss>
