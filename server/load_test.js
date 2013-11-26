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
	items: ["Boots of Speed", "Vampiric Scepter", "Stealth Ward", "The Brutalizer", "n/a", "Doran's Shield", "Warding Totem (Trinket)"],
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
	items: ["Berserker's Greaves", "The Bloodthirster", "n/a", "Health Potion", "Relic Shield" , "Doran's Blade", "Warding Totem (Trinket)"],
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
	items: ["Locket of the Iron Solari", "n/a", "Mercury's Treads", "n/a", "Spirit of the Ancient Golem" , "n/a", "Sweeping Lens (Trinket)"],
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
	items: ["Blade of the Ruined King", "Stealth Ward", "Health Potion", "Berserker's Greaves", "Dagger" , "n/a", "Warding Totem (Trinket)"],
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
	items: ["Frost Queen's Claim", "Boots of Mobility", "Sightstone", "Rejuvenation Bead", "Cloth Armor" , "Null-Magic Mantle", "Sweeping Lens (Trinket)"],
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
	items: ["Deathfire Grasp", "Doran's Ring", "Doran's Ring", "Sorcerer's Shoes", "Doran's Ring" , "Amplifying Tome", "Warding Totem (Trinket)"],
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
	items: ["Trinity Force", "Doran's Blade", "Doran's Shield", "n/a", "Boots of Speed" , "Cloth Armor", "Warding Totem (Trinket)"],
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
	items: ["Haunting Guise", "Sorcerer's Shoes", "Amplifying Tome", "Spirit of the Ancient Golem", "n/a" , "n/a", "Sweeping Lens (Trinket)"],
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
	items: ["The Bloodthirster", "Boots of Speed", "n/a", "Emblem of Valor", "Doran's Blade" , "Doran's Blade", "Warding Totem (Trinket)"],
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
	items: ["Sorcerer's Shoes", "Haunting Guise", "Total Biscuit of Rejuvenation", "n/a", "n/a" , "n/a", "Warding Totem (Trinket)"],
	kills: 0,
	deaths: 2,
	assists: 2,
	cs: 31,
	muted: -1,
	ally: false
});

sb1.save();
