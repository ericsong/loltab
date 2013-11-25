var mongoose = require('mongoose');
var path = require('path');
var http = require('http');
var express = require('express');
var app = express();
var server = http.createServer(app);
var ErrorHandlr = require('errorhandlr').engage()

//replace with scoreboard schema
var ScoreboardSchema = mongoose.Schema({
	time: { type: Date, default: Date.now },
	streamer: String,
	data: {
		blueKills: Number,
		redKills: Number,
		players: [{
			name: String,
			champion: String,
			level: Number,
			spell1: String,
			spell2: String,
			items: [String],
			kills: Number,
			deaths: Number,
			assists: Number,
			cs: Number,
			muted: Boolean
		}]
	}
});

mongoose.connect('mongodb://localhost/loltabber');

var db = mongoose.connection;
db.on('error', console.error.bind(console, 'connection error:'));
db.once('open', function(){
	console.log('database connected');
});

//scorboard schema
var ScoreboardModel = mongoose.model('Scoreboard', ScoreboardSchema);

app.configure(function(){
	app.set('port', process.env.PORT || 8000);
	app.set('address', process.env.ADDRESS || 'localhost');
	app.set('views', __dirname + '/views');
	app.set('view engine', 'jade');
	app.use(express.bodyParser());
	app.use(express.static(path.join(__dirname, '/public')));
	app.use(ErrorHandlr.express());
});

app.get('/getScoreboardData', function(req, res){
	res.header('Access-Control-Allow-Origin', "*");
	console.log("request received for: " + req.query.streamer);

	ScoreboardModel.find({streamer: req.query.streamer}, function(err, query){
		res.json({data: query[0]});
	});
});

server.listen(app.get('port'), function(){
	console.log("Listening on port " + app.get('port'));
});
