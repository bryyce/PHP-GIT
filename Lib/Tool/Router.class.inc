<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

namespace Lib\Tool;
/**
 * Description of Router
 *
 * @author bryyyce
 */
class Router {
    private static $_router = NULL;
    //put your code here
    private $_routes = [];

    /**
     * créer une instance PDOStatement si elle existe pas
     *
     *    @access private
     * @final
     */
    final private function __construct() {
    }

    /**
     * Destructeur
     * @final
     */
    final public function __destruct() {
        if (!is_null(self::$_router)) {
            self::$_router = null;
        }
    }

    /**
     * Récuperer le singleton
     * @return \Tool\Router l'instance courante de Router
     * @final
     */
    final public static function get() {
        // Une instance est-elle disponible ?
        if (!isset(self::$_router))
            self::$_router = new Router();
        return self::$_router;
    }

    public function getCurrentRoute($request_url, $request_method = 'GET'){
        foreach ($this->_routes as $route) {
            if ($request_method == $route->Get('method')) {
                $route_name = preg_replace('/\:([a-zA-Z]*)/', '([a-zA-Z0-9]*)', $route->Get('name'));
                if (preg_match('|^'.$route_name.'$|', $request_url, $vars) !== 0) {
                    preg_match_all('/\:([a-zA-Z]*)/', $route->Get('name'), $vars_name);
                    $params = array();
                    unset($vars[0]);
                    if (count($vars) > 0) {
                            $params += array_combine($vars_name[1], $vars);
                    }
                    //unset($vars_name[0]);
                    return ['controller' => $route->Get('controller'), 'action' => $route->Get('action'), 'params' => $params];
                }
            }
        }
        return FALSE;
    }

    public function match($route, $action, $params = []) {
        $params += array('method' => 'GET');
        list($controller, $action) = explode('#', $action);
        $this->_routes[]= new Route($params['method'], $route, $controller, $action);
    }

    public function resource($resource, $params = []) {
        $this->match($resource . '/',               $resource . '#index',      $params + array('method' => 'GET'));
        $this->match($resource . '/new/',        $resource . '#new',        $params + array('method' => 'GET'));
        $this->match($resource . '/',           $resource . '#create',     $params + array('method' => 'POST'));
        $this->match($resource . '/:id/',        $resource . '#show',    $params + array('method' => 'GET'));
        $this->match($resource . '/edit/:id/',  $resource . '#edit',    $params + array('method' => 'GET'));
        $this->match($resource . '/:id/',          $resource . '#update',     $params + array('method' => 'PUT'));
        $this->match($resource . '/:id/',         $resource . '#delete',     $params + array('method' => 'DELETE'));
    }

}

class Route {
    private $_method;
    private $_name;
    private $_controller;
    private $_action;

    public function __construct($method, $name, $controller, $action)
    {
        $this->_method         = $method;
        $this->_name        = $name;
        $this->_controller     = $controller;
        $this->_action         = $action;
    }

    public function get($variable) {
        return $this->{"_".$variable};
    }
}
