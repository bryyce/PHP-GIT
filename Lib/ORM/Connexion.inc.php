<?php
/**
 * Connection.inc.php file
 * @filesource ./ORM/Connexion.inc.php
 * @package        ORM
 */
namespace Lib\ORM;


/**
 *  Encapsulation de PDOStatement  Connexion Class Doc Comment
 *
 * @category    Class
 * @author        bryyce    bric.robin@gmail.com
 * @license        http://www.gnu.org/copyleft/gpl.html GNU General Public License
 * @link        http://www.bryyce.fr/
 */

class myPDOStatement {

    /**
     *     Instance PDO
     * @var object
     */
    private $_pdoStatement;

    /**
     * Constructeur d'intance de PDO
     * @param object $_pdoStatement  L'objet PDOStatement
     * @final
     */
    final public function __construct($_pdoStatement) {
        myPDO::msg("Construction PDOStatement");
        $this->_pdoStatement = $_pdoStatement;
    }


    /**
     * Destructeur d'intance de PDO
     * @final
     */
    final public function __destruct() {
        myPDO::msg("Destruction PDOStatement");
        $this->_pdoStatement = null;
    }


    /**
     * Surcharge de toutes les méthodes inexistantes de myPDOStatement pour pouvoir appeler celles de PDOStatement
     *
     * @param string $methodName Nom de la méthode
     * @param array $methodArguments Tableau des paramètres
     * @return mixed
     */
    final public function __call($methodName , $methodArguments) {
        // La méthode appelée fait-elle partie de la classe PDOStatement
        if (!method_exists($this->_pdoStatement, $methodName))
            throw new \Exception("PDOStatement::$methodName n'existe pas");
        // Message de debogage
        myPDO::msg("PDOStatement::" . $methodName . " (" . var_export($methodArguments, true) . ")");
        // Appel de la méthode avec l'objet PDOStatement
        return call_user_func_array(array($this->_pdoStatement, $methodName), $methodArguments);
    }

}

/**
 *  Classe permettant de faire une connexion unique et automatique à la BD
 */
class myPDO {

    /**
     * Instance du Singleton
     * @var object
     */
    private static $_mypdo = null;

    /**
     * Est en mode de de debogage
     * @var boolean
     */
    private static $_debug = false;

    /**
     *    Source de données
     * @var string
     */
    private static $_dsn = null;

    /**
     * Identifiant de la base de données
     * @var string
     */
    private static $_user = null;

    /**
     * mot de passe de la base de données
     * @var string
     */
    private static $_pass = null;

    /**
     *    Lien de connexion à la base
     * @var object
     */
    private $_pdo = null;

    /**
     * créer une instance PDOStatement si elle existe pas
     *
     *    @access private
     * @throws Exception
     * @final
     */
    final private function __construct() {
        self::msg("Demande construction PDO...");
        if (is_null(self::$_dsn)
                  || is_null(self::$_user)
                  || is_null(self::$_pass))
            throw new Exception("Construction impossible : les paramÃƒÂªtres de connexion sont absents");
        // Etablir la connexion
        $this->_pdo = new \PDO(self::$_dsn, self::$_user, self::$_pass);
        // Mise en place du mode "Exception" pour les erreurs PDO
        $this->_pdo->setAttribute(\PDO::ATTR_ERRMODE, \PDO::ERRMODE_EXCEPTION);
        // Cas particulier de MySQL
        switch ($this->_pdo->getAttribute(\PDO::ATTR_DRIVER_NAME)) {
            case 'mysql' :
                // Pour que cela fonctionne sur MySQL... C'est pas beau l'abstraction ?
                $this->_pdo->setAttribute(\PDO::MYSQL_ATTR_USE_BUFFERED_QUERY, true);
                // Passage du client MySQL en utf-8
                $this->_pdo->query("SET CHARACTER SET 'utf8'");
                break;
            /*
              case 'oci' :
              // Passage du client MySQL en utf-8
              $this->_pdo->query("ALTER SESSION SET NLS_SORT = FRENCH");
              break ;
             */
        }
        self::msg("Construction PDO terminée");
    }

    /**
     * Destructeur
     * @final
     */
    final public function __destruct() {
        self::msg("Demande de destruction PDO...");
        // S'il y a une connexion établie...
        if (!is_null($this->_pdo)) {
            // ... il faut se deconnecter
            self::msg("Demande de déconnexion...");
            $this->_pdo = null;
            self::$_mypdo = null;
            self::msg("Deconnexion effectuée");
        }
        self::msg("Destruction PDO terminée");
    }

    /**
     * Récuperer le singleton
     * @return \ORM\MyPDO l'instance courante de MyPDO
     * @final
     */
    final public static function get() {
        self::msg("Recherche de l'instance...");
        // Une instance est-elle disponible ?
        if (!isset(self::$_mypdo))
            self::$_mypdo = new myPDO();
        self::msg("Instance trouvée");
        return self::$_mypdo;
    }

    /**
     *    Fonction qui fixe les paramètres de connexion
     *
     * @param string $_dsn le nom d'acces du serveur PDO
     * @param string $_user l'utilisateur d'acces à la base de données
     * @param string $_pass le mot de passe d'acces à la base de données
     * @final
     */
    final public static function parametres($_dsn, $_user, $_pass) {
        self::msg("Demande de positionnement des paramètres de connexion...");
        self::$_dsn = $_dsn;
        self::$_user = $_user;
        self::$_pass = $_pass;
        self::msg("Positionnement des paramètres de connexion terminé");
    }

    /**
     *    Interdit le clonage du singleton
     * @throws Exception
     * @final
     */
    final public function __clone() {
        throw new \Exception("Clonage de " . __CLASS__ . " interdit !");
    }

    /**
     * Affichage de messages de contrôle
     * @param string $m Le message à afficher
     * @final
     */
    final public static function msg($m) {
        if (self::$_debug)
            echo "\n<!-- $m -->\n";
    }

    /**
     * Mise en marche du debogage
     * @final
     */
    final public static function debug_on() {
        self::$_debug = true;
    }

    /**
     * Arret du debogage
     * @final
     */
    final public static function debug_off() {
        self::$_debug = false;
    }

    /**
     * Surcharge de toutes les méthodes inexistantes de myPDO pour pouvoir appeler celles de PDO
     *
     * @param string $methodName Nom de la méthode
     * @param type $methodArguments Tableau des paramètres
     * @return \ORM\myPDOStatement le gestionnaire de base de données
     * @throws Exception
     * @final
     */
    final public function __call($methodName , $methodArguments) {
        // La méthode appelée fait-elle partie de la classe PDO
        if (!method_exists($this->_pdo, $methodName))
            throw new \Exception("\PDO::$methodName n'existe pas");
        // Message de debogage
        self::msg("\PDO::$methodName (" . implode($methodArguments, ", ") . ")");
        // Appel de la méthode avec l'objet PDO
        $result = call_user_func_array(array($this->_pdo, $methodName), $methodArguments);
        // Selon le nom de la méthode
        switch ($methodName) {
            // Cas 'prepare' ou 'query' => mise en place du fetchMode par tableau associatif
            case "prepare" :
            case "query" :
                $result->setFetchMode(\PDO::FETCH_NAMED);
                // Retourne un objet myPDOStatement
                return new myPDOStatement($result);
            // Dans tous les autres cas
            default :
                // Retourne le resultat
                return $result;
        }
    }

}
