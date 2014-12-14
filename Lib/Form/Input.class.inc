<?php
namespace Lib\Form;

/**
 * Description of Input
 *
 * @author bryyce
 */
abstract class Input {
    protected $_attributes = [];
    
    public function __construct ($attributes = []) {
        $this->_attributes = $attributes;
    }
    
    public function getHtmlId () {
        return isset($this->_attributes['id'])? $this->_attributes['id']: '';
    }


    public function render () {
        return require(getLibBasePath() . 'View/input.phtml');
    }
}
