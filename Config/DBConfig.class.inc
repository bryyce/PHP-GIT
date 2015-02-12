<?php

/**
 * Config.class file
 * @filesource ./ORM/Config.class
 * @package        ORM
 */
namespace Config;

/**
 * Config Class Doc Comment
 *
 * @category    Class
 * @package        ORM
 * @author bryyce bric.robin@gmail.com
 * @license        http://www.gnu.org/copyleft/gpl.html GNU General Public License
 * @link        http://www.bryyce.fr/
 */
class DBConfig {

    /**
     * toutes les tables de base de données en fonction du nom du modele
     * @static
     * @access private
     * @var array
     */
    private static $tables = array(
         'user' => 'users',
         'profile' => 'profiles',
         'point' => 'points',
    );

    /**
     * tous les champs de base de données en fonction du nom de la table
     * @static
     * @access private
     * @var array
     */
    private static $fields = array(
        'users' =>
        array('id' => 'id',
              'login' => "login",
             // 'mail' => 'e-mail',
              'password' => 'mot de passe',
              //'salt' => 'salt',
             // 'created_at' => 'crée le',
             // 'updated_at' => 'mis à jour le'
        ),
        'points' => array(
            'id' => 'id',
            'latitude' => 'latitude',
            'longitude' => 'longitude',
            'datetime' => 'date',
            'user_id' => 'user_id',
        ),
         'profiles' =>
         array('id' => 'id',
              'name' => 'nom',
              'firstname' => 'prenom',
              'address' => 'adresse',
              'last_login' => 'dernière connexion')
    );

    /**
     *    Donne le nom des champs de la base de données
     * @param string $table le nomde la table
     * @return array les champs de la base de données
     */
    public static function getFields($table) {
        return static::$fields[$table];
    }
/**
     *    Donne le nom de la table de la base de données
     * @param string $class le nom de la class
     * @return string  le nom de la table de la base de données
     */
    public static function getTable($class) {
        return static::$tables[$class];
    }

    public static function getKeyTable($table) {
        return array_keys(static::$tables, $table)[0];
    }

}

?>
