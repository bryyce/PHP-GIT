<?php
require_once 'class/Config.class.php';
require_once 'class/Enregistrement.class.php';
require_once 'class/MyEnregistrement.class.php';
require_once 'class/Membre.class.php';

print_r(Membre::All());
print("------------<br>");
//print_r(Membre::first());
print_r(Membre::findByPaypal(""));
?>
