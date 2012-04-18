<?php
print_r(Model\Membre::All());
print("------------<br>");
//print_r(Membre::first());
//print_r(Model\Membre::findByPaypal(""));
print_r(Model\Membre::first()->status);
?>
