<?php
/**
 * Config Class Doc Comment
 *
 * @category Class
 * @package  include.class
 * @author bryyce
 * @license  http://www.gnu.org/copyleft/gpl.html GNU General Public License
 * @link     http://www.bryyce.fr/
 */
class Config extends MyEnregistrement
{
    const TABLE = "config";
    /**
     *  Constructeur de config
     */
    public function __construct() {
        MyEnregistrement::$_labels = array('id'=>'id', 'maintenance'=>'maintenance', 'titre' => "titre", 'partenaires' => "partenaires", 'points_donnes' => 'points_donnes', 'montant_cagnotte' => 'montant_cagnotte');
        parent::__construct(1);
    }
}
?>
