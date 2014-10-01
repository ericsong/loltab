// node packages
var express     = require('express'),
    morgan      = require('morgan'),
    bodyParser  = require('body-parser'),
    methodOverride = require('method-override'),
    app = express();

// globals
var LOLTAB_APP = {};
LOLTAB_APP.streamers = {};

app.use(express.static(__dirname + '/public'));
app.use(morgan('dev'));
app.use(bodyParser.urlencoded({ extended: false}));
app.use(methodOverride());

app.get('/', function(req, res){
  console.log('hi');
});

app.get('/getCurrentScoreboard', function(req, res){
  res.json({url: getCurrentScoreboard(req.query.streamer)});
});

app.listen(8000);
console.log("Listening on port 8000");

var getCurrentScoreboard = function(streamer) {
  return LOLTAB_APP.streamers[streamer]['url'];
};

var addStreamer = function(streamer) {
  LOLTAB_APP.streamers[streamer] = {};
};

var updateScoreboard = function(streamer, scoreboard_url) {
  LOLTAB_APP.streamers[streamer].url = scoreboard_url;
}
