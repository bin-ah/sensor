const temperatureElement = document.getElementById("temp");
const humidityElement = document.getElementById("hum");
const updatedElement = document.getElementById("time");

function formatNumber(value, digits = 1) {
	const numericValue = Number(value);
	return Number.isFinite(numericValue) ? numericValue.toFixed(digits) : "--.-";
}

async function fetchData() {
	try {
		const response = await fetch("/data");

		if (!response.ok) {
			throw new Error(`Request failed with status ${response.status}`);
		}

		const data = await response.json();
		const temperature = Number(data.temp ?? data.temperature ?? 0);
		const humidity = Number(data.hum ?? data.humidity ?? 0);

		temperatureElement.textContent = `${formatNumber(temperature)} °C`;
		humidityElement.textContent = `${formatNumber(humidity)} %`;
		updatedElement.textContent = data.time
			? `Updated: ${data.time}`
			: "Auto refresh every 5 seconds";
	} catch (error) {
		updatedElement.textContent = "Waiting for sensor data...";
		console.error("Unable to load climate data:", error);
	}
}

fetchData();
setInterval(fetchData, 5000);
