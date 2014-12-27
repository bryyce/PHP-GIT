<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */


/**
 * Description of 0
 *
 * @author bryyce
 */
class Migration_0 extends \Lib\Migration\Migration {

    public function up() {
        $table = new Lib\Migration\MigrationTable('users');
        $table->addColumn('login', 'varchar(50)')
                ->addColumn('mail', 'varchar(50)')
                ->addColumn('password', 'varchar(40)')
                ->addColumn('salt', 'varchar(40)')
                ->withTime()
                ->create();
    }
    
    public function down() {
        Lib\Migration\MigrationTable::drop('users');
    }
}
