<?php
namespace App\Controller;

use Lib;
use App\Model;

/**
 * Description of Controller
 *
 * @author   bryyce <bric.robin@gmail.com>
 * @category Class
 * @package  App\Controller
 * @license     http://www.gnu.org/copyleft/gpl.html GNU General Public License
 */
class UsersController extends Lib\Controller
{
    protected function _index ()
    {
        $users = Model\User::all();
        $this->_view->setVars(array('users' => $users));
    }

    protected function _show ($params)
    {
        $user = new Model\User($params['id']);
        $this->_view->setVars(array('user' => $user));
    }

    protected function _new()
    {
        $user = new Model\User();
        $this->_view->setVars(array('user' => $user));
    }

    protected function _create ($params)
    {
        $user = new Model\User();
        $this->redirectTo(\users_path($user));
    }

    protected function _edit ($params)
    {
        $user = new Model\User($params['id']);
        $this->_view->setVars(array('user' => $user));
    }

    protected function _update ($params)
    {
        $user = new Model\User($params['id']);
        $this->redirectTo(\users_path($user));
    }

    protected function _delete ($params)
    {
        $user = new Model\User($params['id']);
        $user->delete();
        $this->redirectTo(\users_path());
    }
}
