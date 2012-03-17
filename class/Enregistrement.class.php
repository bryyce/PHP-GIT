<?php
/**
 * Enregistrement Class Doc Comment
 *
 * @package  Include.class
 * @author bryyce
 * @license  http://www.gnu.org/copyleft/gpl.html GNU General Public License
 * @link     http://www.bryyce.fr/
 */
require_once 'Connexion.inc.php' ;
/**
 *  Modele abstrait d'enregistrement
 */
abstract class Enregistrement
{
    /// Valeurs de l'enregistrement :
    /// tableau associatif de la forme nom_du_champ => valeur_pour_l_enregistrement
    protected $valeurs = array();
    /// Constructeur
    abstract public function __construct($_id=null /** Identifiant */);

    public function lecture($_id /** Identifiant */) {
        // Preparation de la requete
        $res = myPDO::get()->prepare(
<<<SQL
    SELECT *
    FROM {$this->table()}
    WHERE {$this->cle_pri()} = ?
SQL
   	);
        // Execution de la requete avec le parametre et lecture du resultat
        if ($res->execute(array($_id)) && $enregistrement = $res->fetch(PDO::FETCH_ASSOC)) {
           // Affectation des valeurs
           $this->chargement($enregistrement);
        }
        else {
            // Lecture impossible
            throw new Exception("Lecture de l'enregistrement {$_id} impossible");
        }
    }
    /// Donne le nom de la table contenant les informations
    abstract protected static function table();

    /// Donne le nom de la cle primaire de la table
    abstract protected static function cle_pri();

    abstract public function etiquettes();
    /// Donne une etiquette particuliere
    public function etiquette($_cle) {
        $etiquettes = $this->etiquettes();
        return $etiquettes[$_cle] ;
    }

    /// Methode d'ecriture dans la base de donnees
    public function ecriture() {
        $_table = $this->table();
        $_cle_pri = $this->cle_pri();
        // La cle est-elle definie ?
        // DECOMMENTER LE IF POUR FAIRE LE TEST DE PRESENCE DE CLE PRIMAIRE
        //if (isset($this->$_cle_pri)) { // Forme tordue de $this->valeurs[$_cle_pri] possible grace a la surcharge de __get()
        // L'Enregistrement existe-t-il dans la base de donnees ?
        // Cela revient a compter le nombre de lignes de la BD qui portent l'identifiant de l'objet
        // 0 => n'existe pas
        // 1  => existe
        // >1 => ne peut pas arriver pour une cle primaire
        $existe = myPDO::get()->prepare(
<<<SQL
    SELECT COUNT(*) "n"
    FROM $_table
    WHERE $_cle_pri=?
SQL
        );
        $existe->execute(array($this->$_cle_pri));
        $nb = $existe->fetch();
        if ($nb['n'] == 1) {
            // l'Enregistrement existe => UPDATE
            // Constitution de la requete
            // Pour chaque attribut...
            foreach (array_keys($this->valeurs) as $cle) {
                // ... attribut=:attribut
                if (!is_null($this->valeurs[$cle]) && !(empty($this->valeurs[$cle]) && !is_numeric($this->valeurs[$cle])))
                    $set[] = "{$cle}=:{$cle}";
            }
            // Preparation de la requete
            $res = myPDO::get()->prepare("UPDATE $_table SET " . implode(",", $set) . " WHERE $_cle_pri=:$_cle_pri");
        }
        else {
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
                // $res->bindValue(":{$cle}", null, PDO::PARAM_null);
            } else {
                // Affectation de la valeur de l'attribut au parametre
                $res->bindValue(":{$cle}", $valeur);
            }
        }
        // Execution de la requete
        $res->execute();
        /* }
          else // La cle est-elle definie ? => NON !
          throw new PDOException("Enregistrement impossible : Aucune clé définie"); */
    }

    /// Affichage de l'enregistrement
    public function affichage($_deb=''       /** Texte a placer avant chaque libelle */,
            $_mil=' : '    /** Texte a placer entre le libelle et la valeur */,
            $_fin="<br>\n" /** Texte a placer apres la valeur */
            ) {
        foreach ($this->etiquettes() as $cle => $etiquette) {
            echo "{$_deb}$etiquette{$_mil}{$this->$cle}{$_fin}" ;
        }
    }

/// Production des elements de formulaire de l'enregistrement
    public function formulaire($_deb=''       /** Texte a placer avant chaque libelle */,
            $_mil=' : '    /** Texte a placer entre le libelle et la valeur */,
            $_fin="<br>\n" /** Texte a placer apres la valeur */
            ) {
        foreach ($this->etiquettes() as $cle => $etiquette) {
            $valeur = htmlentities($this->$cle);
            if($cle!=$this->cle_pri())
                echo "{$_deb}$etiquette{$_mil}<input type='text' name='$cle' value=\"{$valeur}\">{$_fin}" ;
        }
    }

    /// Chargement des valeurs a partir d'un tableau associatif de meme forme que $this->valeurs
    public function chargement($_donnees /** Tableau contenant les valeurs */) {
        // Pour chaque attribut, affecter la valeur si elle est presente dans le tableau
        foreach (array_keys($this->valeurs) as $k) 
            if (array_key_exists($k, $_donnees)) 
                $this->$k = $_donnees[$k] ;
    }

    /// Initialisation des noms des attributs qui sont les cles du tableau des valeurs
    protected function initAttributs($_cles /** Tableau dont les cles sont les noms des attributs */) {
        // Initialisation du tableau des valeurs
        $this->valeurs = array();
        // Pour chaque cle passee en parametre...
        foreach (array_keys($_cles) as $cle) // ...en faire une cle du tableau des valeurs
            $this->valeurs[$cle] = null ;
    }

    /// Surcharge de __get pour donner acces aux valeurs sous la forme $e->attribut
    public function __get($_cle /** Nom de la propriete */) {
        if (array_key_exists($_cle, $this->valeurs)) // La propriete demandee est bien une cle du tableau des valeurs
            return $this->valeurs[$_cle] ;
        throw new Exception("Attribut '$_cle' inconnu dans '".get_class($this)."'");
    }

    /// Surcharge de __set pour donner acces aux valeurs sous la forme $e->attribut=val
    public function __set($_cle /** Nom de la propriete */, $_val /** Sa nouvelle valeur */) {
        if (array_key_exists($_cle, $this->valeurs)) // La propriete demandee est bien une cle du tableau des valeurs
            return $this->valeurs[$_cle] = $_val ;
        throw new Exception("Attribut '$_cle' inconnu dans '".get_class($this)."'");
    }

    /// Surcharge de __isset pour donner acces aux valeurs sous la forme isset($e->attribut)
    public function __isset($_cle /** Nom de la propriete */) {
        if (array_key_exists($_cle, $this->valeurs)) // La propriete demandee est bien une cle du tableau des valeurs
            return isset($this->valeurs[$_cle]);
        throw new Exception("Attribut '$_cle' inconnu dans '".get_class($this)."'");
    }

    /// Surcharge de __unset pour donner acces aux valeurs sous la forme unset($e->attribut)
    public function __unset($_cle /** Nom de la propriete */) {
        if (array_key_exists($_cle, $this->valeurs))// La propriete demandee est bien une cle du tableau des valeurs
            return $this->valeurs[$_cle] = null;
        throw new Exception("Attribut '$_cle' inconnu dans '" . get_class($this) . "'");
    }
}
?>