<?php
/**
 * index.php file
 * @filesource ./index.php
 * @package		\
 */
use App\Controller,
	Lib\Tool\Router as Router,
	Lib\Tool\HTTPRequest as HTTPRequest;

ob_start('ob_gzhandler');
$result = Router::get()->getCurrentRoute(HTTPRequest::requestURI(),HTTPRequest::method());
$class = "App\\Controller\\".ucfirst($result['controller'])."Controller";
$controller = new $class;
$controller->setAction($result['action']);
$controller->run($result['params'] + ${"_".HTTPRequest::method()});
echo round(memory_get_usage()/(1024),2)." Ko";