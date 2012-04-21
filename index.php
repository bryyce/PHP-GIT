<?php
//print_r(Model\Membre::All());
print("------------<br>");
$m =Model\Membre::first();
print_r($m->status->membre->status);
echo round(memory_get_usage()/(1024),2)." Ko";
?>
