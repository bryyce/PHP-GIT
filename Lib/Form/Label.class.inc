<?php
namespace Lib\Form;

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 * Description of Label
 *
 * @author bryyce
 */
class Label extends Input {
    protected $_label = '';
    public function __construct($_label, $attributes = array()) {
        $this->_label = $_label;
        parent::__construct($attributes);
    }

    public function render() {
        return require(getLibBasePath() . 'View/label.phtml');
    }
}
