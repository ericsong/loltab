if(document.title.indexOf("Twitch") != -1){
	console.log("injecting...");
	var bookmark = document.getElementById("channel_actions");
	var btn = document.createElement("a");
	btn.className = btn.className + " normal_button action";
	  
	var btn_span = document.createElement("span");
	btn_span.innerText = "Scoreboard";
	btn.appendChild(btn_span);
	  
	bookmark.appendChild(btn);

	var xhr = new XMLHttpRequest();
	xhr.onreadystatechange = function(data){
		if(xhr.readyState == 4 && xhr.status == 200){
			console.log(data.currentTarget.responseText);
		}
	}
	xhr.open("GET", "http://localhost:8000/getScoreboardData?streamer=Scarra", true);
	xhr.send();
}
