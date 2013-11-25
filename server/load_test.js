var mongoose = require('mongoose');

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
			muted: Number,
			ally: Boolean
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

var sb1 = new ScoreboardModel;
sb1.streamer = "Scarra";
sb1.data.blueKills = 6;
sb1.data.redKills = 11;

sb1.data.players.push({
	name: "scarra",
	champion: "Riven", 
	level: 10,
	spell1: "Flash",
	spell2: "Ignite", 
	items: ["Boots", "Vampire Scepter", "Sight Ward", "Brutalizer", "n/a", "Doran's Shield", "Sight Trinket"],
	kills: 0,
	deaths: 4,
	assists: 1,
	cs: 111,
	muted: 0,
	ally: true
});

sb1.data.players.push({
	name: "Rekeri",
	champion: "Jinx", 
	level: 10,
	spell1: "Barrier",
	spell2: "Flash", 
	items: ["Berserker Greaves", "Bloodthirster", "n/a", "Health Pots", "Truce" , "Doran's Blade", "Sight Trinket"],
	kills: 3,
	deaths: 1,
	assists: 2,
	cs: 101,
	muted: -1,
	ally: true
});

sb1.data.players.push({
	name: "Kroatik",
	champion: "Shyvana", 
	level: 11,
	spell1: "Smite",
	spell2: "Flash", 
	items: ["Iron Locket of Solari", "n/a", "Mercury Treads", "n/a", "Spirit of the Ancient Goelm" , "n/a", "Sweep Trinket"],
	kills: 2,
	deaths: 0,
	assists: 1,
	cs: 112,
	muted: -1,
	ally: true
});

sb1.data.players.push({
	name: "hi im jaysun",
	champion: "Vayne", 
	level: 10,
	spell1: "Flash",
	spell2: "Barrier", 
	items: ["Blade of the Ruined King", "Sight Ward", "Health Pot", "Berserker Greaves", "Dagger" , "n/a", "Sight Trinket"],
	kills: 0,
	deaths: 3,
	assists: 0,
	cs: 128,
	muted: -1,
	ally: true
});

sb1.data.players.push({
	name: "aGambit",
	champion: "Thresh", 
	level: 9,
	spell1: "Ignite",
	spell2: "Flash", 
	items: ["Frost Support Item", "Mobility Boots", "Sightstone", "Rejuvenation Bead", "Cloth Armour" , "Magic Mettle", "Sweep Trinket"],
	kills: 1,
	deaths: 3,
	assists: 5,
	cs: 13,
	muted: -1,
	ally: true
});

sb1.data.players.push({
	name: "Yusui",
	champion: "Tristana", 
	level: 12,
	spell1: "Ignite",
	spell2: "Flash", 
	items: ["Deathfire Grasp", "Doran's Ring", "Doran's Ring", "Sorcerer's Boots", "Doran's Ring" , "Amplifying Tome", "Sight Trinket"],
	kills: 3,
	deaths: 0,
	assists: 2,
	cs: 143,
	muted: -1,
	ally: false
});

sb1.data.players.push({
	name: "Panchie",
	champion: "Jax", 
	level: 12,
	spell1: "Ignite",
	spell2: "Flash", 
	items: ["Trinity Force", "Doran's Blade", "Doran's Shield", "n/a", "Boots" , "Cloth Armour", "Sight Trinket"],
	kills: 4,
	deaths: 0,
	assists: 0,
	cs: 145,
	muted: -1,
	ally: false
});

sb1.data.players.push({
	name: "Pletkon",
	champion: "Elise", 
	level: 10,
	spell1: "Flash",
	spell2: "Smite", 
	items: ["Haunting Guise", "Sorcerer's Boots", "Amplifying Tome", "Spirit of the Ancient Golem", "n/a" , "n/a", "Sweep Trinket"],
	kills: 2,
	deaths: 0,
	assists: 3,
	cs: 70,
	muted: -1,
	ally: false
});

sb1.data.players.push({
	name: "Damian",
	champion: "Caitlyn", 
	level: 10,
	spell1: "Flash",
	spell2: "Barrier", 
	items: ["Bloodthirster", "Boots", "n/a", "Targons", "Doran's Blade" , "Doran's Blade", "Sight Trinket"],
	kills: 2,
	deaths: 4,
	assists: 0,
	cs: 109,
	muted: -1,
	ally: false
});

sb1.data.players.push({
	name: "wc Hero",
	champion: "Zyra", 
	level: 8,
	spell1: "Ignite",
	spell2: "Flash", 
	items: ["Sorcerer's Boots", "Haunting Guise", "Biscuit", "n/a", "n/a" , "n/a", "Sight Trinket"],
	kills: 0,
	deaths: 2,
	assists: 2,
	cs: 31,
	muted: -1,
	ally: false
});

sb1.save();
