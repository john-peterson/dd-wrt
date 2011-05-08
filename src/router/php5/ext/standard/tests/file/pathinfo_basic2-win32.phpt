--TEST--
Test pathinfo() function: basic functionality
--CREDITS--
Dave Kelsey <d_kelsey@uk.ibm.com>
--SKIPIF--
<?php
if(substr(PHP_OS, 0, 3) != "WIN")
  die("skip Only valid for Windows");
?>
--FILE--
<?php
/* Prototype: mixed pathinfo ( string $path [, int $options] );
   Description: Returns information about a file path
*/

echo "*** Testing basic functions of pathinfo() ***\n";

$paths = array (
			'c:\..\dir1',
			'c:\test\..\test2\.\adir\afile.txt',
			'/usr/include/../arpa/./inet.h',
			'c:\test\adir\afile..txt',
			'/usr/include/arpa/inet..h',
			'c:\test\adir\afile.',
			'/usr/include/arpa/inet.',
			'/usr/include/arpa/inet,h',
			'c:afile.txt',
			'..\.\..\test\afile.txt',
			'.././../test/afile',
			'.',
			'..',
			'...',
			'/usr/lib/.../afile'
						
);

$counter = 1;
/* loop through $paths to test each $path in the above array */
foreach($paths as $path) {
  echo "-- Iteration $counter --\n";
  var_dump( pathinfo($path, PATHINFO_DIRNAME) );
  var_dump( pathinfo($path, PATHINFO_BASENAME) );
  var_dump( pathinfo($path, PATHINFO_EXTENSION) );
  var_dump( pathinfo($path, PATHINFO_FILENAME) );
  var_dump( pathinfo($path) );
  $counter++;
}

echo "Done\n";
?>
--EXPECT--
*** Testing basic functions of pathinfo() ***
-- Iteration 1 --
string(5) "c:\.."
string(4) "dir1"
string(0) ""
string(4) "dir1"
array(3) {
  ["dirname"]=>
  string(5) "c:\.."
  ["basename"]=>
  string(4) "dir1"
  ["filename"]=>
  string(4) "dir1"
}
-- Iteration 2 --
string(23) "c:\test\..\test2\.\adir"
string(9) "afile.txt"
string(3) "txt"
string(5) "afile"
array(4) {
  ["dirname"]=>
  string(23) "c:\test\..\test2\.\adir"
  ["basename"]=>
  string(9) "afile.txt"
  ["extension"]=>
  string(3) "txt"
  ["filename"]=>
  string(5) "afile"
}
-- Iteration 3 --
string(22) "/usr/include/../arpa/."
string(6) "inet.h"
string(1) "h"
string(4) "inet"
array(4) {
  ["dirname"]=>
  string(22) "/usr/include/../arpa/."
  ["basename"]=>
  string(6) "inet.h"
  ["extension"]=>
  string(1) "h"
  ["filename"]=>
  string(4) "inet"
}
-- Iteration 4 --
string(12) "c:\test\adir"
string(10) "afile..txt"
string(3) "txt"
string(6) "afile."
array(4) {
  ["dirname"]=>
  string(12) "c:\test\adir"
  ["basename"]=>
  string(10) "afile..txt"
  ["extension"]=>
  string(3) "txt"
  ["filename"]=>
  string(6) "afile."
}
-- Iteration 5 --
string(17) "/usr/include/arpa"
string(7) "inet..h"
string(1) "h"
string(5) "inet."
array(4) {
  ["dirname"]=>
  string(17) "/usr/include/arpa"
  ["basename"]=>
  string(7) "inet..h"
  ["extension"]=>
  string(1) "h"
  ["filename"]=>
  string(5) "inet."
}
-- Iteration 6 --
string(12) "c:\test\adir"
string(6) "afile."
string(0) ""
string(5) "afile"
array(4) {
  ["dirname"]=>
  string(12) "c:\test\adir"
  ["basename"]=>
  string(6) "afile."
  ["extension"]=>
  string(0) ""
  ["filename"]=>
  string(5) "afile"
}
-- Iteration 7 --
string(17) "/usr/include/arpa"
string(5) "inet."
string(0) ""
string(4) "inet"
array(4) {
  ["dirname"]=>
  string(17) "/usr/include/arpa"
  ["basename"]=>
  string(5) "inet."
  ["extension"]=>
  string(0) ""
  ["filename"]=>
  string(4) "inet"
}
-- Iteration 8 --
string(17) "/usr/include/arpa"
string(6) "inet,h"
string(0) ""
string(6) "inet,h"
array(3) {
  ["dirname"]=>
  string(17) "/usr/include/arpa"
  ["basename"]=>
  string(6) "inet,h"
  ["filename"]=>
  string(6) "inet,h"
}
-- Iteration 9 --
string(3) "c:."
string(11) "c:afile.txt"
string(3) "txt"
string(7) "c:afile"
array(4) {
  ["dirname"]=>
  string(3) "c:."
  ["basename"]=>
  string(11) "c:afile.txt"
  ["extension"]=>
  string(3) "txt"
  ["filename"]=>
  string(7) "c:afile"
}
-- Iteration 10 --
string(12) "..\.\..\test"
string(9) "afile.txt"
string(3) "txt"
string(5) "afile"
array(4) {
  ["dirname"]=>
  string(12) "..\.\..\test"
  ["basename"]=>
  string(9) "afile.txt"
  ["extension"]=>
  string(3) "txt"
  ["filename"]=>
  string(5) "afile"
}
-- Iteration 11 --
string(12) ".././../test"
string(5) "afile"
string(0) ""
string(5) "afile"
array(3) {
  ["dirname"]=>
  string(12) ".././../test"
  ["basename"]=>
  string(5) "afile"
  ["filename"]=>
  string(5) "afile"
}
-- Iteration 12 --
string(1) "."
string(1) "."
string(0) ""
string(0) ""
array(4) {
  ["dirname"]=>
  string(1) "."
  ["basename"]=>
  string(1) "."
  ["extension"]=>
  string(0) ""
  ["filename"]=>
  string(0) ""
}
-- Iteration 13 --
string(1) "."
string(2) ".."
string(0) ""
string(1) "."
array(4) {
  ["dirname"]=>
  string(1) "."
  ["basename"]=>
  string(2) ".."
  ["extension"]=>
  string(0) ""
  ["filename"]=>
  string(1) "."
}
-- Iteration 14 --
string(1) "."
string(3) "..."
string(0) ""
string(2) ".."
array(4) {
  ["dirname"]=>
  string(1) "."
  ["basename"]=>
  string(3) "..."
  ["extension"]=>
  string(0) ""
  ["filename"]=>
  string(2) ".."
}
-- Iteration 15 --
string(12) "/usr/lib/..."
string(5) "afile"
string(0) ""
string(5) "afile"
array(3) {
  ["dirname"]=>
  string(12) "/usr/lib/..."
  ["basename"]=>
  string(5) "afile"
  ["filename"]=>
  string(5) "afile"
}
Done