<?php

/**
 * include.php file
 * @filesource ./include.php
 */
/**
 *    Fonction qui gere le chargement automatique des classes
 * @param string $class_name Nom de la classe a charger
 * @return void
 */
error_reporting(E_ALL);
ob_start('ob_gzhandler');

/**
 *     Handler d'exception non attrapée
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
function getAppBasePath(){
    return getBasePath() . 'App/';
}
function getViewsBasePath(){
    return getAppBasePath() . 'Views/';
}
function getLibBasePath(){
    return getBasePath() . 'Lib/';
}
set_exception_handler('exceptionHandler');

/**
 *     Présent le var_dump d'une var
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
    require_once(getBasePath(). str_replace('\\', '/', $class_name) . '.class.inc');
    if (strpos($class_name, 'App\\Model\\') === 0)
        $class_name::loadRelations();
    if (strpos($class_name, 'App\\Controller\\') === 0){
        $function = $class_name::getControllerPrefix() . '_path';
        if ($function != '_path') {
            eval('function ' . $function . '($model = NULL){
                if (!is_null($model)) {
                    return "http://localhost/'.$class_name::getControllerPrefix().'/{$model->id}/";
                } else {
                    return "http://localhost/'.$class_name::getControllerPrefix().'/";
                }
            }');
            eval('function new_' . $function . '(){
                return "http://localhost/'.$class_name::getControllerPrefix().'/new/";
            }');
            eval('function edit_' . $function . '($model){
                if (!is_null($model)) {
                    return "http://localhost/'.$class_name::getControllerPrefix().'/edit/{$model->id}/";
                }
            }');
        }
    }
    //only require the class once, so quit after to save effort (if you got more, then name them something else
    return;
}
require_once getBasePath().'Config/routes.config';
?>
