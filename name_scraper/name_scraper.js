var unirest = require('unirest');

var Request = unirest.get("https://teemojson.p.mashape.com/datadragon/champion")
  .headers({ 
	      "X-Mashape-Authorization": "pEfuSN5Jhqk4wttRdWKVIDoBoMhyQ4u0"
	    })
  .end(function (response) {
	      var data = response.body.data;
	      
	      for(var champ in data){
	      	console.log(data[i].id);
	      }
  });


var Request = unirest.get("https://teemojson.p.mashape.com/player/na/FapSac/ingame")
	.headers({ 
		"X-Mashape-Authorization": "pEfuSN5Jhqk4wttRdWKVIDoBoMhyQ4u0"
	}).end(function (response) {
	      var teamOne = response.body.data.game.teamOne.array;
	      var teamTwo = response.body.data.game.teamTwo;

	      var teamPicks = response.body.data.game.playerChampionSelections.array;


	      for(var i = 0; i < teamOne.length; i++){
		  var ign = teamOne[i].summonerName;
		  var champ;

		  for(var x = 0; x < teamPicks.length; x++){
		  	if(teamPicks[x].summonerInternalName.toUpperCase() == ign.toUpperCase())
				champ = teamPicks[x].championId;
		  }

	          console.log(ign);
	          console.log(champ);
	      }

	});
