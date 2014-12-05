<?php

function link_to($url, $name, $method = 'GET') {
	$class = '';
	if ($method != 'GET') {
		$class = 'rest';
	}
	return '<a href="' . $url . '" data-method="' . $method . '"  class=\'' . $class . ' btn btn-sm btn-primary\'>' . $name . '</a>';
}