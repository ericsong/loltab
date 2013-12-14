var path = require('path');
var http = require('http');
var express = require('express');
var fs = require('fs');
var spawn = require('child_process').spawn;
var dgram = require('dgram');
var app = express();
var server = http.createServer(app);
var ErrorHandlr = require('errorhandlr').engage()

app.configure(function(){
	app.set('port', process.env.PORT || 8000);
	app.set('address', process.env.ADDRESS || 'localhost');
	app.set('views', __dirname + '/views');
	app.set('view engine', 'jade');
	app.use(express.bodyParser());
	app.use(express.static(path.join(__dirname, '/public')));
	app.use(ErrorHandlr.express());
});

app.get('/', function(req, res){
	res.render('home.jade');
});

app.post('/addStreamer', function(req, res){
	var streamer = req.body.streamer + '\n';
	var append_string = req.body.streamer + " ";	
	console.log("Adding " + streamer);

	//spawn streamer process
	var process = spawn('nc', ['-l', '127.0.0.1', '8001']);
	console.log("Started process... " + process.pid);
	append_string += process.pid.toString();
	append_string += '\n'; 

	//add to streamer list
	fs.appendFile('streamer_list', append_string, function(err){
		if(err)
			console.log(err);
	});	



	res.end();
});

app.post('/removeStreamer', function(req, res){
	var streamer = req.body.streamer + '\n';
	console.log("Removing " + streamer);

	fs.readFile('streamer_list', 'utf8', function(err, data){
		var pid;	
		lines = data.split('\n');
	
		console.log(lines);
	
		var removeIndex;	
		for(var i = 0; i < lines.length; i++){
			var s1 = lines[i].split(' ')[0];
			pid = lines[i].split(' ')[1];
			if(s1 === streamer.substring(0, streamer.length-1)){
				removeIndex = i;
			}
		}

		if(removeIndex > -1){
			lines.splice(removeIndex, 1);
		}else{
			console.log("streamer to be removed not found.");
		}

		var outputstring = "";
		for(var i = 0; i < lines.length-1; i++){
			outputstring = outputstring + lines[i] + '\n';
		}

		spawn('kill', [pid.toString()]);
		fs.writeFile('streamer_list', outputstring, function(err){
			if(err)
				console.log(err);

			res.end();
		});
	});	
});

var s = dgram.createSocket('udp4');
s.bind('1234');

s.on('message', function(msg, rinfo){
	console.log("server got: " + msg + " from " + rinfo.address + ":" + rinfo.port);
});

server.listen(app.get('port'), function(){
	console.log("Listening on port " + app.get('port'));
});
