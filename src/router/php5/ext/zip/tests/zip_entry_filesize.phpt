--TEST--
zip_entry_filesize() function
--SKIPIF--
<?php
/* $Id: zip_entry_filesize.phpt,v 1.1 2006/07/24 16:58:58 pajoye Exp $ */
if(!extension_loaded('zip')) die('skip');
?>
--FILE--
<?php
$zip = zip_open(dirname(__FILE__)."/test_procedural.zip");
if (!is_resource($zip)) die("Failure");
$entries = 0;
while ($entry = zip_read($zip)) {
  echo zip_entry_filesize($entry)."\n";
}
zip_close($zip);

?>
--EXPECT--
5
4
0
27
