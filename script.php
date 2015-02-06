<?php
require_once 'include.php';
if ($argv[1] == 'migrate') {
    $schema = json_decode(file_get_contents("/home/bryyce/PHP-GIT/Config/schema.json"), TRUE);
    $target_version = isset($argv[2])? (int)str_replace('VERSION=', '', $argv[2]) :9999999999999999999999999999;
    $method = (int) $schema['version'] < $target_version  ? 'up' : 'down';
    $files = scandir(getAppBasePath() . 'Script/Migration/', $method == 'down');
    foreach ($files as $id => $filename) {
        if (strpos($filename, '.php') > 0 ) {
            $migration_file_number = explode('.php', $filename)[0];
            if ((
                    $method == 'up'
                    && $migration_file_number > $schema['version']
                    && $migration_file_number <  $target_version
                ) || (
                    $method == 'down'
                    && $migration_file_number <= $schema['version']
                    && $migration_file_number >=  $target_version
                )) {
                \Lib\ORM\QueryManager::get()->beginTransaction();
                try {
                    require_once 'App/Script/Migration/' . $migration_file_number . '.php';
                    $class ='Migration_'.$migration_file_number;
                    $migration = new $class;
                    $migration->{$method}();
                    \Lib\ORM\QueryManager::get()->commit();
                    if ($method == 'up') {
                        $schema['version'] = $migration_file_number;
                    } else {
                        if (isset($files[$id + 1]) && strpos($files[$id + 1], '.php') > 0) {
                            $schema['version'] = explode('.php', $files[$id])[0];
                        } else {
                            $schema['version'] =  -1;
                        }
                    }
                    file_put_contents("/home/bryyce/PHP-GIT/Config/schema.json", json_encode($schema));
                } catch (\Exception $ex) {
                    \Lib\ORM\QueryManager::get()->rollback();
                    throw $ex;
                }
            }
        }
    }
}
