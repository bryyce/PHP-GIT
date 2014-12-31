<?php
namespace Lib\Form;
/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 * Description of HiddenInput
 *
 * @author bryyce
 */
class HiddenInput extends Input {
    public function __construct($name, $value, $attributes = array()) {
        parent::__construct($name, $value, ['type' => 'hidden'] + $attributes);
    }
}
