--TEST--
oci_set_prefetch()
--SKIPIF--
<?php if (!extension_loaded('oci8')) die("skip no oci8 extension"); ?>
--FILE--
<?php

require(dirname(__FILE__)."/connect.inc");

// Initialize

$stmtarray = array(
    "drop table prefetch_tab",
    "create table prefetch_tab (id number, value number)",
    "insert into prefetch_tab (id, value) values (1,1)",
    "insert into prefetch_tab (id, value) values (1,1)",
    "insert into prefetch_tab (id, value) values (1,1)",
);

foreach ($stmtarray as $stmt) {
	$s = oci_parse($c, $stmt);
	$r = @oci_execute($s);
	if (!$r) {
		$m = oci_error($s);
		if (!in_array($m['code'], array(   // ignore expected errors
                        942 // table or view does not exist
                ))) {
			echo $stmt . PHP_EOL . $m['message'] . PHP_EOL;
		}
	}
}

foreach ($stmtarray as $stmt) {
	$s = oci_parse($c, $stmt);
	oci_execute($s);
}

// Run Test

$select_sql = "select * from prefetch_tab";

if (!($s = oci_parse($c, $select_sql))) {
	die("oci_parse(select) failed!\n");
}

var_dump(oci_set_prefetch($s, 10));

if (!oci_execute($s)) {
	die("oci_execute(select) failed!\n");
}

var_dump(oci_fetch($s));
var_dump(oci_num_rows($s));

// Cleanup

$stmtarray = array(
    "drop table prefetch_tab"
);

foreach ($stmtarray as $stmt) {
	$s = oci_parse($c, $stmt);
	oci_execute($s);
}

echo "Done\n";
?>
--EXPECT--
bool(true)
bool(true)
int(1)
Done
