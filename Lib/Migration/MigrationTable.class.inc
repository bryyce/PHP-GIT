<?php
namespace Lib\Migration;

/**
 * Description of MigrationTable
 *
 * @author bryyce
 */
class MigrationTable {
    private $_table_name;
    private $_columns;
    private $_engine;
    private $_options;


    public function __construct($table_name, $engine = 'InnoDB', $options = []) {
        $this->_table_name = $table_name;
        $this->_engine = $engine;
        $this->_options = $options;
        $this->_columns = new \Collection();
    }
    
    public function create() {
        \Lib\ORM\QueryManager::get()->createTable($this->_table_name, $this->_columns, $this->_engine, $this->_options);
    }
    
    public function addColumn($column_name, $column_type, $options = []) {
        $this->_columns->push(new MigrationColumn($column_name, $column_type, $options));
        return $this;
    }
    
    public function withTime() {
        $this->addColumn('created_by', 'int')
                ->addColumn('created_at', 'datetime')
                ->addColumn('updated_by', 'int')
                ->addColumn('updated_at', 'datetime');
        return $this;
    }
    
    public static function drop($table_name) {
        \Lib\ORM\QueryManager::get()->dropTable($table_name);
    }
}
