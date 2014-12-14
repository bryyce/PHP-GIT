<?php

namespace Lib\Form;
/**
 * Session Class Doc Comment
 * @category  Class
 * @package    Lib
 * @author    bryyce
 * @license    http://www.gnu.org/copyleft/gpl.html GNU General Public License
 * @link    http://www.bryyce.fr/
 */
class Form {
    protected $_action = NULL;  
    protected $_method = NULL;  
    protected $_inputs = NULL;    
    protected $_labels = NULL;

    
    public function __construct($action, $method = 'POST') {
        $this->_action = $action;
        $this->_method = $method;
        $this->_inputs = new \Collection();
        $this->_labels = new \Collection();
    }


    public function render() {
        return require(getLibBasePath() . 'View/form.phtml');
    }
    
    public function addInput($label, $input) {
        $this->_labels->push(new Label($label,['for' => $input->getHtmlId()]));
        $this->_inputs->push($input);
        return $this;
    }
    
}
