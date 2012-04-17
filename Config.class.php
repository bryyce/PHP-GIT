<?php
/**
 * Config Class Doc Comment
 *
 * @category Class
 * @package  include.class
 * @author bryyce
 * @license  http://www.gnu.org/copyleft/gpl.html GNU General Public License
 * @link     http://www.bryyce.fr/
 */
class Config {
    private static $fields = array(
            'membres' =>
                array('id' => 'id', 'pseudo' => "pseudo", 'code' => "code", 'mail' => 'e-mail', 'verif' => 'verif',
                    'nom' => 'nom', 'prenom' => 'prénom', 'adresse' => 'adresse', 'cp' => 'code postal', 'ville' => 'ville', 'pays' => 'pays',
                    'paypal' => 'paypal', 'inscription' => 'inscription', 'connexion' => 'connexion', 'ip' => 'ip', 'banni' => 'banni',
                    'niveau' => 'niveau', 'id_parrain' => 'id_parrain', 'id_situation_joueur' => 'id_situation_joueur', 'id_status' => 'id_status')
        );
    public static function getFields($table) {
        return static::$fields[$table];
    }
}
?>
