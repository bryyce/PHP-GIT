<?php
    function __autoload($class_name)
    {
//        echo $class_name . '<br>';
        //class directories
            require_once($class_name . '.class');
        //only require the class once, so quit after to save effort (if you got more, then name them something else
        return;
    }
?>
