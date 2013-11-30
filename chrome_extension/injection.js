if(document.title.indexOf("Twitch") != -1){
	console.log("injecting...");
	var bookmark = document.getElementById("channel_actions");
	var btn = document.createElement("a");
	btn.className = btn.className + " normal_button action";
	  
	var btn_span = document.createElement("span");
	btn_span.innerText = "Scoreboard";
	btn.appendChild(btn_span);
	  
	bookmark.appendChild(btn);

	var scoreboard = createTemplate()[0];
	$(scoreboard).hide();	
	$('#stats_and_actions').append(scoreboard);

	$(btn_span).click(function(){
		updateScoreboard();
		$(scoreboard).slideToggle();
	});
}

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

				player_icon.src = chrome.extension.getURL('champions/' + player.champion + '.png');
				player_level.innerText = player.level;
				player_username.innerText = player.name;
				player_champion.innerText = player.champion;
		
				player_summoner1.src = chrome.extension.getURL('summoners/' +  player.spell1 + '.png');
				player_summoner2.src = chrome.extension.getURL('summoners/' + player.spell2 + '.png');
				
				for(var j = 1; j <= 7; j++){
					if(player.items[j-1] == 'n/a')
						player.items[j-1] = 'blank_item';

					player_items[j-1].src = chrome.extension.getURL('items/' + player.items[j-1] + ".png");
				}

				player_kda.innerText = player.kills + "/" + player.deaths + "/" + player.assists;
				player_cs.innerText = player.cs;

			}	
		}
	}

	xhr.open("GET", "http://162.243.95.66/getScoreboardData?streamer=Scarra", true);
	xhr.send();
}

function createTemplate(){
	var base = $('<div></div>');
	base.addClass('template');
		var template_img = $('<img></img>');
		base.append(template_img);
		template_img.attr('src', chrome.extension.getURL('scoreboard_template.jpg'));

		for(var i = 1; i <= 10; i++){
			var player_strip = createPlayerStrip(i);
			base.append(player_strip);
		}	

	console.log('template created');	
	return base;
}

function createPlayerStrip(num){
	var idname = 'player' + num;

	var player_strip = $('<div></div>');
	player_strip.attr('id',	idname);
	player_strip.addClass('player_strip');
		var player_champ_icon = $('<img></img>');
		player_strip.append(player_champ_icon);
		player_champ_icon.addClass('player_champ_icon');
		player_champ_icon.attr('src', chrome.extension.getURL('champions/Lux.png'));
		var player_level = $('<div></div>');
		player_strip.append(player_level);
		player_level.addClass('player_level');
		player_level.text('10');
		var player_names = $('<div></div>');
		player_strip.append(player_names);
		player_names.addClass('player_names');
			var player_username = $('<p></p>');
			player_names.append(player_username);
			player_username.addClass('username');
			player_username.text('regonics');
			var player_champion = $('<p></p>');
			player_names.append(player_champion);
			player_champion.addClass('champion');
			player_champion.text('Lux');
		var player_summoners = $('<div></div>');
		player_strip.append(player_summoners);
		player_summoners.addClass('player_summoners');
			var summoner1 = $('<img></img>');
			player_summoners.append(summoner1);
			summoner1.addClass('summoner1');
			summoner1.attr('src', chrome.extension.getURL('summoners/Revive.png'));
			var summoner2 = $('<img></img>');
			player_summoners.append(summoner2);
			summoner2.addClass('summoner2');
			summoner2.attr('src', chrome.extension.getURL('summoners/Revive.png'));
		var player_items = $('<div></div>');
		player_strip.append(player_items);
		player_items.addClass('player_items');
			var item1 = $('<img></img>');
			player_items.append(item1);
			item1.addClass('item1');
			item1.attr('src', chrome.extension.getURL("items/Rabadon's Deathcap.png"));
			var item2 = $('<img></img>');
			player_items.append(item2);
			item2.addClass('item2');
			item2.attr('src', chrome.extension.getURL("items/Rabadon's Deathcap.png"));
			var item3 = $('<img></img>');
			player_items.append(item3);
			item3.addClass('item3');
			item3.attr('src', chrome.extension.getURL("items/Rabadon's Deathcap.png"));
			var item4 = $('<img></img>');
			player_items.append(item4);
			item4.addClass('item4');
			item4.attr('src', chrome.extension.getURL("items/Rabadon's Deathcap.png"));
			var item5 = $('<img></img>');
			player_items.append(item5);
			item5.addClass('item5');
			item5.attr('src', chrome.extension.getURL("items/Rabadon's Deathcap.png"));
			var item6 = $('<img></img>');
			player_items.append(item6);
			item6.addClass('item6');
			item6.attr('src', chrome.extension.getURL("items/Rabadon's Deathcap.png"));
			var item7 = $('<img></img>');
			player_items.append(item7);
			item7.addClass('item7');
			item7.attr('src', chrome.extension.getURL("items/Rabadon's Deathcap.png"));
		var player_kda = $('<div></div');
		player_strip.append(player_kda);
		player_kda.addClass('player_kda');
		player_kda.text('20/0/10');
		var player_cs =  $('<div></div');
		player_strip.append(player_cs);
		player_cs.addClass('player_cs');
		player_cs.text('314');

	return player_strip;
}
