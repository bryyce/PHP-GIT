<?php

function link_to($url, $name, $method = 'GET') {
	$class = '';
	if ($method != 'GET') {
		$class = 'rest';
	}
	return '<a href="' . $url . '" data-method="' . $method . '"  class=\'' . $class . ' btn btn-sm btn-primary\'>' . $name . '</a>';
}

function form ($name, $url, $method = 'POST', $params = []) {
    $form = new Lib\Form\Form($name, $url, $method, $params);
    echo '<form action="'. $form->getAction() . '" method="'. $form->getMethod() .'">';
    if (in_array($form->getMethod(),['GET','POST'])) {
        echo '<input type="hidden" name="_method" value="'. $form->getMethod() .'"/>';
    }
    yield $form;
    echo '</form>';
}

function i18n ($string) {
    return $string;
}

function h($string) {
    return htmlentities($string);
}

function show_errors($model) {
    if (count($model->errors) > 0) {
        echo '<div class="alert alert-danger" role="alert">';
        echo '<a href="#" class="close" data-dismiss="alert">&times;</a><strong>There are some errors</strong>';
        echo "<ul>";
        foreach ($model->errors as $error) {
            echo "<li class='error'>$error</li>";
        }
        echo "</ul>";
        echo '</div>';
    }
}