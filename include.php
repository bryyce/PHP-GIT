<?php
function __autoload($class_name) {
    require_once dirname(__FILE__).'/'. $class_name . 'class.php';

}
?>
