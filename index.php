<?php
/**
 * index.php file
 * @filesource ./index.php
 * @package        \
 */
require_once('include.php');
use App\Controller,
    Lib\Tool\Router as Router,
    Lib\Tool\HTTPRequest as HTTPRequest;

ob_start('ob_gzhandler');
if (($result = Router::get()->getCurrentRoute(HTTPRequest::requestURI(),HTTPRequest::method())) !== FALSE) {
    $class = "App\\Controller\\".ucfirst($result['controller'])."Controller";
    $controller = new $class;
    $controller->setAction($result['action']);
    //echo filter_input_array(INPUT_GET, FILTER_SANITIZE_STRING);
    $controller->run($result['params'] + $_POST + $_GET/* ${HTTPRequest::getParams()}*/);
   // echo round(memory_get_usage()/(1024),2)." Ko";
} else {
    echo 'ici';
}