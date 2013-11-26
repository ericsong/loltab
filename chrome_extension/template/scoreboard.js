document.getElementById("update_button").onclick = updateScoreboard;

function updateScoreboard(streamer){
	var xhr = new XMLHttpRequest();
	xhr.onreadystatechange = function(data){
	if(xhr.readyState == 4 && xhr.status == 200){
			var parsed_data = JSON.parse(data.currentTarget.responseText);
			var scoreboardData = parsed_data.data;
			var players = scoreboardData.data.players;
			for(var i = 1; i <= players.length; i++){
				var player = players[i-1];

				var playerid = "player" + i;
				var player_strip = document.getElementById(playerid);
				var player_icon = player_strip.getElementsByClassName('player_champ_icon')[0];
				var player_level = player_strip.getElementsByClassName('player_level')[0];
				
				var player_names = player_strip.getElementsByClassName('player_names')[0];
				var player_username = player_names.getElementsByClassName('username')[0];
				var player_champion = player_names.getElementsByClassName('champion')[0];
			
				var player_summoners = player_strip.getElementsByClassName('player_summoners')[0];
				var player_summoner1 = player_summoners.getElementsByClassName('summoner1')[0];
				var player_summoner2 = player_summoners.getElementsByClassName('summoner2')[0];
	
				var player_item_container = player_strip.getElementsByClassName('player_items')[0];	
				var player_items = [];
				for(var j = 1; j <= 7; j++){
					var classname = 'item' + j;
					player_items.push(player_item_container.getElementsByClassName(classname)[0]);
				}

				var player_kda = player_strip.getElementsByClassName('player_kda')[0];
				var player_cs = player_strip.getElementsByClassName('player_cs')[0];

				/*	
				//testing element retrievals
				player_icon.src = '/home/reggi/Downloads/champions/Riven.png';
				player_level.innerText = '18';
				player_username.innerText = 'scarra';
				player_champion.innerText = 'Riven';
				player_summoner1.src = '/home/reggi/Downloads/summoners/Teleport.png';
				player_summoner2.src = '/home/reggi/Downloads/summoners/Ghost.png';
				player_items[0].src = "/home/reggi/Downloads/items/Zhonya's Hourglass.png";
				player_kda.innerText = "10/40/19";
				player_cs.innerText = "100";
				*/

				player_icon.src = '/home/reggi/Downloads/champions/' + player.champion + '.png';
				player_level.innerText = player.level;
				player_username.innerText = player.name;
				player_champion.innerText = player.champion;
				player_summoner1.src = '/home/reggi/Downloads/summoners/' +  player.spell1 + '.png';
				player_summoner2.src = '/home/reggi/Downloads/summoners/' + player.spell2 + '.png';
				
				for(var j = 1; j <= 7; j++){
					if(player.items[j-1] == 'n/a')
						player.items[j-1] = 'blank_item';

					player_items[j-1].src = "/home/reggi/Downloads/items/" + player.items[j-1] + ".png";
				}

				player_kda.innerText = player.kills + "/" + player.deaths + "/" + player.assists;
				player_cs.innerText = player.cs;

			}	
		}
	}

	xhr.open("GET", "http://localhost:8000/getScoreboardData?streamer=Scarra", true);
	xhr.send();
}
