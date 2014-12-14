<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

namespace Lib\Form;

/**
 * Description of RadioInput
 *
 * @author bryyce
 */
class RadioInput extends Input {
    public function __construct($attributes = array()) {
        parent::__construct(['type' => 'radio'] + $attributes);
    }
}
