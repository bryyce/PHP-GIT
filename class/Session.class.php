<?php
/**
 * Session Class Doc Comment
 *
 * @category Class
 * @package  include.class
 * @author bryyce
 * @license  http://www.gnu.org/copyleft/gpl.html GNU General Public License
 * @link     http://www.bryyce.fr/
 */
class Session
{
    private static $_membre;
    public static function CreateSession()
    {
		session_start();
		if(!isset($_SESSION['id'])) {
			session_destroy();
			return false;
		}
                Session::$_membre=new Membre($_SESSION['id']);
		return true;
    }
    public static function CreateSessionOrDie($page){
        if(!self::CreateSession()){
            header('Location: /'.$page);
            exit;
        }
    }
    public static function CreateAdminSessionOrDie($page){
        if(!self::CreateSession() || !Session::isAdmin()){
            header('Location: /'.$page);
            exit;
        }
    }
    public static function isAdmin(){
        return Session::$_membre->isAdmin();
    }
    public static function getMembre(){
        return Session::$_membre;
    }
}
?>