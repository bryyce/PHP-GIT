<?php

/**
 * Collection.class file
 * @filesource ./Collection.class
 * @package		\
 */

/**
 * Description of Collection
 * @package		\
 * @category	Class
 *
 * @author		bryyce bric.robin@gmail.com
 * @license		http://www.gnu.org/copyleft/gpl.html GNU General Public License
 * @link			http://www.bryyce.fr/
 * @implement Iterator, ArrayAccess
 */
class Collection implements ArrayAccess, Iterator {

	/**
	 * 	La collection d'objet à  manipuler
	 * @access private
	 * @var array
	 */
	private $collection = array();

	/**
	 * 	l'index courant de la collection
	 * @access private
	 * @var int
	 */
	private $current_index = 0;

	/**
	 * Constructeur
	 * @param array $array le tableau qui servira a initialiser la collection
	 */
	public function __construct(array $array = array()) {
		$this->collection = $array;
	}

	/**
	 * 	Ajout un élément a la fin de la collection
	 * @param mixed $element élément à  ajouter
	 */
	public function push($element) {
		$this->collection[] = $element;
	}

	/**
	 * Supprime le dernier élément de la collection
	 * @return mixed Retourne la dernière valeur de la collection. Si la collection est vide, NULL sera retourné.
	 */
	public function pop() {
		return array_pop($this->collection);
	}

	/**
	 * Retourne le premier élément de la collection
	 * @return mixed Retourne le premier élément de la collection
	 */
	public function first() {
		return $this->collection[0];
	}

	/**
	 * Retourne le dernier élément de la collection
	 * @return mixed Retourne le dernier élément de la collection
	 */
	public function last() {
		return $this->collection[$this->count() - 1];
	}

	/**
	 * Retourne le nombre d'éléments de la collection
	 * @return int Retourne le nombre d'éléments de la collection
	 */
	public function count() {
		return count($this->collection);
	}

	/* public function next(){
	  $this->current_index ++;
	  if($this->current_index > $this->count())
	  return null;
	  return $this->collection[$this->current_index - 1];
	  }

	  public function prev(){
	  $this->current_index --;
	  if($this->current_index < 0)
	  return null;
	  return $this->collection[$this->current_index];
	  } */

	/**
	 * 	Fusionne un tableau avec la collection
	 * @param array $array le tableau a fusionner
	 * @return la collection obtenu après la fusion
	 */
	public function join(array $array) {
		return $this->collection = array_merge($this->collection, $array);
	}

	/**
	 * Replace le curseur à  zero
	 */
	public function reset() {
		$this->current_index = 0;
	}

	/**
	 * 	Supprime un élément de la collection
	 * @param mixed $element l'élément a supprimer
	 */
	public function remove($element) {
		foreach ($this->collection as $key => $value)
			if ($value === $element) {
				unset($this->collection[$key]);
				break;
			}
		$this->collection = array_values($this->collection);
	}

	/**
	 * Destruction de l'objet collection
	 */
	public function __destruct() {
		unset($this->collection);
	}

	/**
	 * 	Surcharge de l'operateur $array[X];
	 *
	 * @see ArrayAccess::offsetGet($offset)
	 * @param mixed $offset la clé recherchée
	 * @return mixed l'élément associé à  la clé
	 */
	public function offsetGet($offset) {
		return $this->collection[$offset];
	}

	/**
	 * 	Recherche si la clé existe
	 *
	 * @see ArrayAccess::offsetExists($offset)
	 * @param mixed $offset la clé recherchée
	 * @return boolean retourne TRUE s'il existe une clé du nom dans la collection
	 */
	public function offsetExists($offset) {
		return array_key_exists($offset, $this->collection);
	}

	/**
	 * Associe une clé a une valeur
	 *
	 * @see ArrayAccess::offsetSet($offset, $value)
	 * @param mixed $offset la clé recherchée
	 * @param mixed $value la valeur à  associer
	 * @return boolean retourne TRUE s'il existe une clé du nom dans la collection
	 */
	public function offsetSet($offset, $value) {
		$this->collection[$offset] = $value;
	}

	/**
	 * Detruit la valeur dans la collection
	 *
	 * @see ArrayAccess::offsetUnset($offset)
	 * @param mixed $offset la clé recherchée
	 * @return boolean retourne TRUE s'il existe une clé du nom dans la collection
	 */
	public function offsetUnset($offset) {
		unset($this->collection[$offset]);
	}

	/**
	 * Replace le curseur de la collection au debut de celle-ci
	 */
	public function rewind() {
		reset($this->collection);
	}

	/**
	 * 	Retourne l'élément courant de la collection
	 * @return mixed l'élément courant de la collection
	 */
	public function current() {
		return current($this->collection);
	}

	/**
	 * 	Retourne la clé de l'élément courant dans la collection
	 * @return mixed la clé de l'élémént courant
	 */
	public function key() {
		return key($this->collection);
	}

	/**
	 * 	Passe le curseur sur l'élément suivant et le retourne
	 * @return mixed le prochain élément de la collection
	 */
	public function next() {
		return next($this->collection);
	}

	/**
	 * 	Y'a t'il encore un élément dans la collection
	 * @return boolean retourne TRUE s'il existe encore un element
	 */
	public function valid() {
		$key = key($this->collection);
		return ($key !== NULL && $key !== FALSE);
	}

	public function toJson() {
		$res = [];
		foreach ($this->collection as $key => $value) {
			$res[]= $value->toJson();
		}
		return '[' . join(',', $res ) . ']';
	}
}

?>