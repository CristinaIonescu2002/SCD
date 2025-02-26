const mongoose = require('mongoose');

// Tari
const countrySchema = new mongoose.Schema({
    nume_tara: {
        type: String,
        unique: true,
        required: true
    },
    latitudine: { 
        type: Number, 
        required: true 
    },
    longitudine: { 
        type: Number, 
        required: true 
    }
});

const Country = mongoose.model('Country', countrySchema);

// Orase
const citySchema = new mongoose.Schema({
    id_tara: { 
        type: mongoose.Schema.Types.ObjectId, 
        ref: 'Country', 
        required: true 
    },
    nume_oras: { 
        type: String, 
        required: true 
    },
    latitudine: { 
        type: Number, 
        required: true 
    },
    longitudine: { 
        type: Number, 
        required: true 
    }
}, { unique: [['id_tara', 'nume_oras']] });

const City = mongoose.model('City', citySchema);

// Temperaturi
const temperatureSchema = new mongoose.Schema({
    id_oras: { 
        type: mongoose.Schema.Types.ObjectId, 
        ref: 'City', 
        required: true 
    },
    valoare: { 
        type: Number, 
        required: true 
    },
    timestamp: { 
        type: Date, 
        default: Date.now 
    }
}, { unique: [['id_oras', 'timestamp']] });

const Temperature = mongoose.model('Temperature', temperatureSchema);

module.exports = { Country, City, Temperature };
