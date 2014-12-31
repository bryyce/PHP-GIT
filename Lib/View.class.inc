<?php
namespace Lib;
class View {
    private $_file_prefix;
    private $_view_vars;
    private $_layout = 'layout.phtml';



    public function setView($file_prefix) {
        $this->_file_prefix = $file_prefix;
    }

    public function setVars($view_vars = []) {
        $this->_view_vars = $view_vars;
    }

    public function render($file = NULL, $variables = []) {
        require_once 'Helper/default.php';
        if (isset($variables['vars'])) {
            $this->_view_vars = $variables['vars'];
        }

        $this->_content = (is_null($file) ? $this->_file_prefix : $file) . '.phtml';

        $layout = 'layout/';
        if (!isset($variables['layout'])) { //&& !isset($variables['format'])) {
            $layout .= $this->_layout;
        } else {
            $layout .= $variables['layout'];
        }


        if ($layout === 'none' || !(file_exists(getViewsBasePath() . $layout))) {
            $this->getContent();
        } else {
            require getViewsBasePath(). $layout;
        }

        $renderedView = ob_get_clean();
        return $renderedView;
    }


    public function getContent() {
        extract($this->_view_vars);
        return require getViewsBasePath() . $this->_content;
    }

    public function partial($file, $variables = []) {
        if (isset($variables['vars'])) {
            extract($variables['vars']);
        } else {
            extract($this->_view_vars);
        }
        $file .= '.phtml';
        include getViewsBasePath(). $file;
    }
}