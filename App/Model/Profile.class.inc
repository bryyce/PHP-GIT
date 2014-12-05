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

	//***********************************//
	//  Fin Fonctions membres protégées  //
	//***********************************//
	/* public function validation() {
	  $msg = "<ul>";
	  if (count($this->dispo('pseudo')) == 1)
	  $msg .= 'Ce pseudo existe dÃƒÂ©jÃƒÂ , veuillez en choisir un autre';
	  if (count($this->dispo('mail')) == 1)
	  $msg .= 'Cette adresse e-mail existe dÃƒÂ©jÃƒÂ , veuillez en choisir une autre';
	  if (count($this->dispo('ip')) == 1)
	  $msg .= 'Un compte par ordinateur est autorisÃƒÂ©';
	  if (strlen($this->pseudo) > 15 OR strlen($this->pseudo) < 3)
	  $msg .= 'Vous devez choisir un pseudo compris entre 3 et 15 caractÃƒÂ¨res';
	  if (preg_match("#[^a-zA-Z0-9_-]#", $this->pseudo))
	  $msg .= 'La syntaxe de votre pseudo est incorrecte';
	  if (strlen($this->code) < 6 OR strlen($this->code) > 21)
	  $msg .= 'Vous devez choisir un code secret compris entre 6 et 21 caractÃƒÂ¨res';
	  if (!preg_match("#^[a-z0-9._-]+@[a-z0-9._-]{2,}\.[a-z]{2,4}$#", $this->mail))
	  $msg .= 'La syntaxe de votre adresse e-mail est incorrecte';
	  if ($msg != "<ul>")
	  throw new Exception($msg . "</ul>");
	  } */
}

?>