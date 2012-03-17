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
    const TABLE = 'membres';
    /**
     * Constructeur de InstantGagnant
     *
     * @param <int> $_id id de l'objet a chercher dans la BDD si null un objet vide est créé
     */
    public function __construct($_id=null) {
        MyEnregistrement::$_labels = array('id' => 'id', 'pseudo' => "pseudo", 'code' => "code", 'mail' => 'e-mail', 'verif' => 'verif',
        'nom' => 'nom', 'prenom' => 'prénom', 'adresse' => 'adresse', 'cp' => 'code postal', 'ville' => 'ville', 'pays' => 'pays',
        'paypal' => 'paypal', 'inscription' => 'inscription', 'connexion' => 'connexion', 'ip' => 'ip', 'banni' => 'banni',
        'niveau' => 'niveau', 'id_parrain' => 'id_parrain', 'id_situation_joueur' => 'id_situation_joueur', 'id_status' => 'id_status');
        parent::__construct($_id);
    }

    /**
     * Fonction de rédaction d'article
     *
     * @param <bool> $error : vrai si il y a eu une erreur de saisie
     */
    public function formulaire($error=false, $message=null) {
        if ($error)
            echo "<h2>Erreur lors de la connexion</h2>";
        echo '<form action="" method="post" accept-charset="utf-8">';
        echo '<p><label for="pseudo_form"><img src="/images/choix_pseudo.png" alt="Choisissez votre pseudo" /></label><br /><input type="text" value="' . $this->pseudo . '" name="pseudo" id="pseudo_form" class="text_form" /></p>';
        echo '<p><label for="pass_form"><img src="/images/choix_pass.png" alt="Choisissez votre pass" /></label><br /><input type="password"  name="mdp" id="pass_form" class="text_form" /></p>';
        echo '<p><label for="mail_form"><img src="/images/choix_mail.png" alt="Votre Email" /></label><br /><input type="text" name="mail"  value="' . $this->mail . '" id="mail_form" class="text_form" /></p>';
        echo '<p><label for="parrain_form"><img src="/images/parrain.png" alt="Votre Parrain" /></label><br /><input type="text" name="parrain" id="parrain_form" class="text_form"  value="' . $this->id_parrain . '"/></p>'; // value="<?php if(isset($_GET['parrain']) AND !empty($_GET['parrain'])) echo intval($_GET['parrain']); /></p>';
        echo '<p><input type="image" name="valider" src="/images/valider.png" /></p></form>';
    }

    /**
     *  Va chercher les commentaires liée à l'article en cours
     *
     * @return <Array<Commentaire>>
     */
    public function situationMembre() {
        if ($this->_situation_membre == null) {
            if ($this->id_situation_joueur != null) {
                $this->_situation_membre= parent::AllBy("SituationMembre", "id = {$this->id_situation_joueur}");
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
            if ($this->id != null) {
                $this->_commandes = parent::AllBy("Commande", "id_membre = {$this->id}");
            }
            else
                $this->_commandes = array();
        }
        return $this->_commandes;
    }

    public function Messages() {
        // Preparation de la requete
        if ($this->_messages == null) {
            if ($this->id != null) {
                $this->_messages = parent::AllBy("Message", "id_membre = {$this->id}");
            } else
                $this->_messages = array();
        }
        return $this->_messages;
    }

    public function Status() {
        // Preparation de la requete
        if ($this->_status == null) {
            if ($this->id_status != null) {
                $this->_status = parent::AllBy("Status", "id = {$this->id_status}");
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
                $this->_parrain = parent::AllBy(__CLASS__, "id = {$this->id_parrain}");
                $this->_parrain= $this->_parrain[0] ;
            }else
                $this->_parrain = new Membre();
        }
        return $this->_parrain;
    }
    public function dispo($item=null) {
        // Preparation de la requete
        $t = Membre::TABLE;
        switch ($item) {
            case 'pseudo':
                $res = myPDO::get()->prepare("SELECT pseudo FROM $t WHERE pseudo=?");
                $res->execute(array($this->pseudo));
                break;
            case 'mail':
                $res = myPDO::get()->prepare("SELECT mail FROM $t WHERE mail=?");
                $res->execute(array($this->mail));
                break;
            case 'ip':
                $res = myPDO::get()->prepare("SELECT ip FROM $t WHERE ip=?");
                $res->execute(array($this->ip));
                break;
            default: return false;
        }
        return $res->fetchAll();
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

    public function isAdmin() {
        return $this->niveau == 1 && $this->niveau != null ;
    }

    public static function Admins() {
        return parent::AllBy(__CLASS__, "niveau!=1");
    }

    public static function Bannis() {
        return parent::AllBy(__CLASS__, "banni=1");
    }

    public static function Connectes() {
        return parent::AllBy(__CLASS__, "connexion >= DATE_SUB(NOW(), INTERVAL 5 MINUTE)");
    }

    public static function Inactifs() {
        return parent::AllBy(__CLASS__, "connexion <= DATE_SUB(NOW(), INTERVAL 7 DAY)");
    }

    public static function All() {
        return parent::AllBy(__CLASS__, "1=1");
    }

    public static function ByParrain($id) {
        return parent::AllBy(__CLASS__, "id_parrain=" . myPDO::get()->quote($id) . "");
    }
}
?>