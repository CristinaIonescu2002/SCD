const mongoose = require("mongoose");
const cors = require("cors");
const express = require("express");
const bodyParser = require("body-parser");
const { Country, City, Temperature } = require('./models');

const port = process.env.PORT || 5000;

const app = express();

app.use(cors());
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: true }));

app.get("/", function (req, res) {
    res.status(200).send("Hello world!");
});

// Tari
app.post('/api/countries', async (req, res) => {
    try {
        const { nume, lat, lon } = req.body;

        if (!nume || !lat || !lon) {
            return res.status(400).send();
        }

        const country = new Country({
            nume_tara: nume,
            latitudine: lat,
            longitudine: lon
        });

        await country.save();
        res.status(201).json({ id: country._id });

    } catch (err) {
        if (err.name === 'CastError') {
            return res.status(404).send();
        }

        res.status(err.code === 11000 ? 409 : 500).json();
    }
});

app.get('/api/countries', async (req, res) => {
    const countries = await Country.find();

    const result = countries.map(country => {
        return {
            id: country._id,
            nume: country.nume_tara,
            lat: country.latitudine,
            lon: country.longitudine
        };
    });

    res.status(200).json(result);
});

app.put('/api/countries/:id', async (req, res) => {
    try {
        const { id, nume, lat, lon } = req.body;
        const idParam = req.params.id;

        if (!id || !idParam || !nume || !lat || !lon || id !== idParam) {
            return res.status(400).send();
        }

        const country = await Country.findById(id);

        if (!country) 
            return res.status(404).send();

        country.nume_tara = nume;
        country.latitudine = lat;
        country.longitudine = lon;

        await country.save();

        res.status(200).send();
    } catch (err) {
        if (err.name === 'CastError') {
            return res.status(404).send();
        }

        res.status(err.code === 11000 ? 409 : 500).json();
    }
});

app.delete('/api/countries/:id', async (req, res) => {

    try {
        const { id } = req.params;
    
        if (!id) {
            return res.status(400).send();
        }
    
        const result = await Country.findById(id);
        
        if (!result) 
            return res.status(404).send();
        
        const cities = await City.find({ id_tara: id });
    
        if (cities != null) {
            const cityIds = cities.map(city => city._id);
    
            await Temperature.deleteMany({ id_oras: { $in: cityIds } });
            await City.deleteMany({ id_tara: id });
        }
    
        await result.deleteOne({ _id: id });
    
        res.status(200).send();
    } catch (err) {
        if (err.name === 'CastError') {
            return res.status(404).send();
        }

        return res.status(500).send();
    }
});

// Orase
app.post('/api/cities', async (req, res) => {
    try {
        const { idTara, nume, lat, lon } = req.body;

        if (!idTara || !nume || !lat || !lon) {
            return res.status(400).send();
        }

        const country = await Country.findById(idTara);

        if (!country) {
            return res.status(404).send();
        }

        const city = new City({
            id_tara: idTara,
            nume_oras: nume,
            latitudine: lat,
            longitudine: lon
        });

        await city.save();
        res.status(201).json({ id: city._id });

    } catch (err) {
        if (err.name === 'CastError') {
            return res.status(404).send();
        }

        res.status(err.code === 11000 ? 409 : 500).json();
    }
});

app.get('/api/cities', async (req, res) => {
    const cities = await City.find();

    const result = cities.map(city => {
        return {
            id: city._id,
            idTara: city.id_tara,
            nume: city.nume_oras,
            lat: city.latitudine,
            lon: city.longitudine
        };
    });

    res.status(200).json(result);
});

app.get('/api/cities/country/', async (req, res) => {
    return res.status(200).send([]);
});

app.get('/api/cities/country/:id', async (req, res) => {
    try {
        const { id } = req.params;

        if (!id) {
            return res.status(200).send([]);
        }
    
        const cities = await City.find({id_tara: id});
    
        const result = cities.map(city => {
            return {
                id: city._id,
                idTara: city.id_tara,
                nume: city.nume_oras,
                lat: city.latitudine,
                lon: city.longitudine
            };
        });
    
        res.status(200).json(result);
    }catch(err) {
        if (err.name === 'CastError') {
            return res.status(200).send([]);
        }

        return res.status(500).send();
    }
});

app.put('/api/cities/:id', async (req, res) => {
    try {
        const { id, idTara, nume, lat, lon } = req.body;
        const idParam = req.params.id;

        if (!id || !idParam || !idTara || !nume || !lat || !lon || id !== idParam) {
            return res.status(400).send();
        }

        const city = await City.findById(id);

        if (!city) 
            return res.status(404).send();

        city.id_tara = idTara;
        city.nume_oras = nume;
        city.latitudine = lat;
        city.longitudine = lon;

        await city.save();

        res.status(200).send();
    } catch (err) {
        if (err.name === 'CastError') {
            return res.status(404).send();
        }

        res.status(err.code === 11000 ? 409 : 500).json();
    }
});

app.delete('/api/cities/:id', async (req, res) => {
    try {
        const { id } = req.params;
    
        if (!id) {
            return res.status(400).send();
        }
    
        const result = await City.findById(id);
        
        if (!result) 
            return res.status(404).send();
        
        await Temperature.deleteMany({ id_oras: id })
        await result.deleteOne({ _id: id });
    
        res.status(200).send();
    } catch (err) {
        if (err.name === 'CastError') {
            return res.status(404).send();
        }

        return res.status(500).send();
    }
});

// Temperaturi
app.post('/api/temperatures', async (req, res) => {
    try {
        const { id_oras, valoare } = req.body;

        if (!id_oras || !valoare ) {
            return res.status(400).send();
        }

        const city = await City.findById(id_oras);

        if (!city) {
            return res.status(404).send();
        }

        const temp = new Temperature(req.body);

        await temp.save();
        res.status(201).json({ id: temp._id });

    } catch (err) {
        if (err.name === 'CastError') {
            return res.status(404).send();
        }

        res.status(err.code === 11000 ? 409 : 500).json();
    }
});

app.get('/api/temperatures', async (req, res) => {
    const { lat, lon, from, until } = req.query;

    const temperature = await Temperature.find().populate({
        path: "id_oras",
        select: "latitudine longitudine"
    }).exec();

    const temp = temperature.filter(t => {
        const goodLat = lat ? t.id_oras.latitudine == lat : true;
        const goodLon = lon ? t.id_oras.longitudine == lon : true;

        const tmp = new Date(t.timestamp);
        const goodFrom = from ? tmp >= new Date(from) : true;
        const goodUntil = until ? tmp <= new Date(until) : true;

        return goodLat && goodLon && goodFrom && goodUntil;
    });

    const result = temp.map(t => {
        return {
            id: t._id,
            valoare: t.valoare,
            timestamp: t.timestamp
        };
    });

    res.status(200).json(result);
});

app.get('/api/temperatures/cities/:id', async (req, res) => {
    try {
        const { id } = req.params;
        const { from, until } = req.query;
    
        const temperature = await Temperature.find();
    
        const temp = temperature.filter(t => {
            const goodCity = t.id_oras == id;
    
            const tmp = new Date(t.timestamp);
            const goodFrom = from ? tmp >= new Date(from) : true;
            const goodUntil = until ? tmp <= new Date(until) : true;
    
            return goodCity && goodFrom && goodUntil;
        });
    
        const result = temp.map(t => {
            return {
                id: t._id,
                valoare: t.valoare,
                timestamp: t.timestamp
            };
        });
    
        res.status(200).json(result);
    } catch (err) {
        if (err.name === 'CastError') {
            return res.status(404).send();
        }

        return res.status(500).send();
    }
});

app.get('/api/temperatures/countries/:id', async (req, res) => {
    try {
        const { id } = req.params;
        const { from, until } = req.query;
    
        const temperature = await Temperature.find().populate({
            path: "id_oras",
            select: "id_tara"
        }).exec();
    
        const temp = temperature.filter(t => {
            const goodCity = t.id_oras.id_tara == id;
    
            const tmp = new Date(t.timestamp);
            const goodFrom = from ? tmp >= new Date(from) : true;
            const goodUntil = until ? tmp <= new Date(until) : true;
    
            return goodCity && goodFrom && goodUntil;
        });
    
        const result = temp.map(t => {
            return {
                id: t._id,
                valoare: t.valoare,
                timestamp: t.timestamp
            };
        });
    
        res.status(200).json(result);
    } catch (err) {
        if (err.name === 'CastError') {
            return res.status(404).send();
        }

        return res.status(500).send();
    }
});

app.put('/api/temperatures/:id', async (req, res) => {
    try {
        const { id, idOras, valoare } = req.body;
        const idParam = req.params.id;

        if (!id || !idParam || !idOras || !valoare || id !== idParam) {
            return res.status(400).send();
        }

        const temp = await Temperature.findById(id);

        if (!temp) 
            return res.status(404).send();

        temp.id_oras = idOras;
        temp.valoare = valoare;

        await temp.save();

        res.status(200).send();

    } catch (err) {
        if (err.name === 'CastError') {
            return res.status(404).send();
        }

        res.status(err.code === 11000 ? 409 : 500).json();
    }
});

app.delete('/api/temperatures/:id', async (req, res) => {
    try {
        const { id } = req.params;
    
        if (!id) {
            return res.status(400).send();
        }
    
        const result = await Temperature.findById(id);
        
        if (!result) 
            return res.status(404).send();
        
        await result.deleteOne({ _id: id });
    
        res.status(200).send();
    } catch (err) {
        if (err.name === 'CastError') {
            return res.status(404).send();
        }

        return res.status(500).send();
    }
});

mongoose.connect(process.env.MONGO_URI, {
    useNewUrlParser: true,
    useUnifiedTopology: true
});

app.listen(port, function () {
    console.log("Server is running...");
});