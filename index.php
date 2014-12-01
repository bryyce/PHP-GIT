
<script src="//ajax.googleapis.com/ajax/libs/jquery/2.1.1/jquery.min.js"></script>

<script type="text/javascript">
	/* Extend jQuery with functions for PUT and DELETE requests. */
$(document).ready(function(){

	function _ajax_request(url, data, callback, type, method) {
	    if (jQuery.isFunction(data)) {
	        callback = data;
	        data = {};
	    }
	    return jQuery.ajax({
	        type: method,
	        url: url,
	        data: data,
	        success: callback,
	        dataType: type
	        });
	}

	jQuery.extend({
	    put: function(url, data, callback, type) {
	        return _ajax_request(url, data, callback, type, 'PUT');
	    },
	    delete: function(url, data, callback, type) {
	        return _ajax_request(url, data, callback, type, 'DELETE');
	    }
	});

	$('form').on('submit',function (){
		if($(this).find('input[name="_method"]').val() == 'delete'){
			$.delete($(this).action, {},function(result){});
		} if($(this).find('input[name="_method"]').val() == 'update'){

		}
		return false;
	});
});
</script>
<form method="post" action="/users/delete/1" class="button_to">
	<div>
		<input type="hidden" name="_method" value="delete" />
		<input data-confirm='Are you sure?' value="Delete Image" type="submit" />
	</div>
</form>

<?php
/**
 * index.php file
 * @filesource ./index.php
 * @package		\
 */

var_dump( $result = \Tool\Router::get()->getCurrentRoute(\Tool\HTTPRequest::requestURI(),\Tool\HTTPRequest::method()));
$class = "\\Controller\\".ucfirst($result['controller'])."Controller";
$controller = new $class;
$controller->setAction($result['action']);
$controller->run($result['params']);
echo round(memory_get_usage()/(1024),2)." Ko";
?>
