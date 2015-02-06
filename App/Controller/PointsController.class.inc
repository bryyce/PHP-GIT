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
class PointsController extends Lib\Controller
{
    protected function _index ()
    {
        echo Model\Point::find(['viewed' => 'FALSE'])->toJson();
        $res = Lib\ORM\myPDO::get()->prepare("UPDATE points SET viewed = TRUE WHERE viewed = FALSE");
        $res->execute();
        $this->_is_rendered = TRUE;
    }

    protected function _show ($params)
    {
        $user = Model\User::findById($params['id']);
        $this->_view->setVars(array('user' => $user));
    }

    protected function _new()
    {
        $user = new Model\User();
        $this->_view->setVars(array('user' => $user));
    }

    protected function _create ($params)
    {
        print_r($params);
        $points_data = json_decode($params['points'], TRUE);
        foreach ($points_data as $point_data) {
            $point_data['datetime'] = date('Y-m-d H:i:s.u', $point_data['datetime']);
            (new Model\Point($point_data))->save();
        }
        $this->_is_rendered = TRUE;
    }

    protected function _edit ($params)
    {
        $user =  Model\User::findById($params['id']);
        $this->_view->setVars(array('user' => $user));
    }

    protected function _update ($params)
    {
        $user =  Model\User::findById($params['id']);
        $this->redirectTo(\users_path($user));
    }

    protected function _delete ($params)
    {
        $user =  Model\User::findById($params['id']);
        $user->delete();
        $this->redirectTo(\users_path());
    }
}
