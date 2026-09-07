const express = require('express');
const cors = require('cors');
const path = require('path');

const app = express();
const FRONTEND_PATH = path.join(__dirname, '../front');
const PORT = 3000;

app.use(express.json());
app.use(cors());

function normalizeSensorData(body = {}) {
  const temp = Number(body.temp ?? body.temperature ?? 0);
  const hum = Number(body.hum ?? body.humidity ?? 0);

  return {
    temp: Number.isFinite(temp) ? temp : 0,
    hum: Number.isFinite(hum) ? hum : 0,
    time: new Date().toLocaleTimeString()
  };
}

// lưu data tạm
let sensorData = {
  temp: 0,
  hum: 0,
  time: ""
};

// API nhận data từ ESP32
app.post('/data', (req, res) => {
  sensorData = normalizeSensorData(req.body);

  console.log("Received:", sensorData);
  res.send("OK");
});

// API trả data cho web
app.get('/data', (req, res) => {
  res.json(sensorData);
});

// serve web
app.use(express.static(FRONTEND_PATH));
app.get('/', (req, res) => {
  res.sendFile(path.join(FRONTEND_PATH, 'index.html'));
});

app.listen(PORT, () => {
  console.log(`Server running: http://localhost:${PORT}`);
});