<?php
namespace App\Model;

use Lib\ORM;

/**
 * User Class Doc Comment
 *
 * @category       Class
 * @package        Model
 * @author         bryyce <bric.robin@gmail.com>
 * @license        http://www.gnu.org/copyleft/gpl.html GNU General Public License
 * @property-read  int        $id          identifiant BDD
 * @property       string    $login        identifiant de l'utilisateur
 * @property       string    $mail         adresse e-mail de l'utilisateur
 * @property       string    $password     mot de passe de l'utilisateur
 * @property-read  string    $salt         sel d'encryption
 * @property-read  date      $created_at   date de création de l'enregistrement
 * @property-read  date      $updated_at   date de mise à jour
 */
class User extends ORM\MyEnregistrement
{

    /**
     * Stockage des relations avec les autres objets
     *    @access protected
     * @static array
     */
    protected static $has_and_belongs_to_many = array('profiles' => array());

    /**
     * Ensemble des validations a effectuer avant de sauvegarder
     *    @access protected
     * @static array
     */
    protected static $validate = array(
         'unique' => array('login'),
         'length' => array('login' => array('lt' => 5))
    );
}
?>
