<?php
//print_r(Model\Membre::All());
print("------------<br>");
$user = Model\User::first();
print_r($user->profiles[0]->users[0]);
$user->id = 0;
$user->save();
print_r($user);
echo '<br>';
$c = new Collection(array(1,2,3,4));
echo $c->next();
echo '<br>';
echo $c->next();
echo '<br>';
echo $c->next();
echo '<br>';
echo $c->next();
echo '<br>';
echo $c->next();
echo '<br>';

echo $c->remove(2);
echo '<br>';
print_r($c);
echo round(memory_get_usage()/(1024),2)." Ko";
?>