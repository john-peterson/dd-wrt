--TEST--
Prefetch with REF cursor. Test different values for prefetch with oci_set_prefetch().
--SKIPIF--
<?php if (!extension_loaded('oci8')) die("skip no oci8 extension");
if (!extension_loaded('oci8')) die("skip no oci8 extension");
require(dirname(__FILE__)."/connect.inc");
ob_start();
phpinfo(INFO_MODULES);
$phpinfo = ob_get_clean();
$iv = preg_match('/Oracle .*Version => (11\.2|12\.)/', $phpinfo);
if ($iv == 1) {
	$sv = oci_server_version($c);
	$sv = preg_match('/Release 1[012]\./', $sv, $matches);
	if ($sv != 1) {
        die ("skip expected output only valid when using Oracle 10g or greater server");
	}
}
else {
    die ("skip expected output only valid when using Oracle 11.2 or greater client");
}
?>
--FILE--
<?php
require(dirname(__FILE__)."/connect.inc");

// Creates the necessary package and tables. 
$stmtarray = array(
	   "DROP TABLE refcurtest",
	   "CREATE TABLE refcurtest (c1 NUMBER, c2 VARCHAR(20))",
           "CREATE or REPLACE PACKAGE refcurpkg is
           type refcursortype is ref cursor;
           procedure open_ref_cur(cur1 out refcursortype);
           procedure fetch_ref_cur(cur1 in refcursortype, c1 out number,c2 out varchar2);
           end refcurpkg;",
          "CREATE or REPLACE PACKAGE body refcurpkg is
  	    procedure open_ref_cur(cur1 out refcursortype) is
              begin
	        open cur1 for select * from refcurtest order by c1;
	      end open_ref_cur;
  	     procedure fetch_ref_cur(cur1 in refcursortype, c1 out number,
		c2 out varchar2) is
  	      begin
	    	fetch cur1 into c1,c2;
	    end fetch_ref_cur;
         end refcurpkg;"
	);

foreach($stmtarray as $stmt) {
	$s = oci_parse($c,$stmt);
	$r = @oci_execute($s);
    if (!$r) {
		$msg = oci_error($s);
		if ($msg['code'] != 942) {
            echo $msg['message'],"\n";
		}
	}
}

// Insert 500 rows into the table.
$insert_sql = "INSERT INTO refcurtest (c1, c2) VALUES (:c1,:c2)";
if (!($s = oci_parse($c, $insert_sql))) {
    die("oci_parse(insert) failed!\n");
}

for ($i = 0; $i<=500; $i++) {
    $val2 = 'test'.$i;
    oci_bind_by_name($s,':c1',$i);
    oci_bind_by_name($s,':c2',$val2);
    if (!oci_execute($s)) {
        die("oci_execute(insert) failed!\n");
    }
}

// Various values for prefetch
$pref = array(0,1,501,499,250,12345,-12345,-1);
foreach($pref as $value) {
    echo"-----------------------------------------------\n";
    echo "Test with Prefetch value set to $value \n";
    echo"-----------------------------------------------\n";
    $cur1 = oci_new_cursor($c);
    fetch_frm_php($c,$cur1,$value);
    fetch_frm_plsql($c,$cur1);
}


// This function sets the prefetch count to the given $value and fetches one row .

function fetch_frm_php($c,$cur1,$value) {
    $sql1 = "begin refcurpkg.open_ref_cur(:cur1); end;";
    $s1 = oci_parse($c,$sql1);
    if (!oci_bind_by_name($s1,":cur1",$cur1,-1,SQLT_RSET)) {
        die("oci_bind_by_name(sql1) failed!\n");
    }
    oci_execute($s1);
    oci_set_prefetch($cur1,$value);
    oci_execute($cur1);
    echo "Fetch Row  from PHP\n";
    var_dump(oci_fetch_row($cur1));
}
 
// This function calls the fetch_ref_cur procedure to get the values from the REF cur.

function fetch_frm_plsql($c,$cur1) {
    $sql2 = "begin refcurpkg.fetch_ref_cur(:curs1,:c1,:c2); end;";
    $s2 = oci_parse($c,$sql2);
    if (!oci_bind_by_name($s2,":curs1",$cur1,-1,SQLT_RSET)) {
        die("oci_bind_by_name(sql2) failed!\n");
    }
    if (!oci_bind_by_name($s2,":c1",$c1,SQLT_INT)) {
        die("oci_bind_by_name(sql2) failed!\n");
    }
    if (!oci_bind_by_name($s2,":c2",$c2,SQLT_AFC)) {
        die("oci_bind_by_name(sql2) failed!\n");
    }
    oci_execute($s2);
    echo "Fetch Row  from PL/SQL\n";
    var_dump($c1);
    var_dump($c2);
}

// Clean up  here

$stmtarray = array(
    "drop package refcurpkg",
    "drop table refcurtest"
);

foreach($stmtarray as $stmt) {
    $s = oci_parse($c,$stmt);
    $r = @oci_execute($s);
    if (!$r) {
        $msg = oci_error($s);
        echo $msg['message'],"\n";
    }
}
oci_close($c);
echo "Done\n";
?>
--EXPECTF--
-----------------------------------------------
Test with Prefetch value set to 0 
-----------------------------------------------
Fetch Row  from PHP
array(2) {
  [0]=>
  %unicode|string%(%d) "0"
  [1]=>
  %unicode|string%(%d) "test0"
}
Fetch Row  from PL/SQL
%unicode|string%(%d) "1"
%unicode|string%(%d) "test1"
-----------------------------------------------
Test with Prefetch value set to 1 
-----------------------------------------------
Fetch Row  from PHP
array(2) {
  [0]=>
  %unicode|string%(%d) "0"
  [1]=>
  %unicode|string%(%d) "test0"
}
Fetch Row  from PL/SQL
%unicode|string%(%d) "2"
%unicode|string%(%d) "test2"
-----------------------------------------------
Test with Prefetch value set to 501 
-----------------------------------------------
Fetch Row  from PHP
array(2) {
  [0]=>
  %unicode|string%(%d) "0"
  [1]=>
  %unicode|string%(%d) "test0"
}

Warning: oci_execute(): ORA-01002: %s
ORA-06512: at "%s.REFCURPKG", line %d
ORA-06512: at line %d in %s on line %d
Fetch Row  from PL/SQL
NULL
NULL
-----------------------------------------------
Test with Prefetch value set to 499 
-----------------------------------------------
Fetch Row  from PHP
array(2) {
  [0]=>
  %unicode|string%(%d) "0"
  [1]=>
  %unicode|string%(%d) "test0"
}
Fetch Row  from PL/SQL
%unicode|string%(%d) "500"
%unicode|string%(%d) "test500"
-----------------------------------------------
Test with Prefetch value set to 250 
-----------------------------------------------
Fetch Row  from PHP
array(2) {
  [0]=>
  %unicode|string%(%d) "0"
  [1]=>
  %unicode|string%(%d) "test0"
}
Fetch Row  from PL/SQL
%unicode|string%(%d) "251"
%unicode|string%(%d) "test251"
-----------------------------------------------
Test with Prefetch value set to 12345 
-----------------------------------------------
Fetch Row  from PHP
array(2) {
  [0]=>
  %unicode|string%(%d) "0"
  [1]=>
  %unicode|string%(%d) "test0"
}

Warning: oci_execute(): ORA-01002: %s
ORA-06512: at "%s.REFCURPKG", line %d
ORA-06512: at line %d in %s on line %d
Fetch Row  from PL/SQL
NULL
NULL
-----------------------------------------------
Test with Prefetch value set to -12345 
-----------------------------------------------

Warning: oci_set_prefetch(): Number of rows to be prefetched has to be greater than or equal to 0 in %s on line %d
Fetch Row  from PHP
array(2) {
  [0]=>
  %unicode|string%(%d) "0"
  [1]=>
  %unicode|string%(%d) "test0"
}
Fetch Row  from PL/SQL
%unicode|string%(%d) "101"
%unicode|string%(%d) "test101"
-----------------------------------------------
Test with Prefetch value set to -1 
-----------------------------------------------

Warning: oci_set_prefetch(): Number of rows to be prefetched has to be greater than or equal to 0 in %s on line %d
Fetch Row  from PHP
array(2) {
  [0]=>
  %unicode|string%(%d) "0"
  [1]=>
  %unicode|string%(%d) "test0"
}
Fetch Row  from PL/SQL
%unicode|string%(%d) "101"
%unicode|string%(%d) "test101"
Done
