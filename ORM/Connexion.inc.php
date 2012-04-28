<?php
/**
 * Connexion Class Doc Comment
 *
 * @category	Class
 * @package		ORM
 * @author		bryyce
 * @license		http://www.gnu.org/copyleft/gpl.html GNU General Public License
 * @link		http://www.bryyce.fr/
 */
namespace ORM;
error_reporting(E_ALL);
/// Mise en place d'une capture des exceptions non attrappÃƒÂ©es
function exceptionHandler($exception /** L'Exception non attrappÃƒÂ©e */) {
    echo "<pre>\n" ;
    echo $exception->getMessage()."\n" ;
    echo $exception->getTraceAsString()."\n" ;
    echo "</pre>\n" ;
}
set_exception_handler('ORM\exceptionHandler');

/// Outil de dump de variable
function dump($var /** La variable ÃƒÂ© afficher */)
{
    echo "<pre>\n" ;
    var_dump($var);
    echo "</pre>\n" ;
}

/**
 *  Encapsulation de PDOStatement
 */
class myPDOStatement
{
    /// L'objet PDOStatement
    private $_pdoStatement ;

    /// Constructeur
    final public function __construct($_pdoStatement /** L'objet PDOStatement */) {
        myPDO::msg("Construction PDOStatement");
        $this->_pdoStatement = $_pdoStatement ;
    }

    /// Destructeur
    final public function __destruct() {
        myPDO::msg("Destruction PDOStatement");
        $this->_pdoStatement = null ;
    }

    /// Surcharge de toutes les mÃƒÂ©thodes inexistantes de myPDOStatement pour pouvoir appeler celles de PDOStatement
    final public function __call($methodName      /** Nom de la mÃƒÂ©thode */,
            $methodArguments /** Tableau des paramÃƒÂ©tres */
            ) {
        // La mÃƒÂ©thode appelÃƒÂ©e fait-elle partie de la classe PDOStatement
        if (!method_exists($this->_pdoStatement, $methodName))
            throw new Exception("PDOStatement::$methodName n'existe pas");
        // Message de debogage
        myPDO::msg("PDOStatement::".$methodName." (".var_export($methodArguments, true).")");
        // Appel de la mÃƒÂ©thode avec l'objet PDOStatement
        return call_user_func_array(array($this->_pdoStatement, $methodName), $methodArguments);
    }

}

/**
 *  Classe permettant de faire une connexion unique et automatique ÃƒÂ© la BD
 */
class myPDO
{
    /// Singleton
    private static $_mypdo = null ;
    /// Message de debogage
    private static $_debug = false;
    /// Data Source Name
    private static $_dsn   = null ;
    /// Utilisateur
    private static $_user  = null ;
    /// Mot de passe
    private static $_pass  = null ;
    /// Connexion ÃƒÂ© la base
    private        $_pdo   = null ;

    /// Constructeur privÃƒÂ©
    final private function __construct() {
        self::msg("Demande construction PDO...");
        if (       is_null(self::$_dsn)
                || is_null(self::$_user)
                || is_null(self::$_pass))
            throw new Exception("Construction impossible : les paramÃƒÂªtres de connexion sont absents");
        // Etablir la connexion
        $this->_pdo = new \PDO(self::$_dsn, self::$_user, self::$_pass);
        // Mise en place du mode "Exception" pour les erreurs PDO
        $this->_pdo->setAttribute(\PDO::ATTR_ERRMODE, \PDO::ERRMODE_EXCEPTION);
        // Cas particulier de MySQL
        switch ($this->_pdo->getAttribute(\PDO::ATTR_DRIVER_NAME)){
            case 'mysql' :
                // Pour que cela fonctionne sur MySQL... C'est pas beau l'abstraction ?
                $this->_pdo->setAttribute(\PDO::MYSQL_ATTR_USE_BUFFERED_QUERY, true);
                // Passage du client MySQL en utf-8
                $this->_pdo->query("SET CHARACTER SET 'utf8'");
            break ;
            /*
            case 'oci' :
                // Passage du client MySQL en utf-8
                $this->_pdo->query("ALTER SESSION SET NLS_SORT = FRENCH");
            break ;
            */
        }
        self::msg("Construction PDO terminÃƒÂ©e");
    }

    /// Destructeur
    final public function __destruct() {
        self::msg("Demande de destruction PDO...");
        // S'il y a une connexion ÃƒÂ©tablie...
        if (!is_null($this->_pdo))
        {
            // ... il faut se deconnecter
            self::msg("Demande de dÃƒÂ©connexion...");
            $this->_pdo   = null ;
            self::$_mypdo = null ;
            self::msg("Deconnexion effectuÃƒÂ©e");
        }
        self::msg("Destruction PDO terminÃƒÂ©e");
    }

    /// RÃƒÂ©cuperer le singleton
    final public static function get() {
        self::msg("Recherche de l'instance...");
        // Une instance est-elle disponible ?
        if (!isset(self::$_mypdo))
            self::$_mypdo = new myPDO();
        self::msg("Instance trouvÃƒÂ©e");
        return self::$_mypdo ;
    }

    /// Fixer les paramÃƒÂ©tres de connexion
    public static function parametres($_dsn, $_user, $_pass) {
        // self::msg("Demande de positionnement des paramÃƒÂ©tres de connexion...");
        self::$_dsn  = $_dsn ;
        self::$_user = $_user ;
        self::$_pass = $_pass ;
        // self::msg("Positionnement des paramÃƒÂ©tres de connexion terminÃƒÂ©");
    }

    /// Interdit le clonage du singleton
    final public function __clone() {
        throw new Exception("Clonage de ".__CLASS__." interdit !");
    }

    /// Affichage de messages de contrÃƒÂ©le
    final public static function msg($m /** Le message */) {
        if (self::$_debug)
            echo "\n<!-- $m -->\n" ;
    }

    /// Mise en marche du debogage
    final public static function debug_on() {
        self::$_debug = true ;
    }

    /// Arret du debogage
    final public static function debug_off() {
        self::$_debug = false ;
    }

    /// Surcharge de toutes les mÃƒÂ©thodes inexistantes de myPDO pour pouvoir appeler celles de PDO
    final public function __call($methodName      /** Nom de la mÃƒÂ©thode */,
            $methodArguments /** Tableau des paramÃƒÂ©tres */
            ) {
        // La mÃƒÂ©thode appelÃƒÂ©e fait-elle partie de la classe PDO
        if (!method_exists($this->_pdo, $methodName))
            throw new Exception("\PDO::$methodName n'existe pas");
        // Message de debogage
        self::msg("\PDO::$methodName (".implode($methodArguments, ", ").")");
        // Appel de la mÃƒÂ©thode avec l'objet PDO
        $result = call_user_func_array(array($this->_pdo, $methodName), $methodArguments);
        // Selon le nom de la mÃƒÂ©thode
        switch ($methodName)
        {
            // Cas 'prepare' ou 'query' => mise en place du fetchMode par tableau associatif
            case "prepare" :
            case "query" :
                $result->setFetchMode(\PDO::FETCH_NAMED);
                // Retourne un objet myPDOStatement
                return new myPDOStatement($result);
            // Dans tous les autres cas
            default :
                // Retourne le resultat
                return $result ;
        }
    }
}
myPDO::parametres('mysql:localhost;port=1131;dbname=orm', 'orm', 'orm');
?>
