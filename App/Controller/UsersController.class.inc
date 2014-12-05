<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

namespace App\Controller;
use Lib, App\Model;
/**
 * Description of Controller
 *
 * @author bryyyce
 */
class UsersController extends Lib\Controller {
	public function _index () {
		$users = Model\User::all();
        $this->_view->setVars(array('users' => $users));
        $this->_view->render();
	}

	public function _show ($params) {
		$user = new Model\User($params['id']);
        $this->_view->setVars(array('user' => $user));
        $this->_view->render();
	}

    public function _new() {
        $user = new Model\User();
        $this->_view->setVars(array('user' => $user));
        $this->_view->render();
    }

    public function _create ($params) {
        $user = new Model\User();
        $this->redirectTo(\users_path($user));
    }

    public function _edit ($params) {
        $user = new Model\User($params['id']);
        $this->_view->setVars(array('user' => $user));
        $this->_view->render();
    }

    public function _update ($params) {
        $user = new Model\User($params['id']);
        $this->redirectTo(\users_path($user));
    }

    public function _delete ($params) {
        $user = new Model\User($params['id']);
        $user->delete();
        $this->redirectTo(\users_path());
    }
}