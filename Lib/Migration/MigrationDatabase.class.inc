<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

namespace lib\migration;

/**
 * Description of MigrationDatabase
 *
 * @author bryyce
 */
class MigrationDatabase {
    public function __construct($db_name) {
        \Lib\ORM\QueryManager::get()->createDatabase($db_name);
    }
}