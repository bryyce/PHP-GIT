$(document).ready(function(){

	function _ajax_request(url, data, callback, type, method) {
		if (callback == undefined) {
			callback = success;
		}
		if (jQuery.isFunction(data)) {
			callback = data;
			data = {};
		}
		return jQuery.ajax({
			type: 		method,
			url: 		url,
			data: 		data,
			success: 	callback,
			complete: 	complete,
			error: 		error,
			dataType: 	type
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
	function success (result) {
		console.log('success');
	}

	function complete (result) {
		if (result.status >= 300 && result.status < 400)
			window.location.href = result.responseText;
	}

	function error (result) {
		console.log('error');
	}

	$(".rest").restfulizer();
	$('form').on('submit',function (e){
		if($(this).find('input[name="_method"]').val() == 'DELETE'){
			e.preventDefault();
			$.delete($(this).attr('action'), {},function(result){});
			return false;
		} if($(this).find('input[name="_method"]').val() == 'PUT'){
			e.preventDefault();
			$.put($(this).attr('action'), {},function(result){});
			return false;
		}
	});
});