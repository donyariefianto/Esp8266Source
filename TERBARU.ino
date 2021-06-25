
char ssid[]             = "Sunn";            // your network SSID (name)
char pass[]             = "malaysia";        // your network password
 
char server[]           = "api.thingspeak.com";
byte port               = 80;
char APIKey[]           = "JKRP92NZ4GCK7C93";
uint32_t periodeKirim   = 20000;
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

static const int RXPin = 3, TXPin = 4;
static const uint32_t GPSBaud = 9600;

// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);


 
#include <WiFiEsp.h>
#include <SoftwareSerial.h>
 
SoftwareSerial SerialEsp(11, 10);
WiFiEspClient client;
int status = WL_IDLE_STATUS;     // the Wifi radio's status
uint32_t millisKirim;
bool statusKirim;


void setup()
{
  ss.begin(9600);
  Serial.begin(9600);
  Serial.println("Mengirim data melalui jaringan Wifi menggunakan ESP01 dengan metode POST");
  Serial.println();
 
  SerialEsp.begin(115200);
  SerialEsp.println("AT+UART_DEF=115200,8,1,0,0");
  delay(500);
  SerialEsp.begin(9600);
 
  WiFi.init(&SerialEsp);
 
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
  }
  millisKirim = millis();
}
 
void loop()
{
  if (millisKirim < millis())
  {
    millisKirim = millis() + periodeKirim;
     
    // hubungkan ke jaringan wifi jika belum tersambung atau terputus (auto reconnect)
    if (WiFi.status() != WL_CONNECTED)
    {
      Serial.print("Menghubungkan ke jaringan SSID: ");
      Serial.println(ssid);
      while (WiFi.status() != WL_CONNECTED)
      {
        WiFi.begin(ssid, pass);
        Serial.print(".");
        delay(5000);
      }
      printWifiStatus();
      Serial.println("Berhasil terhubung ke jaringan");
    }
 
 
    if (WiFi.status() == WL_CONNECTED)
    {
      Serial.println();
      Serial.println("Menghubungkan dengan server...");
      if (client.connect(server, port))
      {
        Serial.println();
        Serial.println("Terhubung dengan server.");
        double a=-7.8973738;
 
//        char content[30];
        String sendData ="&field1=" + String(a , 6) + "&field2=" + String(a , 6)  +"&field3=" + String(a , 6) + "\r\n\r\n";
//        sprintf(content, "&field1=" + String(a) + "\r\n\r\n");
 
        client.println("POST /update HTTP/1.1");
        client.println("Host: api.thingspeak.com");
        client.println("User-Agent: tslib-arduino/1.5.0");
        client.print("X-THINGSPEAKAPIKEY: ");
        client.println(APIKey);
        client.println("Content-Type: application/x-www-form-urlencoded");
        client.print("Content-Length: ");
        client.println((sendData.length()));
        client.println("Connection: close");
        client.println();
 
        client.print(sendData);
 
        statusKirim = true;
      }
    }
  }
 
  if (statusKirim)
  {
    while (client.available())
    {
      char c = client.read();
      Serial.write(c);
    }
 
    if (!client.connected()) 
    {
      Serial.println();
      Serial.println("Memutuskan hubungan dengan server...");
      delay(10);
      client.stop();
 
      statusKirim = false;
    }
  }
}
 
 
void printWifiStatus()
{
  // print the SSID of the network you're attached to
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
 
  // print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
 
  // print the received signal strength
  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
