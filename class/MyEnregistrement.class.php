<?php
/**
 *
 * Description of MyEnregistrement
 *
 * @author bryyce
 */

abstract class MyEnregistrement extends Enregistrement{

    protected static $_labels = array();
    protected static $_table = "";
    const CLE_PRI="id";
    /**
     * Constructeur d'enregistrement
     *
     * @param <int> $_id id de l'objet a chercher dans la BDD si null un objet vide est créé
     */
    public function __construct($_id=null) {
        $this->initAttributs(self::$_labels);
        if ($_id != null) {
            $this->lecture($_id);
            //  $this->date = new DateTime($this->date);
        }
    }

    public function etiquettes() {
        return self::$_labels;
    }

    //	Donne la cle primaire de la table.
    protected static function cle_pri() {
        return self::CLE_PRI;
    }
    /**php 5.3 seulement**/
    //	Donne la table contenant les Departement.
    protected static function table() {
        return static::TABLE;
    }

    protected static function AllBy($class , $cond){
        $t = eval("return $class::table();");
        $res = myPDO::get()->prepare(
<<<SQL
    SELECT *
    FROM {$t}
         WHERE $cond
SQL
        );
        // Execution de la requete avec le parametre et lecture du resultat
        if ($res->execute() && $enregistrements = $res->fetchAll(PDO::FETCH_ASSOC)) {
            // Affectation des valeurs
            $membres = array();
            //var_dump($enregistrements);
            foreach ($enregistrements as $enregistrement) {
                $a = eval ("return new $class (".$enregistrement[eval (" return $class::CLE_PRI;")].");");
                //$a = new Membre($enregistrement[Membre::CLE_PRI]);
                $membres[] = $a;
            }
        } else {
            // Lecture impossible
            $membres = array();
        }
        return $membres;
    }
}
?>