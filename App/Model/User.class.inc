<?php

/**
 * User.class file
 * @filesource ./Model/User.class
 * @package		Model
 */

namespace App\Model;

use Lib\ORM;

/**
 * User Class Doc Comment
 *
 * @category	Class
 * @package		Model
 * @author		bryyce bric.robin@gmail.com
 * @license		http://www.gnu.org/copyleft/gpl.html GNU General Public License
 * @link			http://www.bryyce.fr/
 * @property-read		int		$id				identifiant BDD
 * @property			string	$login			identifiant de l'utilisateur
 * @property			string	$mail			adresse e-mail de l'utilisateur
 * @property			string	$password		mot de passe de l'utilisateur
 * @property-read		string	$salt			sel d'encryption
 * @property-read		date		$created_at	date de création de l'enregistrement
 * @property-read		date		$updated_at	date de mise à jour
 */
class User extends ORM\MyEnregistrement {

	/**
	 * Stockage des relations avec les autres objets
	 *	@access protected
	 * @static array
	 */
	protected static $has_and_belongs_to_many = array('profiles' => array());

	/**
	 * Ensemble des validations a effectuer avant de sauvegarder
	 *	@access protected
	 * @static array
	 */
	protected static $validate = array(
		 'unique' => array('login'),
		 'length' => array('login' => array('lt' => 5))
	);


	//***********************************//
	//  Fin Fonctions membres protégées  //
	//***********************************//
/*	public function validation() {
		$msg = "<ul>";
		if (count($this->dispo('pseudo')) == 1)
			$msg .= 'Ce pseudo existe déjà , veuillez en choisir un autre';
		if (count($this->dispo('mail')) == 1)
			$msg .= 'Cette adresse e-mail existe déjà , veuillez en choisir une autre';
		if (count($this->dispo('ip')) == 1)
			$msg .= 'Un compte par ordinateur est autorisé';
		if (strlen($this->pseudo) > 15 OR strlen($this->pseudo) < 3)
			$msg .= 'Vous devez choisir un pseudo compris entre 3 et 15 caractères';
		if (preg_match("#[^a-zA-Z0-9_-]#", $this->pseudo))
			$msg .= 'La syntaxe de votre pseudo est incorrecte';
		if (strlen($this->code) < 6 OR strlen($this->code) > 21)
			$msg .= 'Vous devez choisir un code secret compris entre 6 et 21 caractères';
		if (!preg_match("#^[a-z0-9._-]+@[a-z0-9._-]{2,}\.[a-z]{2,4}$#", $this->mail))
			$msg .= 'La syntaxe de votre adresse e-mail est incorrecte';
		if ($msg != "<ul>")
			throw new Exception($msg . "</ul>");
	}*/

}

?>
