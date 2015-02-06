<?php

/**
 * Enregistrement.class file
 * @filesource ./ORM/Enregistrement.class
 */
/**
 * Enregistrement Class Doc Comment
 *
 * @category    Class
 * @package        ORM
 * @author        bryyce bric.robin@gmail.com
 * @license    http://www.gnu.org/copyleft/gpl.html GNU General Public License
 * @link            http://www.bryyce.fr/
 */

namespace Lib\ORM;

/**
 *  Modele abstrait d'enregistrement
 */
abstract class Enregistrement {

    /**
     * Valeurs de l'enregistrement : tableau associatif de la forme nom_du_champ => valeur_pour_l_enregistrement
     * @var array
     * @access protected
     */
    protected $valeurs = array();

    /**
     * Constructeur
     * @param int $_id (Optionnel) Identifiant en base de données (préférence int mais string possible)
     * @abstract
     */
    abstract public function __construct($_id = null /** Identifiant */);

    /**
     * Charge un enregistrement en fonction d'un identifiant de base de données
     * @param int $_id Identifiant en base de données (préférence int mais string possible)
     * @throws \Exception
     */
    public function lecture($_id) {
        $records = QueryManager::get()->
                  select($this->table())->
                  where($this->cle_pri() . " = " . $_id)->
                  execute();
          if ($records->count() == 1) {
            $this->chargement($records->first());
        } else {// Lecture impossible
            throw new \Exception\NotFoundException("Lecture de l'enregistrement {$_id} impossible");
        }
    }

    /**
     * Donne le nom de la table contenant les informations
     */
     protected static function table(){}

    /**
     * Donne le nom de la cle primaire de la table
     */
     protected static function cle_pri(){}

    /**
     * Donne le nom des champs de la base de données
     */
    abstract public function etiquettes();

    /**
     * Donne une etiquette particuliere
     * @param string $_cle le nom de la clé
     * @return string l'etiquette
     */
    public function etiquette($_cle) {
        $etiquettes = $this->etiquettes();
        return $etiquettes[$_cle];
    }

    /**
     * Methode d'ecriture dans la base de donnees
     * Vérifie si la clé primaire existe en base de données avant de sauvegarder
     * s'il existe créer un enregistrement sinon il le mets à jour
     */
    public function ecriture() {
        $_table = $this->table();
        $_cle_pri = $this->cle_pri();

        if (static::count(array(static::cle_pri() => (int)$this->$_cle_pri)) == 1) {
            // l'Enregistrement existe => UPDATE
            // Constitution de la requete
            // Pour chaque attribut...
            foreach (array_keys($this->valeurs) as $cle) {
                // ... attribut=:attribut
                if (!is_null($this->valeurs[$cle]) && !(empty($this->valeurs[$cle]) && !is_numeric($this->valeurs[$cle]))) {
                    $set[] = "{$cle}=:{$cle}";
                }
            }
            // Preparation de la requete
            $res = myPDO::get()->prepare("UPDATE $_table SET " . implode(",", $set) . " WHERE $_cle_pri=:$_cle_pri");
        } else {
            // L'Enregistrement n'existe pas => INSERT
            // Constitution de la requete
            // Pour chaque attribut...
            $col = array(); // ... liste des champs ...
            $val = array(); // et des valeurs parametrees
            foreach (array_keys($this->valeurs) as $cle) {
                if (!is_null($this->valeurs[$cle]) && !(empty($this->valeurs[$cle]) && !is_numeric($this->valeurs[$cle]))) {
                    $col[] = "{$cle}"; // ... liste des champs ...
                    $val[] = ":{$cle}"; // et des valeurs parametrees
                }
            }
            // Preparation de la requete
            $res = myPDO::get()->prepare("INSERT INTO $_table (" . implode(",", $col) . ") VALUES (" . implode(",", $val) . ")");
        }
        // Affectation des valeurs des valeurs
        foreach ($this->valeurs as $cle => $valeur) {
            if (is_null($valeur) // Valeur nulle
                      || (empty($valeur) && !is_numeric($valeur))) { // Valeur vide mais pas 0
                // Affectation de null au parametre
                // $res->bindValue(":{$cle}", null, \PDO::PARAM_null);
            } else {
                // Affectation de la valeur de l'attribut au parametre
                $res->bindValue(":{$cle}", $valeur);
            }
        }
        // Execution de la requete
        $res->execute();
        if ($this->$_cle_pri == ''){
            $this->$_cle_pri = myPDO::get()->lastInsertId();
        }
        return TRUE;
    }

    /**
     * Affichage de l'enregistrement
     * @deprecated ne pas utiliser fonction par default, préferer un template html php
     * @param string $_deb Texte a placer avant chaque libelle
     * @param string $_mil Texte a placer entre le libelle et la valeur
     * @param string $_fin Texte a placer apres la valeur
     */
    public function affichage($_deb = '', $_mil = ' : ', $_fin = "<br>\n") {
        foreach ($this->etiquettes() as $cle => $etiquette) {
            echo "{$_deb}$etiquette{$_mil}{$this->$cle}{$_fin}";
        }
    }

    /**
     * Chargement des valeurs a partir d'un tableau associatif de meme forme que $this->valeurs
     * @param array $_donnees Tableau contenant les valeurs
     */
    public function chargement($_donnees) {
        // Pour chaque attribut, affecter la valeur si elle est presente dans le tableau
        foreach (array_keys($this->valeurs) as $k) {
            if (array_key_exists($k, $_donnees)) {
                $this->$k = $_donnees[$k];
            }
        }
    }

    /**
     * Initialisation des noms des attributs qui sont les cles du tableau des valeurs
     * @param array $_cles  Tableau dont les cles sont les noms des attributs
     */
    protected function initAttributs($_cles) {
        // Initialisation du tableau des valeurs
        $this->valeurs = array();
        // Pour chaque cle passee en parametre...
        foreach (array_keys($_cles) as $cle) { // ...en faire une cle du tableau des valeurs
            $this->valeurs[$cle] = null;
        }
    }

    public function delete(){

        $_table = $this->table();
        $_cle_pri = $this->cle_pri();
        if (static::count(array(static::cle_pri() => $this->$_cle_pri)) == 1) {
            $res = myPDO::get()->prepare("DELETE FROM $_table  WHERE $_cle_pri=:$_cle_pri");
            $res->bindValue(":{$_cle_pri}", $this->$_cle_pri);
            $res->execute();
        }
    }
}

?>
