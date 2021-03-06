<?php

/**
 * Validation.class file
 * @filesource ./ORM/Validation.class
 * @package    ORM
 */
namespace Lib\ORM;

/**
 * Description of Validation
 *
 * @category  Class
 * @package    ORM
 * @author    bryyce
 * @license    http://www.gnu.org/copyleft/gpl.html GNU General Public License
 * @link    http://www.bryyce.fr/
 */
class Validation {

    /**
     * Fonction qui valide l'enregistrement en fonction de paramêtres définis
     *
     * @static
     * @param string    $entity        le nom de l'entité à valider
     * @param array    $validation les différentes validations
     * @param string $id                le nom de la clé primaire
     * @return boolean renvoie TRUE si toutes les validations ont été passées avec succès
     */
    public static function validate($entity, $validation, $id) {
        foreach ($validation as $test => $values) {
            switch ($test) {
                case 'unique':
                    foreach ($values as $value) {
                        if ($entity::count(array($value => $entity->$value), $entity->$id == 0 ? '' : ' and ' . $id . ' != ' . $entity->$id)) {
                            $entity->errors[$value] = 'not unique';
                        }
                    }
                    break;
                case 'presence':
                    foreach ($values as $value) {
                        if (self::isEmpty($entity->$value)) {
                            $entity->errors[$value] = 'is empty';
                        }
                    }
                    break;
                case 'numericality':
                    foreach ($values as $value) {
                        if (!self::isNumeric($entity->$value)) {
                            $entity->errors[$value] = 'is not numeric';
                        }
                    }
                    break;
                case 'length':
                    $value = key($values);
                    if(\is_array($values[$value])) {
                        $size = count($values[$value]);
                        if (strlen($entity->$value) < $size) {
                            $entity->errors[$value] = 'is too short (< ' . $size . ')';
                        }
                    } else {
                        switch(key($values[$value])){
                            case 'gt':
                                if (strlen($entity->$value) < $values[$value]['gt']) {
                                    $entity->errors[$value] = 'is too short (< ' . $values[$value]['gt'] . ')';
                                }
                                break;
                            case 'lt':
                                if (strlen($entity->$value) > $values[$value]['lt']) {
                                    $entity->errors[$value] = 'is too long (> ' . $values[$value]['lt'] . ')';
                                }
                                break;
                            case 'between':
                                if (strlen($entity->$value) > $values[$value]['between'][0] && strlen($entity->$value) < $values[$value]['between'][1]) {
                                    $entity->errors[$value] = 'is not between ( ' . $values[$value]['between'][0] . ' and ' . $values[$value]['between'][1] . ')';
                                }
                                break;
                        }
                    }
                    break;
            }
        }
        return count($entity->errors) > 0;
    }

    /**
     * Calcul si le parametre est un numerique
     *
     * @static
     * @param object $param une variable à tester
     * @return boolean renvoie TRUE si $param est un numérique
     */
    public static function isNumeric($param) {
        return preg_match("#^-?\d+.?\d+$#", $param);
    }

    /**
     * Calcul si le parametre est un entier
     *
     * @static
     * @param object $param une variable à tester
     * @return boolean renvoie TRUE si $param est un entier
     */
    public static function isInteger($param) {
        return preg_match("#^-?\d+$#", $param);
    }

    /**
     * Calcul si le paramêtre est vide
     *
     * @static
     * @param object $param une variable à tester
     * @return boolean renvoie TRUE si $param est vide
     */
    public static function isEmpty($param) {
        return $param === null || $param === "";
    }

    /**
     * Calcul si le paramêtre est unique
     *
     * @static
     * @param object $param une variable à tester
     * @return boolean renvoie TRUE si $param est unique
     */
    public static function isUnique($param) {
        return $param;
    }

}

?>
