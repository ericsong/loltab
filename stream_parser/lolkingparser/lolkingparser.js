var fs = require('fs');
var request = require('request');
var cheerio = require('cheerio');

function getNameData(name, region){
	url = "http://www.lolking.net/now/" + region + '/' + name;
	var bluedata = "";
	var purpledata = "";

	request.get(url, 
		function(error, response, body){
			var $ = cheerio.load(body);
			var elements = $('table.blue-team tbody tr td div div a');

			if(elements.length == 0){
				console.log("retrieval failed, retrying...");
				getNameData(url);
			}else{

			for(var i = 0; i < 20; i++){
				bluedata += elements[i].children[0].data + '\n';	
				bluedata += elements[i+1].children[0].data + '\n';
				i = i + 3;
			}
			bluedata = bluedata.substring(0, bluedata.length-1);

			elements = $('table.purple-team tbody tr td div div a');

			for(var i = 0; i < 20; i++){
				purpledata += elements[i].children[0].data + '\n';	
				purpledata += elements[i+1].children[0].data + '\n';
				i = i + 3;
			}
			purpledata = purpledata.substring(0, purpledata.length-1);
			
			if(bluedata.indexOf(name) == -1){
				console.log(purpledata);
				console.log("!!!!!");
				console.log(bluedata);
			}else{
				console.log(bluedata);
				console.log("!!!!!");
				console.log(purpledata);
			}

			}
		}
	);
}

getNameData('cleanmypool', 'na');
