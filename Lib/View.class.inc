<?php
namespace Lib;
class View {
	private $_file_prefix;
	private $_view_vars;
	private $_layout = 'layout.phtml';


	public function setDefaultView($file_prefix) {
		$this->_file_prefix = $file_prefix;
	}

	public function setVars($view_vars = []) {
		$this->_view_vars = $view_vars;
	}

	public function render($file = NULL, $variables = array()) {
		require 'Helper/default.php';
        if (isset($variables['vars'])) {
        	extract($variables['vars']);
        } else {
        	extract($this->_view_vars);
        }

        // ob_start('ob_gzhandler');
        $this->_content = getViewsBasePath() . (is_null($file) ? $this->_file_prefix : $file);
        if (!isset($variables['format'])) {
        	$this->_content .= '.phtml';
        }
        $layout = 'layout/';
        if (!isset($variables['layout']) && !isset($variables['format'])) {
        	$layout .= $this->_layout;
        }
        $view = $this;
        include getViewsBasePath(). $layout;
        $renderedView = ob_get_clean();
        // ob_end_flush();
        echo $renderedView;
    }
}