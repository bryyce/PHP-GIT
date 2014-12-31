<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

namespace Lib\Form;

/**
 * Description of TextInput
 *
 * @author bryyce
 */
class SubmitInput extends Input {
    public function __construct($name, $value, $attributes = array()) {
        parent::__construct($name, $value, ['type' => 'submit'] + $attributes);
    }
}
