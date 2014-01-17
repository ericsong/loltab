var unirest = require('unirest');
var async = require('async');
var fs = require('fs');
var mongoose = require('mongoose');

var outfile = "metadata.txt";
var player;

var output_string = "";
var metadata = {
	streamer: "",
	ign: "",
	ingame: true,
	picks: []
};

var StreamerSchema = mongoose.Schema({
	id: String,
	names: []
});

mongoose.connect("mongodb://localhost/loltabber");

var db = mongoose.connection;
db.on('error', console.error.bind(console, "connection error:"));
db.once('open', function(){
	console.log("database connected");
});

var StreamerModel = mongoose.model('Streamer', StreamerSchema);

function getPicks(player, callback){
	var status;
	var Request = unirest.get("https://teemojson.p.mashape.com/datadragon/champion")
	  .headers({ 
		      "X-Mashape-Authorization": "pEfuSN5Jhqk4wttRdWKVIDoBoMhyQ4u0"
		    })
	  .end(function (response) {
			var champs = response.body.data;

			unirest.get("https://teemojson.p.mashape.com/player/na/" + player + "/ingame")
			.headers({ 
				"X-Mashape-Authorization": "pEfuSN5Jhqk4wttRdWKVIDoBoMhyQ4u0"
			}).end(function (response) {
				if(response.status != 200){
					status = 0;
					callback(0, 0);
					return;
				}

				metadata.ign = player;

			      var teamOne = response.body.data.game.teamOne.array;
			      var teamTwo = response.body.data.game.teamTwo.array;

			      var teamPicks = response.body.data.game.playerChampionSelections.array;

				console.log("***Team One picks***");
				output_string += "***Team One picks***\n";

			      for(var i = 0; i < teamOne.length; i++){
				  var ign = teamOne[i].summonerName;
			 	  var key;
				  var champ;

				  for(var x = 0; x < teamPicks.length; x++){
					if(teamPicks[x].summonerInternalName.toUpperCase().replace(/\s/g,"") == ign.toUpperCase().replace(/\s/g, "")){
						var t1 = parseInt(teamPicks[x].championId);
						key = teamPicks[x].championId;
						for(var champ in champs){
							var t2 = parseInt(champs[champ].key);
							if(t1 == t2){
								var t2 = parseInt(teamPicks[x].championId);
								champ = champs[champ].id;
								break;
							}
						}	
					}
				  }

				  console.log(ign + ": " + champ);
				  output_string += ign + ": " + champ + '\n';
				  metadata.picks.push({name: ign, champion: champ});
			      }

				console.log("***Team Two picks***");
				output_string += "***Team Two picks***" + '\n';

			      for(var i = 0; i < teamTwo.length; i++){
				  var ign = teamTwo[i].summonerName;
				  var key;
				  var champ;

				  for(var x = 0; x < teamPicks.length; x++){
					if(teamPicks[x].summonerInternalName.toUpperCase().replace(/\s/g,"") == ign.toUpperCase().replace(/\s/g,"")){
						var t1 = parseInt(teamPicks[x].championId);
						key = teamPicks[x].championId;
						for(var champ in champs){
							var t2 = parseInt(champs[champ].key);
							if(t1 == t2){
								var t2 = parseInt(teamPicks[x].championId);
								champ = champs[champ].id;
								break;
							}
						}	
					}
				  }

				  console.log(ign + ": " + champ);
				  output_string += ign + ": " + champ + '\n';
				  metadata.picks.push({name: ign, champion: champ});
			      }
				
				status = 1;	
				callback(0, 1);
				return;
			});

	});

	return status;
}

//get streamer name
fs.readFile('metadata.txt', 'utf8', function(err, data){
	var olddata = JSON.parse(data);
	console.log(olddata);

	player = olddata.streamer;
	metadata.streamer = player;

	StreamerModel.find({id: player}, function(err, query){
		var names = query[0].names;
		var functions = [];
		for(var i = 0; i < names.length; i++){
			(function(i){
				var newfunc = function(callback){
					var status = getPicks(names[i], callback);
				};
		
				functions.push(newfunc);
			})(i);
		}

		async.parallel(functions, function(err, results){
			var count = 0;
			var found = false;

			for(var i = 0; i < results.length; i++){
				if(results[i] == 1){
					found = true;
					count++;
				}			
			}

			//add notification for count != 1 or 0. 

			if(found){
				console.log("game found");
				fs.writeFile(outfile, JSON.stringify(metadata));
			}else{
				console.log("game not found. notifying eric/tim");
			}
		});
	});
});

