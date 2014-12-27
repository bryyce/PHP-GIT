<?php
namespace Lib\Migration;
/**
 * Description of MigrationColumn
 *
 * @author bryyce
 */
class MigrationColumn {
    private $_name;
    private $_type;
    private $_options;
    
    private static $_default_options = [
      'default_value' => ''  
    ];


    public function __construct($name, $type, $options = []) {
        $this->_name = $name;
        $this->_type = $type;
        $this->_options = $options + self::$_default_options;
    }
    
    public function getName() {
        return $this->_name;
    }
    
    public function getType() {
        return $this->_type;
    }
    
    public function getOption($option) {
        return $this->_options[$option];
    }
}