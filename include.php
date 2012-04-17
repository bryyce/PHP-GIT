<?php
    function __autoload($class_name)
    {
        echo $class_name.'<br>';
        //exc
        //class directories
        $directorys = array(
            './ORM',
            './Model',
            './',
        );
       
        //for each directory
        foreach($directorys as $directory)
        {
            //see if the file exsists
            if(file_exists($directory.$class_name . '.class'))
            {
                require_once($directory.$class_name . '.class');
                //only require the class once, so quit after to save effort (if you got more, then name them something else
                return;
            }           
        }
    }
?>
