<?php

/**
 * include.php file
 * @filesource ./include.php
 */
/**
 *	Fonction qui gere le chargement automatique des classes
 * @param string $class_name Nom de la classe a charger
 * @return void
 */
error_reporting(E_ALL);

/**
 * 	Handler d'exception non attrapée
 *
 * @param object $exception  L'Exception non attrappée
 */
function exceptionHandler($exception) {
	echo "<pre>\n";
	echo $exception->getMessage() . "\n";
	echo $exception->getTraceAsString() . "\n";
	echo "</pre>\n";
}
function getBasePath() {
	return '/var/www/PHP-GIT/';
}
set_exception_handler('exceptionHandler');

/**
 * 	Présent le var_dump d'une var
 *
 * @param mixed $var la variable à afficher
 */
function dump($var) {
	echo "<pre>\n";
	var_dump($var);
	echo "</pre>\n";
}

function __autoload($class_name) {
	//class directories
	require_once(getBasePath(). str_replace('\\', '/', $class_name) . '.class');
	if (strpos($class_name, 'Model\\') === 0)
		$class_name::loadRelations();
	//only require the class once, so quit after to save effort (if you got more, then name them something else
	return;
}
require_once getBasePath().'Config/routes.config';
?>
