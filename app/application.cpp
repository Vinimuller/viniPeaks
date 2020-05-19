#include <SmingCore.h>
#include <Libraries/DHTesp/DHTesp.h>

#define WORK_PIN 		14
#define READ_INTERVAL 	10 * 60 * 1000

HttpClient thingSpeak;
DHTesp dht;

Timer readTemperatureProcTimer;
void onTimer_readTemperatures();
int onHttpDataSent(HttpConnection& client, bool successful);
void sendData(float temperature, float humidity);

// Will be called when WiFi station timeout was reached
void connectFail(const String& ssid, MacAddress bssid, WifiDisconnectReason reason)
{
	Serial.println("Connect Fail");
}

void gotIP(IpAddress ip, IpAddress netmask, IpAddress gateway)
{
	Serial.println("got IP. Starting timer");
	// Start send data loop
	readTemperatureProcTimer.initializeMs(READ_INTERVAL, onTimer_readTemperatures).start();
}

void init()
{
	Serial.begin(SERIAL_BAUD_RATE);
	Serial.systemDebugOutput(true); // Allow debug output to serial

	dht.setup(WORK_PIN, DHTesp::DHT22);

	// //Start Access Point
	// WifiAccessPoint.enable(true);
	// WifiAccessPoint.config("Fish Peaks", "", AUTH_OPEN);
	// WifiAccessPoint.setIP(IPAddress(192,168,45,1));

	// Run our method when station was connected to AP (or not connected)
	WifiEvents.onStationDisconnect(connectFail);
	WifiEvents.onStationGotIP(gotIP);

	WifiStation.enable(true);
	WifiStation.config("Salame2G", "Um2tres4cinco");
}

void onTimer_readTemperatures()
{
	float humidity 		= 0;
	float temperature 	= 0;

	TempAndHumidity th;
	th 			= dht.getTempAndHumidity();
	humidity 	= th.humidity;
	temperature = th.temperature;

	if(dht.getStatus() == DHTesp::ERROR_NONE) 
	{//Read Ok

		sendData(temperature, humidity);

		Serial.print("\tHumidity: ");
		Serial.print(th.humidity);
		Serial.print("% Temperature: ");
		Serial.print(th.temperature);
		Serial.print(" *C\n");
		//  Other goodies:
		//
		//  Heatindex is the percieved temperature taking humidity into account
		//  More: https://en.wikipedia.org/wiki/Heat_index
		//
		Serial.print("Heatindex: ");
		Serial.print(dht.computeHeatIndex(temperature, humidity));
		Serial.print("*C\n");

		//
		//  Dewpoint is the temperature where condensation starts.
		//  Water vapors will start condensing on an object having this temperature or below.
		//  More: https://en.wikipedia.org/wiki/Dew_point
		//
		Serial.printf("Dewpoint: ");
		Serial.print(dht.computeDewPoint(temperature, humidity));
		Serial.print("*C\n");

		//
		// Determine thermal comfort according to http://epb.apogee.net/res/refcomf.asp
		//
		ComfortState cf;

		Serial.print("Comfort is at ");
		Serial.print(dht.getComfortRatio(cf, temperature, humidity));
		Serial.print("\n\n");
	} else 
	{//Error
		Serial.print("Failed to read from DHT: ");
		Serial.print(dht.getStatus());
	}
}

void sendData(float temperature, float humidity)
{
	Serial.println("\n=== SEND SENSOR DATA ===");

	ComfortState cf;

	Url url;
	url.Scheme 			= URI_SCHEME_HTTP;
	url.Host 			= "api.thingspeak.com";
	url.Path 			= "/update";
	url.Query["key"] 	= "2PZCZLNKD7SQN4JW";
	url.Query["field1"] = String(temperature);
	url.Query["field2"] = String(humidity);
	url.Query["field3"] = String(dht.computeHeatIndex(temperature, humidity));
	url.Query["field4"] = String(dht.computeDewPoint(temperature, humidity));
	url.Query["field5"] = String(dht.getComfortRatio(cf, temperature, humidity));

	thingSpeak.downloadString(url, onHttpDataSent);
}

int onHttpDataSent(HttpConnection& client, bool successful)
{
	if(successful)
		Serial.println("Success sent");
	else
		Serial.println("Failed");

	String response = client.getResponse()->getBody();
	Serial.println("Server response: '" + response + "'");
	if(response.length() > 0) {
		int intVal = response.toInt();

		if(intVal == 0)
			Serial.println("Sensor value wasn't accepted. May be we need to wait a little?");
	}

	return 0;
}