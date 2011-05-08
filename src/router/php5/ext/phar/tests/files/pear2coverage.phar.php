<?php
function __autoload($class)
{
    $class = str_replace("PEAR2\Pyrus\Developer\CoverageAnalyzer", "", $class);
    include "phar://" . __FILE__ . "/" . str_replace("\\", "/", $class) . ".php";
}
Phar::webPhar("pear2coverage.phar.php");
echo "This phar is a web application, run within your web browser to use\n";
exit -1;
__HALT_COMPILER(); ?>
�  
                  Web/Controller.php�	  �0�I�	  ���ն         Web/View.phpS:  �0�IS:  Le�         Web/Aggregator.php  �0�I  ݺ.b�         Web/Exception.phph   �0�Ih   pZ�         SourceFile.php:  �0�I:  ~��0�         Aggregator.php�  �0�I�  �3n\�         Exception.phpd   �0�Id   @���      
   Sqlite.php0X  �0�I0X  �{�,�         SourceFile/PerTest.php�  �0�I�  �R�      	   index.phpF  �0�IF  k!�_�      <?php
namespace PEAR2\Pyrus\Developer\CoverageAnalyzer\Web {
use PEAR2\Pyrus\Developer\CoverageAnalyzer\Sqlite;
class Controller {
    protected $view;
    protected $sqlite;
    protected $rooturl;

    function __construct(View $view, $rooturl)
    {
        $this->view = $view;
        $view->setController($this);
        $this->rooturl = $rooturl;
    }

    function route()
    {
        if (!isset($_SESSION['fullpath'])) {
            if (isset($_POST['setdatabase'])) {
                if (file_exists($_POST['setdatabase'])) {
                    $this->sqlite = new \Sqlite3($_POST['setdatabase']);
                    $_SESSION['fullpath'] = $_POST['setdatabase'];
                    return $this->view->TOC($this->sqlite);
                }
            }
            return $this->getDatabase();
        } else {
            $this->sqlite = new \Sqlite3($_POST['setdatabase']);
            if (isset($_GET['test'])) {
                if ($_GET['test'] === 'TOC') {
                    return $this->view->testTOC($this->sqlite);
                }
                if (isset($_GET['file'])) {
                    return $this->view->fileCoverage($this->sqlite, $_GET['file'], $_GET['test']);
                }
                return $this->view->testTOC($this->sqlite, $_GET['test']);
            }
            if (isset($_GET['file'])) {
                if (isset($_GET['line'])) {
                    return $this->view->fileLineTOC($this->sqlite, $_GET['file'], $_GET['line']);
                }
                return $this->view->fileCoverage($this->sqlite, $_GET['file']);
            }
            return $this->view->TOC($this->sqlite);
        }
    }

    function getFileLink($file, $test = null, $line = null)
    {
        if ($line) {
            return $this->rooturl . '?file=' . urlencode($file) . '&line=' . $line;
        }
        if ($test) {
            return $this->rooturl . '?file=' . urlencode($file) . '&test=' . $test;
        }
        return $this->rooturl . '?file=' . urlencode($file);
    }

    function getTOCLink($test = false)
    {
        if ($test === false) {
            return $this->rooturl;
        }
        if ($test === true) {
            return $this->rooturl . '?test=TOC';
        }
        if ($test) {
            return $this->rooturl . '?test=' . urlencode($test);
        }
    }

    function getDatabase()
    {
        $this->sqlite = $this->view->getDatabase();
    }
}
}
?>
<?php
namespace PEAR2\Pyrus\Developer\CoverageAnalyzer\Web {
/**
 * Takes a source file and outputs HTML source highlighting showing the
 * number of hits on each line, highlights un-executed lines in red
 */
class View
{
    protected $savePath;
    protected $testPath;
    protected $sourcePath;
    protected $source;
    protected $controller;

    function getDatabase()
    {
        $output = new \XMLWriter;
        if (!$output->openUri('php://output')) {
            throw new Exception('Cannot open output - this should never happen');
        }
        $output->startElement('html');
         $output->startElement('head');
          $output->writeElement('title', 'Enter a path to the database');
         $output->endElement();
         $output->startElement('body');
          $output->writeElement('h2', 'Please enter the path to a coverage database');
          $output->startElement('form');
           $output->writeAttribute('name', 'getdatabase');
           $output->writeAttribute('method', 'POST');
           $output->writeAttribute('action', $this->controller->getTOCLink());
           $output->startElement('input');
            $output->writeAttribute('type', 'text');
            $output->writeAttribute('name', 'setdatabase');
           $output->endElement();
           $output->startElement('input');
            $output->writeAttribute('type', 'submit');
           $output->endElement();
          $output->endElement();
         $output->endElement();
        $output->endElement();
        $output->endDocument();
    }

    function setController($controller)
    {
        $this->controller = $controller;
    }

    function TOC($sqlite)
    {
        $coverage = $sqlite->retrieveProjectCoverage();
        $this->renderSummary($sqlite, $sqlite->retrievePaths(), false, $coverage[1], $covered[0]);
    }

    function testTOC($sqlite)
    {
        $this->renderTestSummary($sqlite);
    }

    function fileLineTOC($sqlite, $file, $line)
    {
        
    }

    function fileCoverage($sqlite, $file, $test = null)
    {
        
    }

    function mangleFile($path, $istest = false)
    {
        return $this->controller->getFileLink($path, $istest);
    }

    function mangleTestFile($path)
    {
        return $this->controller->getTOClink($path);
    }

    function getLineLink($name, $line)
    {
        return $this->controller->getFileLink($name, null, $line);
    }

    function renderLineSummary($name, $line, $testpath, $tests)
    {
        $output = new \XMLWriter;
        if (!$output->openUri($this->getLinePath($name, $line))) {
            throw new Exception('Cannot render ' . $name . ' line ' . $line . ', opening XML failed');
        }
        $output->setIndentString(' ');
        $output->setIndent(true);
        $output->startElement('html');
        $output->startElement('head');
        $output->writeElement('title', 'Tests covering line ' . $line . ' of ' . $name);
        $output->startElement('link');
        $output->writeAttribute('href', 'cover.css');
        $output->writeAttribute('rel', 'stylesheet');
        $output->writeAttribute('type', 'text/css');
        $output->endElement();
        $output->endElement();
        $output->startElement('body');
        $output->writeElement('h2', 'Tests covering line ' . $line . ' of ' . $name);
        $output->startElement('p');
        $output->startElement('a');
        $output->writeAttribute('href', 'index.html');
        $output->text('Aggregate Code Coverage for all tests');
        $output->endElement();
        $output->endElement();
        $output->startElement('p');
        $output->startElement('a');
        $output->writeAttribute('href', $this->mangleFile($name));
        $output->text('File ' . $name . ' code coverage');
        $output->endElement();
        $output->endElement();
        $output->startElement('ul');
        foreach ($tests as $testfile) {
            $output->startElement('li');
            $output->startElement('a');
            $output->writeAttribute('href', $this->mangleTestFile($testfile));
            $output->text(str_replace($testpath . '/', '', $testfile));
            $output->endElement();
            $output->endElement();
        }
        $output->endElement();
        $output->endElement();
        $output->endDocument();
    }

    /**
     * @param PEAR2\Pyrus\Developer\CodeCoverage\SourceFile $source
     * @param string $istest path to test file this is covering, or false for aggregate
     */
    function render(SourceFile $source, $istest = false)
    {
        $output = new \XMLWriter;
        if (!$output->openUri($this->manglePath($source->name(), $istest))) {
            throw new Exception('Cannot render ' . $source->name() . ', opening XML failed');
        }
        $output->setIndent(false);
        $output->startElement('html');
        $output->text("\n ");
        $output->startElement('head');
        $output->text("\n  ");
        if ($istest) {
            $output->writeElement('title', 'Code Coverage for ' . $source->shortName() . ' in ' . $istest);
        } else {
            $output->writeElement('title', 'Code Coverage for ' . $source->shortName());
        }
        $output->text("\n  ");
        $output->startElement('link');
        $output->writeAttribute('href', 'cover.css');
        $output->writeAttribute('rel', 'stylesheet');
        $output->writeAttribute('type', 'text/css');
        $output->endElement();
        $output->text("\n  ");
        $output->endElement();
        $output->text("\n ");
        $output->startElement('body');
        $output->text("\n ");
        if ($istest) {
            $output->writeElement('h2', 'Code Coverage for ' . $source->shortName() . ' in ' . $istest);
        } else {
            $output->writeElement('h2', 'Code Coverage for ' . $source->shortName());
        }
        $output->text("\n ");
        $output->writeElement('h3', 'Coverage: ' . $source->coveragePercentage() . '%');
        $output->text("\n ");
        $output->startElement('p');
        $output->startElement('a');
        $output->writeAttribute('href', 'index.html');
        $output->text('Aggregate Code Coverage for all tests');
        $output->endElement();
        $output->endElement();
        $output->startElement('pre');
        foreach ($source->source() as $num => $line) {
            $coverage = $source->coverage($num);

            $output->startElement('span');
            $output->writeAttribute('class', 'ln');
            $output->text(str_pad($num, 8, ' ', STR_PAD_LEFT));
            $output->endElement();

            if ($coverage === false) {
                $output->text(str_pad(': ', 13, ' ', STR_PAD_LEFT) . $line);
                continue;
            }

            $output->startElement('span');
            if ($coverage < 1) {
                $output->writeAttribute('class', 'nc');
                $output->text('           ');
            } else {
                $output->writeAttribute('class', 'cv');
                if (!$istest) {
                    $output->startElement('a');
                    $output->writeAttribute('href', $this->getLineLink($source->name(), $num));
                }
                $output->text(str_pad($coverage, 10, ' ', STR_PAD_LEFT) . ' ');
                if (!$istest) {
                    $output->endElement();
                    $this->renderLineSummary($source->name(), $num, $source->testpath(),
                                             $source->getLineLinks($num));
                }
            }

            $output->text(': ' .  $line);
            $output->endElement();
        }
        $output->endElement();
        $output->text("\n ");
        $output->endElement();
        $output->text("\n ");
        $output->endElement();
        $output->endDocument();
    }

    function renderSummary(Aggregator $agg, array $results, $istest = false, $total = 1, $covered = 1)
    {
        $output = new \XMLWriter;
        if (!$output->openUri('php://output')) {
            throw new Exception('Cannot render test summary, opening XML failed');
        }
        $output->setIndentString(' ');
        $output->setIndent(true);
        $output->startElement('html');
        $output->startElement('head');
        if ($istest) {
            $output->writeElement('title', 'Code Coverage Summary [' . $istest . ']');
        } else {
            $output->writeElement('title', 'Code Coverage Summary');
        }
        $output->startElement('link');
        $output->writeAttribute('href', 'cover.css');
        $output->writeAttribute('rel', 'stylesheet');
        $output->writeAttribute('type', 'text/css');
        $output->endElement();
        $output->endElement();
        $output->startElement('body');
        if ($istest) {
            $output->writeElement('h2', 'Code Coverage Files for test ' . $istest);
        } else {
            $output->writeElement('h2', 'Code Coverage Files');
            $output->writeElement('h3', 'Total lines: ' . $total . ', covered lines: ' . $covered);
            $percent = 0;
            if ($total > 0) {
                $percent = round(($covered / $total) * 100);
            }
            $output->startElement('p');
            if ($percent < 50) {
                $output->writeAttribute('class', 'bad');
            } elseif ($percent < 75) {
                $output->writeAttribute('class', 'ok');
            } else {
                $output->writeAttribute('class', 'good');
            }
            $output->text($percent . '% code coverage');
            $output->endElement();
        }
        $output->startElement('p');
        $output->startElement('a');
        $output->writeAttribute('href', $this->controller->getTOCLink(true));
        $output->text('Code Coverage per PHPT test');
        $output->endElement();
        $output->endElement();
        $output->startElement('ul');
        foreach ($results as $i => $name) {
            $source = new SourceFile($name, $agg, $this->testPath, $this->sourcePath);
            $output->startElement('li');
            $percent = $source->coveragePercentage();
            $output->startElement('span');
            if ($percent < 50) {
                $output->writeAttribute('class', 'bad');
            } elseif ($percent < 75) {
                $output->writeAttribute('class', 'ok');
            } else {
                $output->writeAttribute('class', 'good');
            }
            $output->text(' Coverage: ' . str_pad($percent . '%', 4, ' ', STR_PAD_LEFT));
            $output->endElement();
            $output->startElement('a');
            $output->writeAttribute('href', $this->mangleFile($name, $istest));
            $output->text($source->shortName());
            $output->endElement();
            $output->endElement();
        }
        $output->endElement();
        $output->endElement();
        $output->endDocument();
    }

    function renderTestSummary(Aggregator $agg)
    {
        $output = new \XMLWriter;
        if (!$output->openUri('php://output')) {
                throw new Exception('Cannot render tests summary, opening XML failed');
        }
        $output->setIndentString(' ');
        $output->setIndent(true);
        $output->startElement('html');
        $output->startElement('head');
        $output->writeElement('title', 'Test Summary');
        $output->startElement('link');
        $output->writeAttribute('href', 'cover.css');
        $output->writeAttribute('rel', 'stylesheet');
        $output->writeAttribute('type', 'text/css');
        $output->endElement();
        $output->endElement();
        $output->startElement('body');
        $output->writeElement('h2', 'Tests Executed, click for code coverage summary');
        $output->startElement('p');
        $output->startElement('a');
        $output->writeAttribute('href', $this->controller->getTOClink());
        $output->text('Aggregate Code Coverage for all tests');
        $output->endElement();
        $output->endElement();
        $output->startElement('ul');
        foreach ($agg->retrieveTestPaths() as $test) {
            $output->startElement('li');
            $output->startElement('a');
            $output->writeAttribute('href', $this->mangleTestFile($test));
            $output->text(str_replace($agg->testpath . '/', '', $test));
            $output->endElement();
            $output->endElement();
        }
        $output->endElement();
        $output->endElement();
        $output->endDocument();
    }

    function renderTestCoverage(Aggregator $agg, $testpath, $test)
    {
        $reltest = str_replace($testpath . '/', '', $test);
        $output = new \XMLWriter;
        if (!$output->openUri('php://output')) {
            throw new Exception('Cannot render test ' . $reltest . ' coverage, opening XML failed');
        }
            $output->setIndentString(' ');
            $output->setIndent(true);
            $output->startElement('html');
            $output->startElement('head');
            $output->writeElement('title', 'Code Coverage Summary for test ' . $reltest);
            $output->startElement('link');
            $output->writeAttribute('href', 'cover.css');
            $output->writeAttribute('rel', 'stylesheet');
            $output->writeAttribute('type', 'text/css');
            $output->endElement();
            $output->endElement();
            $output->startElement('body');
            $output->writeElement('h2', 'Code Coverage Files for test ' . $reltest);
            $output->startElement('ul');
            $paths = $agg->retrievePathsForTest($test);
            foreach ($paths as $name) {
                echo '.';
                $source = new SourceFile\PerTest($name, $agg, $testpath, $basePath, $test);
                $this->render($source, $reltest);
                $output->startElement('li');
                $percent = $source->coveragePercentage();
                $output->startElement('span');
                if ($percent < 50) {
                    $output->writeAttribute('class', 'bad');
                } elseif ($percent < 75) {
                    $output->writeAttribute('class', 'ok');
                } else {
                    $output->writeAttribute('class', 'good');
                }
                $output->text(' Coverage: ' . str_pad($source->coveragePercentage() . '%', 4, ' ', STR_PAD_LEFT));
                $output->endElement();
                $output->startElement('a');
                $output->writeAttribute('href', $this->mangleFile($name, $reltest));
                $output->text($source->shortName());
                $output->endElement();
                $output->endElement();
            }
            echo "done\n";
            $output->endElement();
            $output->endElement();
            $output->endDocument();
        }
        echo "done\n";
    }
}
}
?>
<?php
namespace PEAR2\Pyrus\Developer\CoverageAnalyzer\Web {
use PEAR2\Pyrus\Developer\CoverageAnalyzer;
class Aggregator extends CoverageAnalyzer\Aggregator;
{
    protected $codepath;
    protected $testpath;
    protected $sqlite;
    public $totallines = 0;
    public $totalcoveredlines = 0;

    /**
     * @var string $testpath Location of .phpt files
     * @var string $codepath Location of code whose coverage we are testing
     */
    function __construct($testpath, $codepath, $db = ':memory:')
    {
        $newcodepath = realpath($codepath);
        if (!$newcodepath) {
            if (!strpos($codepath, '://') || !file_exists($codepath)) {
                // stream wrapper not found
                throw new Exception('Can not find code path $codepath');
            }
        } else {
            $codepath = $newcodepath;
        }
        $this->sqlite = new Sqlite($codepath, $db);
        $this->codepath = $codepath;
    }

    function retrieveLineLinks($file)
    {
        return $this->sqlite->retrieveLineLinks($file);
    }

    function retrievePaths()
    {
        return $this->sqlite->retrievePaths();
    }

    function retrievePathsForTest($test)
    {
        return $this->sqlite->retrievePathsForTest($test);
    }

    function retrieveTestPaths()
    {
        return $this->sqlite->retrieveTestPaths();
    }

    function coveragePercentage($sourcefile, $testfile = null)
    {
        return $this->sqlite->coveragePercentage($sourcefile, $testfile);
    }

    function coverageInfo($path)
    {
        return $this->sqlite->retrievePathCoverage($path);
    }

    function coverageInfoByTest($path, $test)
    {
        return $this->sqlite->retrievePathCoverageByTest($path, $test);
    }

    function retrieveCoverage($path)
    {
        return $this->sqlite->retrieveCoverage($path);
    }

    function retrieveCoverageByTest($path, $test)
    {
        return $this->sqlite->retrieveCoverageByTest($path, $test);
    }

    function retrieveXdebug($path, $testid)
    {
        $source = '$xdebug = ' . file_get_contents($path) . ";\n";
        eval($source);
        $this->sqlite->addCoverage(str_replace('.xdebug', '.phpt', $path), $testid, $xdebug);
    }

    function scan($testpath)
    {
        $a = $testpath;
        $testpath = realpath($testpath);
        if (!$testpath) {
            throw new Exception('Unable to process path' . $a);
        }
        $testpath = str_replace('\\', '/', $testpath);
        $this->testpath = $testpath;

        // first get a list of all directories
        $dirs = $globdirs = array();
        $index = 0;
        $dir = $testpath;
        do {
            $globdirs = glob($dir . '/*', GLOB_ONLYDIR);
            if ($globdirs) {
                $dirs = array_merge($dirs, $globdirs);
                $dir = $dirs[$index++];
            } else {
                while (!isset($dirs[$index++]) && $index <= count($dirs));
                if (isset($dirs[$index])) {
                    $dir = $dirs[$index];
                }
            }
        } while ($index <= count($dirs));

        // then find all code coverage files
        $xdebugs = array();
        foreach ($dirs as $dir) {
            $globbie = glob($dir . '/*.xdebug');
            $xdebugs = array_merge($xdebugs, $globbie);
        }
        $xdebugs = array_unique($xdebugs);
        $modified = array();
        $unmodified = array();
        foreach ($xdebugs as $path) {
            if ($this->sqlite->unChangedXdebug($path)) {
                $unmodified[$path] = true;
                continue;
            }
            $modified[] = $path;
        }
        $xdebugs = $modified;
        sort($xdebugs);
        // index from 1
        array_unshift($xdebugs, '');
        unset($xdebugs[0]);
        $test = array_flip($xdebugs);
        foreach ($this->sqlite->retrieveTestPaths() as $path) {
            $xdebugpath = str_replace('.phpt', '.xdebug', $path);
            if (isset($test[$xdebugpath]) || isset($unmodified[$xdebugpath])) {
                continue;
            }
            // remove outdated tests
            echo "Removing results from $xdebugpath\n";
            $this->sqlite->removeOldTest($path, $xdebugpath);
        }
        return $xdebugs;
    }

    function render($toPath)
    {
        $decorator = new DefaultSourceDecorator($toPath, $this->testpath, $this->codepath);
        echo "Generating project coverage data...";
        $coverage = $this->sqlite->retrieveProjectCoverage();
        echo "done\n";
        $decorator->renderSummary($this, $this->retrievePaths(), $this->codepath, false, $coverage[1], 
                                  $coverage[0]);
        $a = $this->codepath;
        echo "[Step 2 of 2] Rendering per-test coverage...";
        $decorator->renderTestCoverage($this, $this->testpath, $a);
        echo "done\n";
    }
}
}
?>
<?php
namespace PEAR2\Pyrus\Developer\CoverageAnalyzer\Web {
class Exception extends \Exception {}
}
?>
<?php
namespace PEAR2\Pyrus\Developer\CoverageAnalyzer {
class SourceFile
{
    protected $source;
    protected $path;
    protected $sourcepath;
    protected $coverage;
    protected $aggregator;
    protected $testpath;
    protected $linelinks;

    function __construct($path, Aggregator $agg, $testpath, $sourcepath)
    {
        $this->source = file($path);
        $this->path = $path;
        $this->sourcepath = $sourcepath;

        array_unshift($this->source, '');
        unset($this->source[0]); // make source array indexed by line number

        $this->aggregator = $agg;
        $this->testpath = $testpath;
        $this->setCoverage();
    }

    function setCoverage()
    {
        $this->coverage = $this->aggregator->retrieveCoverage($this->path);
    }

    function aggregator()
    {
        return $this->aggregator;
    }

    function testpath()
    {
        return $this->testpath;
    }

    function render(AbstractSourceDecorator $decorator = null)
    {
        if ($decorator === null) {
            $decorator = new DefaultSourceDecorator('.');
        }
        return $decorator->render($this);
    }

    function coverage($line)
    {
        if (!isset($this->coverage[$line])) {
            return false;
        }
        return $this->coverage[$line];
    }

    function coveragePercentage()
    {
        return $this->aggregator->coveragePercentage($this->path);
    }

    function name()
    {
        return $this->path;
    }

    function shortName()
    {
        return str_replace($this->sourcepath . DIRECTORY_SEPARATOR, '', $this->path);
    }

    function source()
    {
        return $this->source;
    }

    function coveredLines()
    {
        $info = $this->aggregator->coverageInfo($this->path);
        return $info[0];
    }

    function getLineLinks($line)
    {
        if (!isset($this->linelinks)) {
            $this->linelinks = $this->aggregator->retrieveLineLinks($this->path);
        }
        if (isset($this->linelinks[$line])) {
            return $this->linelinks[$line];
        }
        return false;
    }
}
}
?>
<?php
namespace PEAR2\Pyrus\Developer\CoverageAnalyzer {
class Aggregator
{
    protected $codepath;
    protected $testpath;
    protected $sqlite;
    public $totallines = 0;
    public $totalcoveredlines = 0;

    /**
     * @var string $testpath Location of .phpt files
     * @var string $codepath Location of code whose coverage we are testing
     */
    function __construct($testpath, $codepath, $db = ':memory:')
    {
        $newcodepath = realpath($codepath);
        if (!$newcodepath) {
            if (!strpos($codepath, '://') || !file_exists($codepath)) {
                // stream wrapper not found
                throw new Exception('Can not find code path $codepath');
            }
        } else {
            $codepath = $newcodepath;
        }
        $this->sqlite = new Sqlite($db, $codepath, $testpath);
        $this->codepath = $codepath;
        $this->sqlite->begin();
        echo "Scanning for xdebug coverage files...";
        $files = $this->scan($testpath);
        echo "done\n";
        $infostring = '';
        echo "Parsing xdebug results\n";
        if (count($files)) {
            foreach ($files as $testid => $xdebugfile) {
                if (!file_exists(str_replace('.xdebug', '.phpt', $xdebugfile))) {
                    echo "\nWARNING: outdated .xdebug file $xdebugfile, delete this relic\n";
                    continue;
                }
                $id = $this->sqlite->addTest(str_replace('.xdebug', '.phpt', $xdebugfile));
                echo '(' . $testid . ' of ' . count($files) . ') ' . $xdebugfile;
                $this->retrieveXdebug($xdebugfile, $id);
                echo "done\n";
            }
            echo "done\n";
            $this->sqlite->updateTotalCoverage();
            $this->sqlite->commit();
        } else {
            echo "done (no modified xdebug files)\n";
        }
    }

    function retrieveLineLinks($file)
    {
        return $this->sqlite->retrieveLineLinks($file);
    }

    function retrievePaths()
    {
        return $this->sqlite->retrievePaths();
    }

    function retrievePathsForTest($test)
    {
        return $this->sqlite->retrievePathsForTest($test);
    }

    function retrieveTestPaths()
    {
        return $this->sqlite->retrieveTestPaths();
    }

    function coveragePercentage($sourcefile, $testfile = null)
    {
        return $this->sqlite->coveragePercentage($sourcefile, $testfile);
    }

    function coverageInfo($path)
    {
        return $this->sqlite->retrievePathCoverage($path);
    }

    function coverageInfoByTest($path, $test)
    {
        return $this->sqlite->retrievePathCoverageByTest($path, $test);
    }

    function retrieveCoverage($path)
    {
        return $this->sqlite->retrieveCoverage($path);
    }

    function retrieveCoverageByTest($path, $test)
    {
        return $this->sqlite->retrieveCoverageByTest($path, $test);
    }

    function retrieveXdebug($path, $testid)
    {
        $source = '$xdebug = ' . file_get_contents($path) . ";\n";
        eval($source);
        $this->sqlite->addCoverage(str_replace('.xdebug', '.phpt', $path), $testid, $xdebug);
    }

    function scan($testpath)
    {
        $a = $testpath;
        $testpath = realpath($testpath);
        if (!$testpath) {
            throw new Exception('Unable to process path' . $a);
        }
        $testpath = str_replace('\\', '/', $testpath);
        $this->testpath = $testpath;

        // first get a list of all directories
        $dirs = $globdirs = array();
        $index = 0;
        $dir = $testpath;
        do {
            $globdirs = glob($dir . '/*', GLOB_ONLYDIR);
            if ($globdirs) {
                $dirs = array_merge($dirs, $globdirs);
                $dir = $dirs[$index++];
            } else {
                while (!isset($dirs[$index++]) && $index <= count($dirs));
                if (isset($dirs[$index])) {
                    $dir = $dirs[$index];
                }
            }
        } while ($index <= count($dirs));

        // then find all code coverage files
        $xdebugs = array();
        foreach ($dirs as $dir) {
            $globbie = glob($dir . '/*.xdebug');
            $xdebugs = array_merge($xdebugs, $globbie);
        }
        $xdebugs = array_unique($xdebugs);
        $modified = array();
        $unmodified = array();
        foreach ($xdebugs as $path) {
            if ($this->sqlite->unChangedXdebug($path)) {
                $unmodified[$path] = true;
                continue;
            }
            $modified[] = $path;
        }
        $xdebugs = $modified;
        sort($xdebugs);
        // index from 1
        array_unshift($xdebugs, '');
        unset($xdebugs[0]);
        $test = array_flip($xdebugs);
        foreach ($this->sqlite->retrieveTestPaths() as $path) {
            $xdebugpath = str_replace('.phpt', '.xdebug', $path);
            if (isset($test[$xdebugpath]) || isset($unmodified[$xdebugpath])) {
                continue;
            }
            // remove outdated tests
            echo "Removing results from $xdebugpath\n";
            $this->sqlite->removeOldTest($path, $xdebugpath);
        }
        return $xdebugs;
    }

    function render($toPath)
    {
        $decorator = new DefaultSourceDecorator($toPath, $this->testpath, $this->codepath);
        echo "Generating project coverage data...";
        $coverage = $this->sqlite->retrieveProjectCoverage();
        echo "done\n";
        $decorator->renderSummary($this, $this->retrievePaths(), $this->codepath, false, $coverage[1], 
                                  $coverage[0]);
        $a = $this->codepath;
        echo "[Step 2 of 2] Rendering per-test coverage...";
        $decorator->renderTestCoverage($this, $this->testpath, $a);
        echo "done\n";
    }
}
}
?>
<?php
namespace PEAR2\Pyrus\Developer\CoverageAnalyzer {
class Exception extends \Exception {}
}
?>
<?php
namespace PEAR2\Pyrus\Developer\CoverageAnalyzer {
class Sqlite
{
    protected $db;
    protected $totallines = 0;
    protected $coveredlines = 0;
    protected $pathCovered = array();
    protected $pathTotal = array();
    public $codepath;
    public $testpath;

    function __construct($path = ':memory:', $codepath = null, $testpath = null)
    {
        $this->db = new \Sqlite3($path);

        $sql = 'SELECT version FROM analyzerversion';
        if (@$this->db->querySingle($sql) == '2.1.0') {
            $this->codepath = $this->db->querySingle('SELECT codepath FROM paths');
            $this->testpath = $this->db->querySingle('SELECT testpath FROM paths');
            return;
        }
        if (!$codepath || !$testpath) {
            throw new Exception('Both codepath and testpath must be set in ' .
                                'order to initialize a coverage database');
        }
        $this->codepath = $codepath;
        $this->testpath = $testpath;
        // restart the database
        echo "Upgrading database to version 2.1.0\n";
        $this->db->exec('
            DROP TABLE coverage;
            DROP TABLE files;
            DROP TABLE tests;
            DROP TABLE coverage_per_file;
            DROP TABLE xdebugs;
            DROP TABLE analyzerversion;
            VACUUM;');

        $this->db->exec('BEGIN');

        $query = '
          CREATE TABLE coverage (
            files_id integer NOT NULL,
            tests_id integer NOT NULL,
            linenumber INTEGER NOT NULL,
            iscovered BOOL NOT NULL,
            issource BOOL NOT NULL,
            PRIMARY KEY (files_id, tests_id, linenumber)
          );
          CREATE INDEX coverage_files_id on coverage (files_id);
          CREATE INDEX coverage_tests_id on coverage (tests_id, issource);
          CREATE INDEX coverage_tests_id2 on coverage (tests_id, files_id, issource);
          CREATE INDEX coverage_linenumber on coverage (files_id, linenumber);
          CREATE INDEX coverage_issource on coverage (issource);

          CREATE TABLE coverage_per_file (
            files_id integer NOT NULL,
            linenumber INTEGER NOT NULL,
            coverage INTEGER NOT NULL,
            PRIMARY KEY (files_id, linenumber)
          );
          CREATE INDEX coverage_per_file_linenumber on coverage_per_file (linenumber);
          ';
        $worked = $this->db->exec($query);
        if (!$worked) {
            @$this->db->exec('ROLLBACK');
            $error = $this->db->lastErrorMsg();
            throw new Exception('Unable to create Code Coverage SQLite3 database: ' . $error);
        }

        $query = '
          CREATE TABLE files (
            id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
            filepath TEXT(500) NOT NULL,
            filepathmd5 TEXT(32) NOT NULL,
            issource BOOL NOT NULL,
            UNIQUE (filepath)
          );
          CREATE INDEX files_issource on files (issource);
          ';
        $worked = $this->db->exec($query);
        if (!$worked) {
            @$this->db->exec('ROLLBACK');
            $error = $this->db->lastErrorMsg();
            throw new Exception('Unable to create Code Coverage SQLite3 database: ' . $error);
        }

        $query = '
          CREATE TABLE xdebugs (
            xdebugpath TEXT(500) NOT NULL,
            xdebugpathmd5 TEXT(32) NOT NULL,
            PRIMARY KEY (xdebugpath)
          );';
        $worked = $this->db->exec($query);
        if (!$worked) {
            @$this->db->exec('ROLLBACK');
            $error = $this->db->lastErrorMsg();
            throw new Exception('Unable to create Code Coverage SQLite3 database: ' . $error);
        }

        $query = '
          CREATE TABLE tests (
            id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
            testpath TEXT(500) NOT NULL,
            testpathmd5 TEXT(32) NOT NULL,
            UNIQUE (testpath)
          );';
        $worked = $this->db->exec($query);
        if (!$worked) {
            @$this->db->exec('ROLLBACK');
            $error = $this->db->lastErrorMsg();
            throw new Exception('Unable to create Code Coverage SQLite3 database: ' . $error);
        }

        $query = '
          CREATE TABLE analyzerversion (
            version TEXT(5) NOT NULL
          );

          INSERT INTO analyzerversion VALUES("2.0.0");

          CREATE TABLE paths (
            codepath TEXT NOT NULL,
            testpath TEXT NOT NULL,
          );
          
          INSERT INTO paths ("' . $this->db->escapeString($codepath) . '", "' .
          $this->db->escapeString($testpath). '")';
        $worked = $this->db->exec($query);
        if (!$worked) {
            @$this->db->exec('ROLLBACK');
            $error = $this->db->lastErrorMsg();
            throw new Exception('Unable to create Code Coverage SQLite3 database: ' . $error);
        }
        $this->db->exec('COMMIT');
    }

    function retrieveLineLinks($file)
    {
        $id = $this->getFileId($file);
        $query = 'SELECT t.testpath, c.linenumber
            FROM
                coverage c, tests t
            WHERE
                c.files_id=' . $id . ' AND t.id=c.tests_id' ;
        $result = $this->db->query($query);
        if (!$result) {
            $error = $this->db->lastErrorMsg();
            throw new Exception('Cannot retrieve line links for ' . $file .
                                ' line #' . $line .  ': ' . $error);
        }

        $ret = array();
        while ($res = $result->fetchArray(SQLITE3_ASSOC)) {
            $ret[$res['linenumber']][] = $res['testpath'];
        }
        return $ret;
    }

    function retrieveTestPaths()
    {
        $query = 'SELECT testpath from tests ORDER BY testpath';
        $result = $this->db->query($query);
        if (!$result) {
            $error = $this->db->lastErrorMsg();
            throw new Exception('Cannot retrieve test paths :' . $error);
        }
        $ret = array();
        while ($res = $result->fetchArray(SQLITE3_NUM)) {
            $ret[] = $res[0];
        }
        return $ret;
    }

    function retrievePathsForTest($test, $all = 0)
    {
        $id = $this->getTestId($test);
        if ($all) {
            $query = 'SELECT DISTINCT filepath
                FROM coverage c, files
                WHERE c.tests_id=' . $id . '
                    AND files.id=c.files_id
                GROUP BY c.files_id
                ORDER BY filepath';
        } else {
            $query = 'SELECT DISTINCT filepath
                FROM coverage c, files
                WHERE c.tests_id=' . $id . '
                    AND c.issource=1
                    AND files.id=c.files_id
                GROUP BY c.files_id
                ORDER BY filepath';
        }
        $result = $this->db->query($query);
        if (!$result) {
            $error = $this->db->lastErrorMsg();
            throw new Exception('Cannot retrieve file paths for test ' . $test . ':' . $error);
        }
        $ret = array();
        while ($res = $result->fetchArray(SQLITE3_NUM)) {
            $ret[] = $res[0];
        }
        return $ret;
    }

    function retrievePaths($all = 0)
    {
        if ($all) {
            $query = 'SELECT filepath from files ORDER BY filepath';
        } else {
            $query = 'SELECT filepath from files WHERE issource=1 ORDER BY filepath';
        }
        $result = $this->db->query($query);
        if (!$result) {
            $error = $this->db->lastErrorMsg();
            throw new Exception('Cannot retrieve file paths :' . $error);
        }
        $ret = array();
        while ($res = $result->fetchArray(SQLITE3_NUM)) {
            $ret[] = $res[0];
        }
        return $ret;
    }

    function coveragePercentage($sourcefile, $testfile = null)
    {
        if ($testfile) {
            $coverage = $this->retrievePathCoverageByTest($sourcefile, $testfile);
        } else {
            $coverage = $this->retrievePathCoverage($sourcefile);
        }
        return round(($coverage[0] / $coverage[1]) * 100);
    }

    function retrieveProjectCoverage()
    {
        if ($this->totallines) {
            return array($this->coveredlines, $this->totallines);
        }
        $query = 'SELECT COUNT(linenumber),filepath FROM coverage_per_file, files
                WHERE files.id=coverage_per_file.files_id
                GROUP BY files_id';
        $result = $this->db->query($query);
        if (!$result) {
            $error = $this->db->lastErrorMsg();
            throw new Exception('Cannot retrieve coverage for ' . $path.  ': ' . $error);
        }
        while ($res = $result->fetchArray(SQLITE3_NUM)) {
            $this->pathTotal[$res[1]] = $res[0];
            $this->totallines += $res[0];
        }

        $query = 'SELECT COUNT(linenumber),filepath FROM coverage_per_file, files
                WHERE coverage > 0 AND files.id=coverage_per_file.files_id
                GROUP BY files_id';
        $result = $this->db->query($query);
        if (!$result) {
            $error = $this->db->lastErrorMsg();
            throw new Exception('Cannot retrieve coverage for ' . $path.  ': ' . $error);
        }
        while ($res = $result->fetchArray(SQLITE3_NUM)) {
            $this->pathCovered[$res[1]] = $res[0];
            $this->coveredlines += $res[0];
        }

        return array($this->coveredlines, $this->totallines);
    }

    function retrievePathCoverage($path)
    {
        if (!$this->totallines) {
            // set up the cache
            $this->retrieveProjectCoverage();
        }
        if (!isset($this->pathCovered[$path])) {
            return array(0, 0);
        }
        return array($this->pathCovered[$path], $this->pathTotal[$path]);
    }

    function retrievePathCoverageByTest($path, $test)
    {
        $id = $this->getFileId($path);
        $testid = $this->getTestId($test);

        $query = 'SELECT COUNT(linenumber)
            FROM coverage
            WHERE issource=1 AND files_id=' . $id . ' AND tests_id=' . $testid;
        $result = $this->db->query($query);
        if (!$result) {
            $error = $this->db->lastErrorMsg();
            throw new Exception('Cannot retrieve path coverage for ' . $path .
                                ' in test ' . $test . ': ' . $error);
        }

        $ret = array();
        while ($res = $result->fetchArray(SQLITE3_NUM)) {
            $total = $res[0];
        }

        $query = 'SELECT COUNT(linenumber)
            FROM coverage
            WHERE issource=1 AND iscovered AND files_id=' . $id. ' AND tests_id=' . $testid;
        $result = $this->db->query($query);
        if (!$result) {
            $error = $this->db->lastErrorMsg();
            throw new Exception('Cannot retrieve path coverage for ' . $path .
                                ' in test ' . $test . ': ' . $error);
        }

        $ret = array();
        while ($res = $result->fetchArray(SQLITE3_NUM)) {
            $covered = $res[0];
        }
        return array($covered, $total);
    }

    function retrieveCoverageByTest($path, $test)
    {
        $id = $this->getFileId($path);
        $testid = $this->getTestId($test);

        $query = 'SELECT iscovered as coverage, linenumber FROM coverage
                    WHERE issource=1 AND files_id=' . $id . ' AND tests_id=' . $testid;
        $result = $this->db->query($query);
        if (!$result) {
            $error = $this->db->lastErrorMsg();
            throw new Exception('Cannot retrieve test ' . $test .
                                ' coverage for ' . $path.  ': ' . $error);
        }

        $ret = array();
        while ($res = $result->fetchArray(SQLITE3_ASSOC)) {
            $ret[$res['linenumber']] = $res['coverage'];
        }
        return $ret;
    }

    function getFileId($path)
    {
        $query = 'SELECT id FROM files WHERE filepath=:filepath';
        $stmt = $this->db->prepare($query);
        $stmt->bindValue(':filepath', $path);
        if (!($result = $stmt->execute())) {
            throw new Exception('Unable to retrieve file ' . $path . ' id from database');
        }
        while ($id = $result->fetchArray(SQLITE3_NUM)) {
            return $id[0];
        }
        throw new Exception('Unable to retrieve file ' . $path . ' id from database');
    }

    function getTestId($path)
    {
        $query = 'SELECT id FROM tests WHERE testpath=:filepath';
        $stmt = $this->db->prepare($query);
        $stmt->bindValue(':filepath', $path);
        if (!($result = $stmt->execute())) {
            throw new Exception('Unable to retrieve test file ' . $path . ' id from database');
        }
        while ($id = $result->fetchArray(SQLITE3_NUM)) {
            return $id[0];
        }
        throw new Exception('Unable to retrieve test file ' . $path . ' id from database');
    }

    function removeOldTest($testpath, $xdebugpath)
    {
        try {
            $id = $this->getTestId($testpath);
        } catch (\Exception $e) {
            // get a unique ID
            return $this->db->querySingle('SELECT COUNT(id) from tests')+1;
        }
        $this->db->exec('DELETE FROM tests WHERE id=' . $id);
        $this->db->exec('DELETE FROM coverage WHERE tests_id=' . $id);
        $this->db->exec('DELETE FROM xdebugs WHERE xdebugpath="' . $this->db->escapeString($xdebugpath) . '"');
        return $id;
    }

    function addTest($testpath)
    {
        $id = $this->removeOldTest($testpath, str_replace('.phpt', '.xdebug', $testpath));
        $query = 'INSERT INTO tests
            (testpath, testpathmd5)
            VALUES(:testpath, :md5)';
        $stmt = $this->db->prepare($query);
        $stmt->bindValue(':testpath', $testpath);
        $md5 = md5_file($testpath);
        $stmt->bindValue(':md5', $md5);
        $stmt->execute();
        $id = $this->db->lastInsertRowID();

        $query = 'INSERT INTO xdebugs
            (xdebugpath, xdebugpathmd5)
            VALUES(:testpath, :md5)';
        $stmt = $this->db->prepare($query);
        $stmt->bindValue(':testpath', str_replace('.phpt', '.xdebug', $testpath));
        $md5 = md5_file(str_replace('.phpt', '.xdebug', $testpath));
        $stmt->bindValue(':md5', $md5);
        $stmt->execute();
        return $id;
    }

    function unChangedXdebug($path)
    {
        $query = 'SELECT xdebugpathmd5 FROM xdebugs
                    WHERE xdebugpath=:path';
        $stmt = $this->db->prepare($query);
        $stmt->bindValue(':path', $path);
        $result = $stmt->execute();
        if (!$result) {
            return false;
        }
        $md5 = 0;
        while ($res = $result->fetchArray(SQLITE3_NUM)) {
            $md5 = $res[0];
        }
        if (!$md5) {
            return false;
        }
        if ($md5 == md5_file($path)) {
            return true;
        }
        return false;
    }

    function addFile($filepath, $issource = 0)
    {
        $query = 'SELECT id FROM files WHERE filepath=:filepath';
        $stmt = $this->db->prepare($query);
        $stmt->bindParam(':filepath', $filepath);
        if (!($result = $stmt->execute())) {
            throw new Exception('Unable to add file ' . $filepath . ' to database');
        }
        while ($id = $result->fetchArray(SQLITE3_NUM)) {
            $query = 'UPDATE files SET filepathmd5=:md5 WHERE filepath=:filepath';
            $stmt = $this->db->prepare($query);
            $stmt->bindParam(':filepath', $filepath);
            $md5 = md5_file($filepath);
            $stmt->bindParam(':md5', $md5);
            if (!$stmt->execute()) {
                throw new Exception('Unable to update file ' . $filepath . ' md5 in database');
            }
            return $id[0];
        }
        $stmt->clear();
        $query = 'INSERT INTO files
            (filepath, filepathmd5, issource)
            VALUES(:testpath, :md5, :issource)';
        $stmt = $this->db->prepare($query);
        $stmt->bindValue(':testpath', $filepath);
        $md5 = md5_file($filepath);
        $stmt->bindValue(':md5', $md5);
        $stmt->bindValue(':issource', $issource);
        if (!$stmt->execute()) {
            throw new Exception('Unable to add file ' . $filepath . ' to database');
        }
        return $this->db->lastInsertRowID();
    }

    function getTotalCoverage($file, $linenumber)
    {
        $query = 'SELECT coveragecount FROM coverage_per_file
                    WHERE files_id=' . $this->getFileId($file) . ' AND linenumber=' . $linenumber;
        $result = $this->db->query($query);
        if (!$result) {
            return false;
        }
        $coverage = 0;
        while ($res = $result->fetchArray(SQLITE3_NUM)) {
            $coverage = $res[0];
        }
        return $coverage;
    }

    function retrieveCoverage($path)
    {
        $id = $this->getFileId($path);
        $query = 'SELECT coverage, linenumber FROM coverage_per_file
                    WHERE files_id=' . $id . '
                    ORDER BY linenumber ASC';
        $result = $this->db->query($query);
        if (!$result) {
            $error = $this->db->lastErrorMsg();
            throw new Exception('Cannot retrieve coverage for ' . $path.  ': ' . $error);
        }

        $ret = array();
        while ($res = $result->fetchArray(SQLITE3_ASSOC)) {
            $ret[$res['linenumber']] = $res['coverage'];
        }
        return $ret;
    }

    /**
     * This is used to get the coverage which is then inserted into our
     * intermediate coverage_per_file table to speed things up at rendering
     */
    function retrieveSlowCoverage($id)
    {
        $query = 'SELECT SUM(iscovered) as coverage, linenumber FROM coverage WHERE files_id=' . $id . '
                    GROUP BY linenumber';
        $result = $this->db->query($query);
        if (!$result) {
            $error = $this->db->lastErrorMsg();
            throw new Exception('Cannot retrieve coverage for ' . $path.  ': ' . $error);
        }

        $ret = array();
        while ($res = $result->fetchArray(SQLITE3_ASSOC)) {
            $ret[$res['linenumber']] = $res['coverage'];
        }
        return $ret;
    }

    function updateTotalCoverage()
    {
        $query = 'DELETE FROM coverage_per_file';
        $this->db->exec($query);
        echo "Updating coverage per-file intermediate table\n";
        foreach ($this->retrievePaths() as $path) {
            echo ".";
            $id = $this->getFileId($path);
            foreach ($this->retrieveSlowCoverage($id) as $linenumber => $coverage) {
                $query = 'INSERT INTO coverage_per_file
                    (files_id, coverage, linenumber)
                    VALUES(' . $id . ',' . $coverage . ',' . $linenumber .')';
                $this->db->exec($query);
            }
        }
        echo "done\n";
    }

    function addCoverage($testpath, $testid, $xdebug)
    {
        foreach ($xdebug as $path => $results) {
            if (!file_exists($path)) {
                continue;
            }
            if (strpos($path, $this->codepath) !== 0) {
                $issource = 0;
            } else {
                if (strpos($path, $this->testpath) === 0) {
                    $issource = 0;
                } else {
                    $issource = 1;
                }
            }
            echo ".";
            $id = $this->addFile($path, $issource);
            foreach ($results as $line => $info) {
                if ($info > 0) {
                    $res = 1;
                } else {
                    $res = 0;
                }
                $query = 'INSERT INTO coverage
                    (files_id, tests_id, linenumber, iscovered, issource)
                    VALUES(' . $id . ', ' . $testid . ', ' . $line . ', ' . $res . ',' . $issource . ')';

                $worked = $this->db->exec($query);
                if (!$worked) {
                    $error = $this->db->lastErrorMsg();
                    throw new Exception('Cannot add coverage for test ' . $testpath .
                                        ', covered file ' . $path . ': ' . $error);
                }
            }
        }
    }

    function begin()
    {
        $this->db->exec('BEGIN');
    }

    function commit()
    {
        $this->db->exec('COMMIT');
    }

    /**
     * Retrieve a list of .phpt tests that either have been modified,
     * or the files they access have been modified
     * @return array
     */
    function getModifiedTests()
    {
        $modifiedPaths = array();
        $modifiedTests = array();
        $paths = $this->retrievePaths(1);
        echo "Scanning ", count($paths), " source files";
        foreach ($paths as $path) {
            echo '.';
            $query = '
                SELECT id, filepathmd5 FROM files where filepath="' .
                $this->db->escapeString($path) . '"';
            $result = $this->db->query($query);
            while ($res = $result->fetchArray(SQLITE3_ASSOC)) {
                if (md5_file($path) == $res['filepathmd5']) {
                    break;
                }
                $modifiedPaths[] = $path;
                // file is modified, get a list of tests that execute this file
                $query = '
                    SELECT t.testpath
                    FROM coverage c, tests t
                    WHERE
                        c.files_id=' . $res['id'] . '
                        AND t.id=c.tests_id';
                $result2 = $this->db->query($query);
                while ($res = $result2->fetchArray(SQLITE3_NUM)) {
                    $modifiedTests[$res[0]] = true;
                }
                break;
            }
        }
        echo "done\n";
        echo count($modifiedPaths), ' modified files resulting in ',
            count($modifiedTests), " modified tests\n";
        $paths = $this->retrieveTestPaths();
        echo "Scanning ", count($paths), " test paths";
        foreach ($paths as $path) {
            echo '.';
            $query = '
                SELECT id, testpathmd5 FROM tests where testpath="' .
                $this->db->escapeString($path) . '"';
            $result = $this->db->query($query);
            while ($res = $result->fetchArray(SQLITE3_ASSOC)) {
                if (md5_file($path) != $res['testpathmd5']) {
                    $modifiedTests[$path] = true;
                }
            }
        }
        echo "done\n";
        echo count($modifiedTests), " tests should be re-run\n";
        return array_keys($modifiedTests);
    }
}
}
?><?php
namespace PEAR2\Pyrus\Developer\CoverageAnalyzer\SourceFile {
use PEAR2\Pyrus\Developer\CoverageAnalyzer\Aggregator,
    PEAR2\Pyrus\Developer\CoverageAnalyzer\AbstractSourceDecorator;
class PerTest extends \PEAR2\Pyrus\Developer\CoverageAnalyzer\SourceFile
{
    protected $testname;

    function __construct($path, Aggregator $agg, $testpath, $sourcepath, $testname)
    {
        $this->testname = $testname;
        parent::__construct($path, $agg, $testpath, $sourcepath);
    }

    function setCoverage()
    {
        $this->coverage = $this->aggregator->retrieveCoverageByTest($this->path, $this->testname);
    }

    function coveredLines()
    {
        $info = $this->aggregator->coverageInfoByTest($this->path, $this->testname);
        return $info[0];
    }

    function render(AbstractSourceDecorator $decorator = null)
    {
        if ($decorator === null) {
            $decorator = new DefaultSourceDecorator('.');
        }
        return $decorator->render($this, $this->testname);
    }

    function coveragePercentage()
    {
        return $this->aggregator->coveragePercentage($this->path, $this->testname);
    }
}
}
?>
<?php
namespace PEAR2\Pyrus\Developer\CoverageAnalyzer {
$class = "PEAR2\Pyrus\Developer\CoverageAnalyzer\Web\View";
var_dump(str_replace("PEAR2\Pyrus\Developer\CoverageAnalyzer", "", $class));
$view = new Web\View;
$rooturl = $_SERVER["REQUEST_URI"];
$controller = new Web\Controller($view, $rooturl);
$controller->route();
}нl�]d�nZ����G�|��   GBMB