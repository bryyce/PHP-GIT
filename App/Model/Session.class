<?php

/**
 * Session.class file
 * @filesource ./Model/Session.class
 * @package		Model
 */

namespace App\Model;

/**
 * Session Class Doc Comment
 * @category  Class
 * @package    Model
 * @author    bryyce
 * @license    http://www.gnu.org/copyleft/gpl.html GNU General Public License
 * @link    http://www.bryyce.fr/
 */
class Session {

	/**
	 * l'utilisateur courant de la session
	 *
	 * 	@access private
	 * @var User
	 */
	private static $_user;

	/**
	 * Crée unu session en fonction de l'id en session
	 * @return boolean
	 */
	public static function CreateSession() {
		session_start();
		if (!isset($_SESSION['id'])) {
			session_destroy();
			return false;
		}
		Session::$_user = new User($_SESSION['id']);
		return true;
	}

	/**
	 * 	Crée unu session ou redirige vers une page passée en parametre si la session n'a pas pu etre créée
	 * @param string $page la page de redirection
	 */
	public static function CreateSessionOrDie($page) {
		if (!self::CreateSession()) {
			header('Location: /' . $page);
			exit;
		}
	}

	/**
	 * 	Crée unu session ou redirige vers une page passée en parametre si la session n'a pas pu etre créée ou n'est pas admin
	 * @param string $page la page de redirection
	 */
	public static function CreateAdminSessionOrDie($page) {
		if (!self::CreateSession() || !Session::isAdmin()) {
			header('Location: /' . $page);
			exit;
		}
	}

	/**
	 * Verifie si l'utilisateur courant est administrateur
	 * @return boolean
	 */
	public static function isAdmin() {
		return Session::$_user->isAdmin();
	}

	/**
	 * Renvoi l'utilisateur courant
	 * @return User
	 */
	public static function currentUser() {
		return Session::$_user;
	}
}
?>