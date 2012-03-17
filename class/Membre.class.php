<?php
/**
 * Membre Class Doc Comment
 *
 * @category Class
 * @package  include.class
 * @author bryyce
 * @license  http://www.gnu.org/copyleft/gpl.html GNU General Public License
 * @link     http://www.bryyce.fr/
 */
class Membre extends MyEnregistrement
{
    private $_parrain = null;
    private $_situation_membre = null;
    private $_messages = null;
    private $_commandes = null;
    private $_status = null;
    /**
     * Constructeur de InstantGagnant
     *
     * @param <int> $_id id de l'objet a chercher dans la BDD si null un objet vide est créé
     */
    public function __construct($_id=null) {
        parent::__construct($_id);
    }


    /**
     *  Va chercher les commentaires liée à l'article en cours
     *
     * @return <Array<Commentaire>>
     */
    public function situationMembre() {
        if ($this->_situation_membre == null) {
            if ($this->id_situation_joueur != null) {
                $this->_situation_membre= SituationMembre::AllBy("id = {$this->id_situation_joueur}");
                $this->_situation_membre= $this->_situation_membre[0] ;
            }
            else
                $this->_situation_membre = new SituationMembre();
        }
        return $this->_situation_membre;
    }

    public function Commandes() {
        // Preparation de la requete
        if ($this->_commandes == null) {
            if ($this->id != null) 
                $this->_commandes = Commande::AllBy("id_membre = {$this->id}");
            else
                $this->_commandes = array();
        }
        return $this->_commandes;
    }

    public function Messages() {
        // Preparation de la requete
        if ($this->_messages == null) {
            if ($this->id != null) 
                $this->_messages = Message::AllBy("id_membre = {$this->id}");
            else
                $this->_messages = array();
        }
        return $this->_messages;
    }

    public function Status() {
        // Preparation de la requete
        if ($this->_status == null) {
            if ($this->id_status != null) {
                $this->_status = Status::AllBy("id = {$this->id_status}");
                $this->_status = $this->_status[0] ;
            }
            else
                $this->_status = new Status();
        }
        return $this->_status;
    }

    public function Parrain() {
        // Preparation de la requete
        if ($this->_parrain == null) {
            if ($this->id_parrain) {
                $this->_parrain = Parrain::AllBy("id = {$this->id_parrain}");
                $this->_parrain= $this->_parrain[0] ;
            }else
                $this->_parrain = new Membre();
        }
        return $this->_parrain;
    }
   

    //***********************************//
    //  Fin Fonctions membres protégées  //
    //***********************************//
    public function validation() {
        $msg = "<ul>";
        if (count($this->dispo('pseudo')) == 1)
            $msg .= 'Ce pseudo existe déjà, veuillez en choisir un autre';
        if (count($this->dispo('mail')) == 1)
            $msg .= 'Cette adresse e-mail existe déjà, veuillez en choisir une autre';
        if (count($this->dispo('ip')) == 1)
            $msg .= 'Un compte par ordinateur est autorisé';
        if (strlen($this->pseudo) > 15 OR strlen($this->pseudo) < 3)
            $msg .= 'Vous devez choisir un pseudo compris entre 3 et 15 caractères';
        if (preg_match("#[^a-zA-Z0-9_-]#", $this->pseudo))
            $msg .= 'La syntaxe de votre pseudo est incorrecte';
        if (strlen($this->code) < 6 OR strlen($this->code) > 21)
            $msg .= 'Vous devez choisir un code secret compris entre 6 et 21 caractères';
        if (!preg_match("#^[a-z0-9._-]+@[a-z0-9._-]{2,}\.[a-z]{2,4}$#", $this->mail))
            $msg .= 'La syntaxe de votre adresse e-mail est incorrecte';
        if ($msg != "<ul>")
            throw new Exception($msg . "</ul>");
    }


    public static function Admins() {
        return self::AllBy("niveau!=1");
    }

    public static function Bannis() {
        return self::AllBy("banni=1");
    }

    public static function Connectes() {
        return self::AllBy("connexion >= DATE_SUB(NOW(), INTERVAL 5 MINUTE)");
    }

    public static function Inactifs() {
        return self::AllBy("connexion <= DATE_SUB(NOW(), INTERVAL 7 DAY)");
    }
    
    public static function ByParrain($id) {
        return self::AllBy("id_parrain=" . myPDO::get()->quote($id) . "");
    }
}
?>