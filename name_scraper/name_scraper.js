var unirest = require('unirest');

var Request = unirest.get("https://teemojson.p.mashape.com/datadragon/champion")
  .headers({ 
	      "X-Mashape-Authorization": "pEfuSN5Jhqk4wttRdWKVIDoBoMhyQ4u0"
	    })
  .end(function (response) {
		var champs = response.body.data;

		unirest.get("https://teemojson.p.mashape.com/player/na/Causing/ingame")
		.headers({ 
			"X-Mashape-Authorization": "pEfuSN5Jhqk4wttRdWKVIDoBoMhyQ4u0"
		}).end(function (response) {
		      var teamOne = response.body.data.game.teamOne.array;
		      var teamTwo = response.body.data.game.teamTwo.array;

		      var teamPicks = response.body.data.game.playerChampionSelections.array;

			console.log("**** Team One picks****");

		      for(var i = 0; i < teamOne.length; i++){
			  var ign = teamOne[i].summonerName;
			  var champ;

			  for(var x = 0; x < teamPicks.length; x++){
				if(teamPicks[x].summonerInternalName.toUpperCase() == ign.toUpperCase()){
					var t1 = parseInt(teamPicks[x].championId);
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
		      }

			console.log("**** Team Two picks****");

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
		      }


		});

});


