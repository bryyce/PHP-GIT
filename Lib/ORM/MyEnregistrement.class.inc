<?php

/**
 * MyEnregistrement.class file
 * @filesource ./ORM/MyEnregistrement.class
 * @package     ORM
 */

namespace Lib\ORM;
use Config;
/**
 *
 * Description of MyEnregistrement
 *
 * @todo        gerer le set pour hbtm
 * @todo    unset poour les relations
 * @abstract
 * @category    Class
 * @package     ORM
 * @author      bryyce
 * @license     http://www.gnu.org/copyleft/gpl.html GNU General Public License
 * @link        http://www.bryyce.fr/
 */
abstract class MyEnregistrement extends Enregistrement {
    /**
     * identifiant de table en BDD
     */

    const CLE_PRI = "id";

    /**
     * Tableau qui gere les relations 1-1
     * @access protected
     * @static
     * @var array
     */
    protected static $has_one = array();
    /**
     * Tableau qui gere les relations 1-n
     * @access protected
     * @static
     * @var array
     */
    protected static $has_many = array();
    /**
     * Tableau qui gere les relations n-n
     * @access protected
     * @static
     * @var array
     */
    protected static $has_and_belongs_to_many = array();
    /**
     * Tableau qui gere les relations 1-1 ou n-1
     * @access protected
     * @static
     * @var array
     */
    protected static $belongs_to = array();
    /**
     * Tableau qui stocke les éléments charger depuis la base de données
     * @access protected
     * @static
     * @var array
     */
    protected static $relations_elem = array();

    /**
     * Tableau qui stocke les erreurs de validation
     *    @access public
     * @var array
     */
    public $errors = array();

    /**
     * Tableau qui stocke les validations à effectuer avant la sauvegarde en base de données
     * @access protected
     * @static
     * @var array
     */
    protected static $validate = array();

    protected static $order = NULL;
    protected static $offset = 0;
    protected static $limit = NULL;

    /**
     * Constructeur d'enregistrement
     *
     * @param <int> $_id id de l'objet a chercher dans la BDD si null un objet vide est créé
     */
    public function __construct($_params = []) {
        $this->initAttributs(self::etiquettes());
        $this->chargement($_params);
        /*if ($_id != null) {
            $this->lecture($_id);
        }*/

    }

    /**
     * Precharge tous les modèles de relations du modele de données
     *    @static
     */
    public static function loadRelations() {
        foreach (static::$has_one as $k => $v) {
            if (!array_key_exists('primary_key', static::$has_one[$k])) {
                static::$has_one[$k]['primary_key'] = $k . '_id';
            }
            if (!array_key_exists('class', static::$has_one[$k])) {
                static::$has_one[$k]['class'] = 'App\\Model\\' . ucfirst($k);
            }
        }
        foreach (static::$has_many as $k => $v) {
            if (!array_key_exists('primary_key', static::$has_many[$k])) {
                static::$has_many[$k]['primary_key'] = static::revertTable(static::table()) . '_id';
              //0  static::$has_many[$k]['primary_key'] = $k . '_id';
            }
            if (!array_key_exists('class', static::$has_many[$k])) {
                //static::$has_many[$k]['class'] = 'App\\Model\\' . ucfirst($k);
                static::$has_many[$k]['class'] = 'App\\Model\\' . ucfirst(static::revertTable($k));
            }
        }
        foreach (static::$has_and_belongs_to_many as $k => $v) {
            $class = 'App\\Model\\' . ucfirst(substr($k, 0, strlen($k) - 1));
            if (!array_key_exists('primary_key', static::$has_and_belongs_to_many[$k])) {
                static::$has_and_belongs_to_many[$k]['primary_key'] = substr($k, 0, strlen($k) - 1) . '_id';
            }
            if (!array_key_exists('class', static::$has_and_belongs_to_many[$k])) {
                static::$has_and_belongs_to_many[$k]['class'] = $class;
            }
            if (array_key_exists('master', static::$has_and_belongs_to_many[$k]) && static::$has_and_belongs_to_many[$k]['master'] == false) {
                static::$has_and_belongs_to_many[$k]['join_table'] = $class::table() . '_' . static::table();
            } else {
                static::$has_and_belongs_to_many[$k]['join_table'] = static::table() . '_' . $class::table();
            }
        }
        foreach (static::$belongs_to as $k => $v) {
            if (!array_key_exists('primary_key', static::$belongs_to[$k])) {
                static::$belongs_to[$k]['primary_key'] = static::table() . '_id';
            }
            if (!array_key_exists('class', static::$belongs_to[$k])) {
                static::$belongs_to[$k]['class'] = 'App\\Model\\' . ucfirst($k);
            }
        }
    }

    /**
     * Renvoie les etiquettes de la tables
     *
     *    @uses Config::getFields()
     * @return string[]
     */
    public function etiquettes() {
        return Config\DBConfig::getFields(static::table());
    }

    /**
     * Donne la cle primaire de la table.
     * @return string
     */
    protected static function cle_pri() {
        return static::CLE_PRI;
    }

    /**
     *    retourne la valeur de la clé primaire de la table
     * @return int
     */
    protected function get_primary_key_value() {
        $id = static::cle_pri();
        return $this->$id;
    }

    /**
     *    Retourne le nom de la classe
     * @return string
     */
    protected static function getClassName() {
        return strtolower(preg_replace('#([a-zA-Z]+\\\\)+([a-zA-Z]+)#i', '$2', get_called_class()));
    }

    /**
     *    retourne le nom de la table mysql
     * @uses Config::getTable()
     * @return string
     */
    protected static function table() {
        return Config\DBConfig::getTable(static::getClassName());
    }

    protected static function revertTable($table) {
        return Config\DBConfig::getKeyTable($table);
    }

    /**
     *    Retourne une collection d'enregistrements de la table sql de l'instance en fonction d'une condition sql
     * @param string $cond la condition sql
     * @return \Collection
     */
    protected static function allBy($cond = "") {
        $records = new \Collection();
        // Execution de la requete avec le parametre et lecture du resultat
        foreach (QueryManager::get()->select(static::table())->where($cond)->order(static::$order)->execute() as $record) {
            if (!is_null($record)) {
                $records->push(new static($record));
            }
        }
        return $records;
    }

    /**
     *    Retourne tous les enregistrements de la table sql de l'instance
     * @return \Collection
     */
    public static function all() {
        return self::allBy();
    }


    public static function where($_params = []) {
        $_params += ['params' => array(), 'limit' => static::$limit, 'offset' => static::$offset, 'order' => static::$order];
        $params = array();
        foreach ($_params['params'] as $key => $value) {
            if (is_array($value)) {
                $value = join(" ",$value);
            } else {
                $value  = '$value';
            }
            $params[] = "$key $value";
        }

        $collection = QueryManager::get()->select(static::table())->where($params)->order($_params['order'])->limit($_params['limit'], $_params['offset'])->execute();
        $records = new \Collection();
        foreach ($collection as $record) {
            if (!is_null($record)) {
                $records->push(new static($record));
            }
        }
        return $records;
    }

    /**
     *    Retourne le premier enregistrement de la table sql de l'instance
     * @param array $_params ensemble de condition
     * @return \Collection
     */
    public static function first($_params = array()) {
        $params = array();
        foreach ($_params as $key => $value) {
            $params[] = "$key = '$value'";
        }
        $collection = QueryManager::get()->select(static::table())->where($params)->order('id')->limit(1)->execute();
        return new static($collection->count() ? $collection->first() : array());
    }

    /**
     *    Retourne le dernier enregistrement de la table sql de l'instance
     * @param array $_params ensemble de condition
     * @return \Collection
     */
    public static function last($_params = array()) {
        $params = array();
        foreach ($_params as $key => $value) {
            $params[] = "$key = '$value'";
        }
        $collection = QueryManager::get()->select(static::table())->where($params)->order('id DESC')->limit(1)->execute();
        return new static($collection->count() ? $collection->first() : array());
    }

    /**
     * Compte tous les éléments présent en base de données
     *
     * @param array $_params tous les paramètres pour faire les décomptes
     * @param string $_others autres paramètres directement en SQL pour faire les décomptes
     * @return int  Retourne le nombre d'éléments
     */
    public static function count($_params = array(), $_others = '') {
        $params = array();
        foreach ($_params as $key => $value) {
            $params[] = "$key = '$value'";
        }
        $conditions = implode(' AND ', $params) . ' ' . $_others;
        $res = QueryManager::get()->select(static::table())->where($conditions)->count()->execute();
        // Execution de la requete avec le parametre et lecture du resultat
        if ($res->count() > 0) {
            $result = $res->first();
            return $result['count'];
        }
        return 0;
    }
    /**
     * Fonction qui permet de recuperer des enregistrements fonction des parametres par le biais de différentes méthodes
     * exemple :
     *        - findBy
     *        - firstBy
     *        - lastBy
     *        - countBy
     * @param string le nom de la méthode (findBy|firstBy|lastBy|countBy)...
     * @param array $params les paramètre d'appel de la fonction
     * @return \Collection object ou int suivant la méthode appelée
     * @throws \Exception
     */
    public static function __callStatic($method, $params) {
        if (!\preg_match('/^(find|first|last|count)By(\w+)$/', $method, $matches)) {
            throw new \Exception("Call to undefined method {$method}");
        }

        $criteriaKeys = \array_map('strtolower', \explode('_And_', \preg_replace('/([a-z0-9])([A-Z])/', '$1_$2', $matches[2])));
        $criteriaValues = \array_slice($params, 0, \count($criteriaKeys));
        $criteria = \array_combine($criteriaKeys, $criteriaValues);

        $methods = $matches[1];
        return static::$methods($criteria);
    }

    /**
     *    Recherche tous les éléments d'une base de données en fonction d'un tableau de paramètres
     * @param array $_params les paramètres de recherchee
     * @return \Collection les données trouvées
     */
    public static function find($_params) {
        $params = array();
        foreach ($_params as $key => $value) {
            $params[] = "$key = '$value'";
        }
        return static::allBy(implode(" AND ", $params));
    }

    /**
     *    Recherche tous les éléments d'une base de données en fonction d'un tableau de paramètres
     * @param array $_params les paramètres de recherchee
     * @return \Collection les données trouvées
     */
    public static function findById($_id) {
        return static::allBy("id = " . $_id)->first();
    }

    /**
     * Surcharge de __get pour donner acces aux valeurs sous la forme $e->attribut
     *
     * @param type $_cle Nom de la propriete
     * @return string or int or array or object suivant la propriété
     * @throws \Exception
     */
    public function __get($_cle) {
        if (array_key_exists($_cle, $this->valeurs)) { // La propriete demandee est bien une cle du tableau des valeurs
            return $this->valeurs[$_cle];
        } else if (array_key_exists($_cle, static::$has_one)) {
            $id_assoc = static::$has_one[$_cle]['primary_key'];
            $class = static::$has_one[$_cle]['class'];
            if (!array_key_exists($_cle, static::$relations_elem) || static::$relations_elem[$_cle] == null) {
                static::$relations_elem[$_cle] = new $class;
                if ($this->get_primary_key_value()) {
                    static::$relations_elem[$_cle] = $class::first(array("$id_assoc" => $this->get_primary_key_value()));
                }
            }
        }
        else if (array_key_exists($_cle, static::$belongs_to)) {
            echo $id_assoc = static::$belongs_to[$_cle]['primary_key'];
            echo $class = static::$belongs_to[$_cle]['class'];
            if (!array_key_exists($_cle, static::$relations_elem) || static::$relations_elem[$_cle] == null) {
                static::$relations_elem[$_cle] = new \Collection();
                if ($this->$id_assoc) {
                    static::$relations_elem[$_cle] = $class::first("id = {$this->$id_assoc}");
                }
            }
        }
        else if (array_key_exists($_cle, static::$has_and_belongs_to_many)) {
            $id_assoc = static::getClassName() . '_id';
            $id_assoc_2 = static::$has_and_belongs_to_many[$_cle]['primary_key'];
            $_join_table = static::$has_and_belongs_to_many[$_cle]['join_table'];
            $class = static::$has_and_belongs_to_many[$_cle]['class'];
            if (!array_key_exists($_cle, static::$relations_elem) || static::$relations_elem[$_cle] == null) {
                static::$relations_elem[$_cle] = new \Collection();
                if ($this->get_primary_key_value()) {
                    // Execution de la requete avec le parametre et lecture du resultat
                    $enregistrements = QueryManager::get()->
                            select($class::table())->
                            join($_join_table, array($id_assoc_2, static::cle_pri()))->
                            where($_join_table . '.' . $id_assoc . ' = ' . $this->get_primary_key_value())
                            ->execute();
                    foreach ($enregistrements as $enregistrement) {
                        static::$relations_elem[$_cle]->push($class::load($enregistrement));
                    }
                }
            }
        } else if (array_key_exists($_cle, static::$has_many)) {
            var_dump('ici');
            echo PHP_EOL;
            echo PHP_EOL;
            echo PHP_EOL;
            echo PHP_EOL;
            echo PHP_EOL;
            echo $id_assoc = static::$has_many[$_cle]['primary_key'];
            echo $class = static::$has_many[$_cle]['class'];
            if (!array_key_exists($_cle, static::$relations_elem) || static::$relations_elem[$_cle] == null) {
                static::$relations_elem[$_cle] = new $class;
                if ($this->get_primary_key_value()) {
                    static::$relations_elem[$_cle] = $class::AllBy("$id_assoc = {$this->get_primary_key_value()}");
                }
            }
        } else {
            throw new \Exception("Attribut '$_cle' inconnu dans '" . get_class($this) . "'");
        }
        return static::$relations_elem[$_cle];
    }

    /**
     *    Surcharge de __set pour donner acces aux valeurs sous la forme $e->attribut=val
     *
     * @param string $_cle Nom de la propriete
     * @param object $_val Sa nouvelle valeur
     * @return object la nouvelle valeur de l'objet
     * @throws \Exception
     */
    public function __set($_cle, $_val){
        if (array_key_exists($_cle, $this->valeurs)) { // La propriete demandee est bien une cle du tableau des valeurs
            return $this->valeurs[$_cle] = $_val;
        } elseif (array_key_exists($_cle, static::$relations_elem)) {
            return static::$relations_elem[$_cle] = $_val;
        }
        throw new \Exception("Attribut '$_cle' inconnu dans '" . get_class($this) . "'");
    }

    /**
     * Surcharge de __isset pour donner acces aux valeurs sous la forme isset($e->attribut)
     * @param string $_cle Nom de la propriete
     * @return boolean
     * @throws \Exception
     */
    public function __isset($_cle) {
        if (array_key_exists($_cle, $this->valeurs)) { // La propriete demandee est bien une cle du tableau des valeurs
            return isset($this->valeurs[$_cle]);
        } elseif (array_key_exists($_cle, static::$relations_elem)) {
            isset(static::$relations_elem[$_cle]);
        }
        throw new \Exception("Attribut '$_cle' inconnu dans '" . get_class($this) . "'");
    }

    /**
     * Surcharge de __unset pour donner acces aux valeurs sous la forme unset($e->attribut)
     * @param string $_cle Nom de la propriete
     * @return boolean
     * @throws \Exception
     */
    public function __unset($_cle /** Nom de la propriete */) {
        if (array_key_exists($_cle, $this->valeurs)) {// La propriete demandee est bien une cle du tableau des valeurs
            return $this->valeurs[$_cle] = null;
        } elseif (array_key_exists($_cle, static::$relations_elem)) {
            unset(static::$relations_elem[$_cle]);
        }
        throw new \Exception("Attribut '$_cle' inconnu dans '" . get_class($this) . "'");
    }

    /**
     * Chargement de l'objet à partir d'attribut
     * @param array $attributes tous les attibuts à charger
     * @return \ORM\class
     */
    /*public static function load($attributes) {
        $class = get_called_class();
        $record = new $class();
        $record->chargement($attributes);
        return $record;
    }*/

    /**
     *    Sauvegarde de l'objet en base de données si celui ci à passé les validations
     * @return boolean renvoie TRUE si l'objetà pu etre sauvegardé
     */
    public function save() {
        if (Validation::validate($this, static::$validate, static::cle_pri())) {
            return false;
        }
        return $this->ecriture();
    }
}