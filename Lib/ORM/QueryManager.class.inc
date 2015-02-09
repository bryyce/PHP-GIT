<?php
/**
 * QueryManager.class file
 * @filesource ./ORM/QueryManager.class
 * @package     ORM
 */
namespace Lib\ORM;

/**
 *
 * Description of QueryManager
 *
 * @category    Class
 * @package     ORM
 * @author      bryyce
 * @license     http://www.gnu.org/copyleft/gpl.html GNU General Public License
 * @link        http://www.bryyce.fr/
 */
class QueryManager {

	/**
	 * L'instance de gestionnaire de requête
	 *	@access private
	 * @static
	 * @var \ORM\QueryManager
	 */
	private static $instance;

	/**
	 * Compartiment SELECT de la requete SQL
	 *	@access private
	 * @var string
	 */
	private $select;

	/**
	 * Table à joindre dans la requete
	 *	@access private
	 * @var array
	 */
	private $join = array();

	/**
	 * Compartiment ORDER de la requete SQL
	 *	@access private
	 * @var string
	 */
	private $order;

	/**
	 * Compartiment WHERE de la requete SQL
	 *	@access private
	 * @var string
	 */
	private $where;

	/**
	 * Compartiment LIMIT de la requete SQL
	 *	@access private
	 * @var string
	 */
	private $limit;

	/**
	 * Est ce que la requete est de type COUNT
	 *	@access private
	 * @var boolean
	 */
	private $count;

	/**
	 * La table concernée par la requete SQL
	 *	@access private
	 * @var string
	 */
	private $table;

	/**
	 * Constructeur par defaut du manager de requête
	 * Ne fait rien
	 */
	public function __construct() {

	}

	/**
	 *	Construit la partie SELECT de la requête
	 * @param string	$table	La table qui concerne le coeur de la requête
	 * @param array	$columns	(Optionnel) Les colonnes recherchées
	 * @return \ORM\QueryManager l'instance (pour la chainabilité)
	 */
	public function select($table, $columns = array()) {
		$this->select = 'SELECT '.$table. '.' . (count($columns) == 0 ? '*' : join(', '.$table. '.', $columns)) . ' FROM ' . $table;
		$this->table = $table;
		return $this;
	}

	/**
	 *	Construit la partie JOIN de la requête
	 * Utilisation de LEFT JOIN pour pouvoir ramener tous les éléments
	 *
	 * @param string $tables		(Optionnel) Les tables à joindre par default string mais peut etre array dans le cas d'une jointure multiple
	 * @param array $foreign_keys	(Optionnel) Les clés étrangeres pour construire les jointures
	 * @return \ORM\QueryManager l'instance (pour la chainabilité)
	 */
	public function join($tables = array(), array $foreign_keys = array()) {
		$table_2 = $this->table;
		if (!is_array($tables)) {
            $table = $tables;
        } else {
            if (array_key_exists(0, $tables)) {
                $table = $tables[0];
            }
            if (array_key_exists(1, $tables)) {
                $table_2 = $tables[1];
            }
        }
        $id_join_1 = $id_join_2 = $this->table . '_id';
		if (array_key_exists(0, $foreign_keys)) {
            $id_join_1 = $foreign_keys[0];
        }
        if (array_key_exists(1, $foreign_keys)) {
            $id_join_2 = $foreign_keys[1];
        }
        $this->join [] = 'LEFT JOIN ' . $table . ' ON ' . $table . '.' . $id_join_1 . ' = ' . $table_2 . '.' . $id_join_2;
		return $this;
	}

	/**
	 * Construit la partie ORDER de la requête
	 * @param array $columns les colonnes à trier string autorisé pour le mode monocolonne
	 * @return \ORM\QueryManager l'instance (pour la chainabilité)
	 */
	public function order($columns) {
    if (!is_null($columns)) {
      if (!is_array($columns)) {
        $columns = array($columns);
      }
      $this->order = 'ORDER BY ' . join(', ', $columns);
    }
		return $this;
	}

	/**
	 *	Construit la partie WHERE de la requête
	 * @param array $conditions les différentes conditions SQL comme ceci array('a = 1','b >= 2') mais string autorisé
	 * @return \ORM\QueryManager l'instance (pour la chainabilité)
	 */
	public function where($conditions) {
		if (is_array($conditions)) {
			if (count($conditions) > 0) {
                $this->where = 'WHERE ' . join(' AND ', $conditions);
            }
        }
		else if ($conditions != '') {
            $this->where = 'WHERE ' . $conditions;
        }
        return $this;
	}

	/**
	 * Construction de la LIMIT SQL
	 *
	 * @param int $number Nombre d'élément à récupérer
	 * @param int $offset (Optionnel) Décallage dans la limite
	 * @return \ORM\QueryManager l'instance (pour la chainabilité)
	 */
	public function limit($number, $offset = 0) {
		$this->limit = 'LIMIT ' . $offset . ', ' . $number;
		return $this;
	}

	/**
	 *	Défini la requête en mode count
	 * @return \ORM\QueryManager l'instance (pour la chainabilité)
	 */
	public function count() {
		$this->count = true;
		return $this;
	}

	/**
	 *	Remet l'instance à zéro
	 */
	public function reset() {
		$this->select = $this->where = $this->order = $this->limit = null;
		$this->join = array();
		$this->count = false;
	}

	/**
	 *	Construction de la requete SQL avec tous les éléments qui ont été fourni
	 * @return string la requete SQL
	 */
	public function expectedQuery() {
		return ($this->count ? 'SELECT COUNT(*) as count FROM '.$this->table : $this->select ) . ' ' .
				join(' ', $this->join) . ' ' .
				$this->where . ' ' .
				$this->order . ' ' .
				$this->limit;
	}

	/**
	 * Execute la requete
	 * @return \Collection la collection d'enregistrement en sous-tableau
	 */
	public function execute() {
		$query = myPDO::get()->prepare(
				$this->expectedQuery()
		);
		$enregistrements = new \Collection();
		if ($query->execute()) {
            $enregistrements->join($query->fetchAll(\PDO::FETCH_ASSOC));
        }
        $this->reset();
		return $enregistrements;
	}

    /**
     *
     * @param type $db_name
     */
    public function createDatabase($db_name) {
        myPDO::get()->prepare('CREATE DATABASE ' . $db_name)->execute();
    }

    public function createTable($table_name, $columns, $engine, $options){
        $_columns = [];
        foreach ($columns as $column) {
            $_columns[] = "{$column->getName()} {$column->getType()} {$column->getOption('default_value')}";
        }
        $sql = "CREATE TABLE $table_name ("
                . "id int primary key not null auto_increment,"
                . join(',', $_columns)
                . " ) Engine $engine";
        myPDO::get()->prepare($sql)->execute();
    }

    public function dropTable($table_name) {
        myPDO::get()->prepare("DROP TABLE $table_name")->execute();
    }

    public function executeRawQuery($query) {
        myPDO::get()->prepare($query)->execute();
    }

    public function beginTransaction() {
        myPDO::get()->beginTransaction();
    }

    public function commit() {
        myPDO::get()->commit();
    }

    public function rollback() {
        myPDO::get()->rollback();
    }

    /**
	 * Interdit le clonage du singleton
	 * @throws Exception
	 */
	final public function __clone() {
		throw new Exception("Clonage de " . __CLASS__ . " interdit !");
	}

	/**
	 * Récuperer le singleton
	 * @return \Lib\ORM\QueryManager
	 */
	final public static function get() {
		// Une instance est-elle disponible ?
		if (!isset(self::$instance)) {
            self::$instance = new QueryManager();
        }
		return self::$instance;
	}

}