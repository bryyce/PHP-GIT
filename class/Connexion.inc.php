<?php
/**
 * Connexion Class Doc Comment
 *
 * @category Class
 * @package  Include.class
 * @author bryyce
 * @license  http://www.gnu.org/copyleft/gpl.html GNU General Public License
 * @link     http://www.bryyce.fr/
 */
error_reporting(E_ALL);
/// Mise en place d'une capture des exceptions non attrappées
function exceptionHandler($exception /** L'Exception non attrappée */) {
    echo "<pre>\n" ;
    echo $exception->getMessage()."\n" ;
    echo $exception->getTraceAsString()."\n" ;
    echo "</pre>\n" ;
}
set_exception_handler('exceptionHandler');

/// Outil de dump de variable
function dump($var /** La variable é afficher */)
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

    /// Surcharge de toutes les méthodes inexistantes de myPDOStatement pour pouvoir appeler celles de PDOStatement
    final public function __call($methodName      /** Nom de la méthode */,
            $methodArguments /** Tableau des paramétres */
            ) {
        // La méthode appelée fait-elle partie de la classe PDOStatement
        if (!method_exists($this->_pdoStatement, $methodName))
            throw new Exception("PDOStatement::$methodName n'existe pas");
        // Message de debogage
        myPDO::msg("PDOStatement::".$methodName." (".var_export($methodArguments, true).")");
        // Appel de la méthode avec l'objet PDOStatement
        return call_user_func_array(array($this->_pdoStatement, $methodName), $methodArguments);
    }

}

/**
 *  Classe permettant de faire une connexion unique et automatique é la BD
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
    /// Connexion é la base
    private        $_pdo   = null ;

    /// Constructeur privé
    final private function __construct() {
        self::msg("Demande construction PDO...");
        if (       is_null(self::$_dsn)
                || is_null(self::$_user)
                || is_null(self::$_pass))
            throw new Exception("Construction impossible : les paramêtres de connexion sont absents");
        // Etablir la connexion
        $this->_pdo = new PDO(self::$_dsn, self::$_user, self::$_pass);
        // Mise en place du mode "Exception" pour les erreurs PDO
        $this->_pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
        // Cas particulier de MySQL
        switch ($this->_pdo->getAttribute(PDO::ATTR_DRIVER_NAME)){
            case 'mysql' :
                // Pour que cela fonctionne sur MySQL... C'est pas beau l'abstraction ?
                $this->_pdo->setAttribute(PDO::MYSQL_ATTR_USE_BUFFERED_QUERY, true);
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
        self::msg("Construction PDO terminée");
    }

    /// Destructeur
    final public function __destruct() {
        self::msg("Demande de destruction PDO...");
        // S'il y a une connexion établie...
        if (!is_null($this->_pdo))
        {
            // ... il faut se deconnecter
            self::msg("Demande de déconnexion...");
            $this->_pdo   = null ;
            self::$_mypdo = null ;
            self::msg("Deconnexion effectuée");
        }
        self::msg("Destruction PDO terminée");
    }

    /// Récuperer le singleton
    final public static function get() {
        self::msg("Recherche de l'instance...");
        // Une instance est-elle disponible ?
        if (!isset(self::$_mypdo))
            self::$_mypdo = new myPDO();
        self::msg("Instance trouvée");
        return self::$_mypdo ;
    }

    /// Fixer les paramétres de connexion
    public static function parametres($_dsn, $_user, $_pass) {
        // self::msg("Demande de positionnement des paramétres de connexion...");
        self::$_dsn  = $_dsn ;
        self::$_user = $_user ;
        self::$_pass = $_pass ;
        // self::msg("Positionnement des paramétres de connexion terminé");
    }

    /// Interdit le clonage du singleton
    final public function __clone() {
        throw new Exception("Clonage de ".__CLASS__." interdit !");
    }

    /// Affichage de messages de contréle
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

    /// Surcharge de toutes les méthodes inexistantes de myPDO pour pouvoir appeler celles de PDO
    final public function __call($methodName      /** Nom de la méthode */,
            $methodArguments /** Tableau des paramétres */
            ) {
        // La méthode appelée fait-elle partie de la classe PDO
        if (!method_exists($this->_pdo, $methodName))
            throw new Exception("PDO::$methodName n'existe pas");
        // Message de debogage
        self::msg("PDO::$methodName (".implode($methodArguments, ", ").")");
        // Appel de la méthode avec l'objet PDO
        $result = call_user_func_array(array($this->_pdo, $methodName), $methodArguments);
        // Selon le nom de la méthode
        switch ($methodName)
        {
            // Cas 'prepare' ou 'query' => mise en place du fetchMode par tableau associatif
            case "prepare" :
            case "query" :
                $result->setFetchMode(PDO::FETCH_NAMED);
                // Retourne un objet myPDOStatement
                return new myPDOStatement($result);
            // Dans tous les autres cas
            default :
                // Retourne le resultat
                return $result ;
        }
    }
}
?>
