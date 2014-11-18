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
function __autoload($class_name) {
	//class directories
	require_once($class_name . '.class');
	if (strpos($class_name, 'Model\\') === 0)
		$class_name::loadRelations();
	//only require the class once, so quit after to save effort (if you got more, then name them something else
	return;
}
require_once 'Tool/Rooter.class';
?>
