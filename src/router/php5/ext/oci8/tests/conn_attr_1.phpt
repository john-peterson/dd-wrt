--TEST--
Set and get of connection attributes with all types of connections.
--SKIPIF--
<?php if (!extension_loaded('oci8')) die("skip no oci8 extension"); 
require(dirname(__FILE__)."/connect.inc");
if (strcasecmp($user, "system") && strcasecmp($user, "sys"))
    die("skip needs to be run as a DBA user");
if ($test_drcp) die("skip output might vary with DRCP");

$sv = oci_server_version($c);
$sv = preg_match('/Release 1[012]\./', $sv, $matches);
if ($sv == 1) {
    ob_start();
    phpinfo(INFO_MODULES);
    $phpinfo = ob_get_clean();
    $iv = preg_match('/Oracle .*Version => 1[012]\./', $phpinfo);
    if ($iv != 1) {
        die ("skip test expected to work only with Oracle 10g or greater client ");
    }
}
else {
    die ("skip test expected to work only with Oracle 10g or greater server");
}

?>
--FILE--
<?php
require(dirname(__FILE__)."/conn_attr.inc");

$attr_array = array('MODULE','ACTION','CLIENT_INFO','CLIENT_IDENTIFIER');

echo"**Test 1.1 - Default values for the attributes **************\n";
$c = get_conn(1);
foreach($attr_array as $attr) {
        get_attr($c,$attr);
}

echo"**Test 1.2 - Set and get values for the attributes **************\n";

// With oci_connect, oci_pconnect, oci_new_connect

$conn1 = get_conn(1); //oci_connect()
foreach($attr_array as $attr) {
	set_attr($conn1,$attr,1);
    get_attr($conn1,$attr);
}

$conn2 = get_conn(2); //oci_pconnect()
foreach($attr_array as $attr) {
	set_attr($conn2,$attr,2);
    get_attr($conn2,$attr);
}

$conn3 = get_conn(3); //oci_new_connect()
foreach($attr_array as $attr) {
	set_attr($conn3,$attr,3);
    get_attr($conn3,$attr);
}

// clean up
oci_close($conn1);
oci_close($conn2);
oci_close($conn3);
clean_up($c);

echo "Done\n";

?>
--EXPECTF--
**Test 1.1 - Default values for the attributes **************
Testing with oci_connect()
The value of MODULE is %s
The value of ACTION is 
The value of CLIENT_INFO is 
The value of CLIENT_IDENTIFIER is 
**Test 1.2 - Set and get values for the attributes **************
Testing with oci_connect()
Value of MODULE has been set successfully
The value of MODULE is PHP TEST1
Value of ACTION has been set successfully
The value of ACTION is TASK1
Value of CLIENT_INFO has been set successfully
The value of CLIENT_INFO is INFO11
Value of CLIENT_IDENTIFIER has been set successfully
The value of CLIENT_IDENTIFIER is ID001
Testing with oci_pconnect()
Value of MODULE has been set successfully
The value of MODULE is PHP TEST2
Value of ACTION has been set successfully
The value of ACTION is TASK2
Value of CLIENT_INFO has been set successfully
The value of CLIENT_INFO is INFO12
Value of CLIENT_IDENTIFIER has been set successfully
The value of CLIENT_IDENTIFIER is ID002
Testing with oci_new_connect()
Value of MODULE has been set successfully
The value of MODULE is PHP TEST3
Value of ACTION has been set successfully
The value of ACTION is TASK3
Value of CLIENT_INFO has been set successfully
The value of CLIENT_INFO is INFO13
Value of CLIENT_IDENTIFIER has been set successfully
The value of CLIENT_IDENTIFIER is ID003
Done
