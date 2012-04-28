<?php
//print_r(Model\Membre::All());
print("------------<br>");
$user = Model\User::first();
print_r($user->profiles);
echo round(memory_get_usage()/(1024),2)." Ko";
?>