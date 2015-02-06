<?php

namespace Lib\Tool;

/**
 * Description of HttpRequest
 *
 * @author bryyce
 */

class HTTPRequest
{
	public static function cookieData($key)
	{
		return isset($_COOKIE[$key]) ? $_COOKIE[$key] : null;
	}

	public static function cookieExists($key)
	{
		return isset($_COOKIE[$key]);
	}

	public static function getData($key)
	{
		return isset($_GET[$key]) ? $_GET[$key] : null;
	}

	public static function getExists($key)
	{
		return isset($_GET[$key]);
	}

	public static function method()
	{
		return $_SERVER['REQUEST_METHOD'];
	}

	public static function postData($key)
	{
		return isset($_POST[$key]) ? $_POST[$key] : null;
	}

	public static function postExists($key)
	{
		return isset($_POST[$key]);
	}

	public static function requestURI()
	{
        return strtok($_SERVER['REQUEST_URI'][0] == '/' ? substr($_SERVER['REQUEST_URI'], 1):$_SERVER['REQUEST_URI'], '?');
	}
    
    public static function getParams() 
    {
        return "_". (in_array(self::method(), array('PUT', 'DELETE'))? 'POST' : self::method());
    }
}
