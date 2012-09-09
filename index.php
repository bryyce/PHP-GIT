<?php
/**
 * index.php file
 * @filesource ./index.php 
 */
//print_r(Model\Membre::All());
print("------------<br>");
$user = Model\User::all();
print_r($user);
echo '<br><br>';
$user = Model\User::first();
print_r($user->profiles[0]->users[0]);
echo '<br><br>';
$user->login = 'abc';
$user->save();
print_r($user);
echo '<br><br>';
$user = new Model\User(1);
print_r($user);
echo '<br><br>';
echo round(memory_get_usage()/(1024),2)." Ko";
?>
