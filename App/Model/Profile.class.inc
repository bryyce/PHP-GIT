<?php

/**
 * Profile.class file
 * @filesource ./Model/Profile.class
 * @package		Model
 */

namespace App\Model;

use Lib\ORM;

/**
 * Status Class Doc Comment
 *
 * @category	Class
 * @package		Model
 * @author		bryyce bric.robin@gmail.com
 * @license		http://www.gnu.org/copyleft/gpl.html GNU General Public License
 * @link			http://www.bryyce.fr/
 *
 * @property-read		int		$id				identifiant BDD
 * @property			string	$name				nom de l'utilisateur
 * @property			string	$firstname		prénom de l'utilisateur
 * @property			string	$address			adresse de l'utilisateur
 * @property			date		$last_login		date de dernière connexion de l'utilisateur
 *
 */
class Profile extends ORM\MyEnregistrement {

	/**
	 * Stockage des relations avec les autres objets
	 *	@access protected
	 * @static array
	 */
	protected static $has_and_belongs_to_many = array('users' => array('master' => false));

}