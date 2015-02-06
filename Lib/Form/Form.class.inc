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
    protected $_name = NULL;
    protected $_action = NULL;
    protected $_method = NULL;
    protected $_inputs = NULL;
    protected $_labels = NULL;


    public function __construct($name, $action, $method = 'POST', $params = []) {
        $this->_name = $name;
        $this->_action = $action;
        $this->_method = $method;

        $this->_inputs = new \Collection();
        $this->_labels = new \Collection();
    }

    public function getAction() {
        return $this->_action;
    }

    public function getMethod() {
        return $this->_method;
    }

    public function label($label, $params = []) {
        return (new Label($label, $params + array("class" => "control-label")))->render();
    }

    public function text($name, $value, $params = []) {
        return (new TextInput($this->formatInputName($name), $value, $params + array("class" => "form-control")))->render();
    }

    public function password($name, $value, $params = []) {
        return (new PasswordInput($this->formatInputName($name), $value, $params + array("class" => "form-control")))->render();
    }

    public function radio($name, $value, $params = []) {
        return (new RadioInput($this->formatInputName($name), $value, $params + array("class" => "form-control")))->render();
    }

    public function checkbox($name, $value, $params = []) {
        return (new CheckboxInput($this->formatInputName($name), $value, $params + array("class" => "form-control")))->render();
    }

    public function submit($name, $value, $params = []) {
        return (new SubmitInput($name, $value, $params + array("class" => "btn btn-default")))->render();
    }

    private function formatInputName($name) {
        return $this->_name .'['.$name .']';
    }
    /*
    public function render() {
        return require(getLibBasePath() . 'View/form.phtml');
    }

    public function addInput($label, $input) {
        $this->_labels->push(new Label($label,['for' => $input->getHtmlId()]));
        $this->_inputs->push($input);
        return $this;
    }*/

}
