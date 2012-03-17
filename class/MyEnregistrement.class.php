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
        $this->initAttributs(self::etiquettes());
        if ($_id != null) {
            $this->lecture($_id);
            //  $this->date = new DateTime($this->date);
        }
    }

    public function etiquettes() {
        return Config::getFields(self::table());
    }

    //	Donne la cle primaire de la table.
    protected static function cle_pri() {
        return self::CLE_PRI;
    }
    /**php 5.3 seulement**/
    //	Donne la table contenant les Departement.
    protected static function table() {
        return strtolower(get_called_class())."s";
    }

    protected static function AllBy($cond){
        $class = get_called_class();
        $res = myPDO::get()->prepare(
<<<SQL
    SELECT *
    FROM {$class::table()}
         WHERE $cond
SQL
        );
        $records = array();
        // Execution de la requete avec le parametre et lecture du resultat
        if ($res->execute() && $enregistrements = $res->fetchAll(PDO::FETCH_ASSOC)) {
            // Affectation des valeurs 
            foreach ($enregistrements as $enregistrement) 
                $records[] = new $class ($enregistrement[$class::CLE_PRI]);
        } 
        return $records;
    }
    public static function All(){
        return self::AllBy("1=1");
        
    }
}
?>