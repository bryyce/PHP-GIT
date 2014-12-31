<?php
namespace Lib\Form;

/**
 * Description of Input
 *
 * @author bryyce
 */
abstract class Input {
    protected $_attributes = [];
    protected $_name;
    protected $_value;
    public function __construct ($name, $value, $attributes = []) {
        $this->_name = $name;
        $this->_value = $value;
        $this->_attributes = $attributes;
    }

    public function render () {
        return eval(file_get_contents(getLibBasePath() . 'View/input.phtml'));
    }
}
