<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

namespace Lib;
/**
 * Description of Controller
 *
 * @author bryyyce
 */
class Controller {
	protected $_view = null;

	public function __construct(){
		$this->_view = new View();
	}

	public function setAction ($action) {
		$this->_action = $action;
	}

	protected function beforeFilter () {

	}

	protected function afterFilter () {

	}

	public function run ($params) {
		try {
			if (method_exists($this, '_' . $this->_action)){
				$this->_view->setDefaultView(self::getControllerPrefix().'/'.$this->_action);
				$this->beforeFilter();
				$this->{'_'.$this->_action}($params);
				$this->afterFilter();
			} else {
				header("HTTP/1.1 404 Not Found");
			}
		} catch(\Exception\NotFoundException $e) {
			header("HTTP/1.1 404 Not Found");
			echo 'Not Found';
			die;
		}
	}

	protected function render($params = []) {
		require_once 'Views/layout.phtml';
	}

	public static function getControllerPrefix() {
		return strtolower(str_replace('Controller','',str_replace('App\\Controller\\', '', get_called_class())));
	}

	public function redirectTo($url) {
		header("HTTP/1.1 307 Temporary Redirect");
		echo $url;
		die;
	}
}
