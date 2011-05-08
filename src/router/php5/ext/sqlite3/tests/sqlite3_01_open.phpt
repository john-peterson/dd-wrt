--TEST--
SQLite3::open/close tests
--SKIPIF--
<?php require_once(dirname(__FILE__) . '/skipif.inc'); ?>
--FILE--
<?php

require_once(dirname(__FILE__) . '/new_db.inc');

var_dump($db);
var_dump($db->close());
echo "Done\n";
?>
--EXPECTF--
object(SQLite3)#%d (0) {
}
bool(true)
Done
