var mongoose = require('mongoose');

var StreamerSchema = mongoose.Schema({
	id: String,
	names: []
});

mongoose.connect("mongodb://localhost/loltabber");

var db = mongoose.connection;
db.on('error', console.error.bind(console, 'connection error:'));
db.once('open', function(){
	console.log('database connected');
});

var StreamerModel = mongoose.model('Streamer', StreamerSchema);

var command = process.argv[2];
var streamer = process.argv[3];
var name = process.argv[4];

//read
//add name
//remove name

if(command == "read"){
	StreamerModel.find({id: streamer}, function(err, query){
		console.log(query[0].names);
	});
}

if(command == "add"){
	StreamerModel.find({id: streamer}, function(err, query){
		var new_streamer;
		if(query.length == 0){
			new_streamer = new StreamerModel;
			new_streamer.id = streamer;
		}else{
			new_streamer = query[0];
		}

		new_streamer.names.push(name);
		new_streamer.save();
	});
}

if(command == "remove"){

}
