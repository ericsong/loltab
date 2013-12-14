console.log('js loaded');

$('#addStreamer_button').click(function(){
	var streamer = $('#addStreamer_input').val();

	$.post('/addStreamer', { streamer: streamer });
});


$('#removeStreamer_button').click(function(){
	var streamer = $('#removeStreamer_input').val();

	$.post('/removeStreamer', { streamer: streamer });
});
