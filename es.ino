# include <SoftwareSerial.h>
#define nama_wifi "Sunn"
#define pass_wifi "malaysia"
#define ip_host "192.168.10.115"  // ip computer kita, bisa liat di CMD (ipconfig)
String GET = "GET http://" + String(ip_host) + "/cupang/datame.php?nilai=";
SoftwareSerial wifi (5, 6); // Rx Tx

boolean connected = false;

void setup() {
  // put your setup code here, to run once:
  wifi.begin(9600);
  Serial.begin(9600);
  wifi.setTimeout(5000);

  Serial.println("ESP8266 tes");

  delay (1000);
  wifi.println("AT+RST");
  delay(1000);

  for (int i = 0; i < 5; i++) {
    connect_to_wifi();
    if (connected) {
      Serial.println(" ESP8266 SIAP ");
      break;
    }
  }
  if (!connected) {
    while (1);
  }
  delay(5000);
  wifi.println("AT+CIPMUX=0");
  delay(1000);
}
void loop()
{
  updateTemp();
}


void connect_to_wifi()
{
  wifi.println("AT+CWMODE=1");
  String cmd = "AT+CWJAP=\"";
  cmd += nama_wifi;
  cmd += "\",\"";
  cmd += pass_wifi;
  cmd += "\"";
  wifi.println(cmd);
  Serial.println(cmd);
  if (wifi.find("OK")) {
    Serial.println("Berhasil Terkoneksi ke internet");
    connected = true;
  } else {
    Serial.println("Gagal Terkoneksi");
    connected = false;
  }

}

void updateTemp() {
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += ip_host;
  cmd += "\",80";
  sendDebug(cmd);
  delay(2000);
  Serial.write(wifi.read());
  if (wifi.find("Error")) {
    Serial.print("RECEIVED: Error");
    return;
  }
  cmd = GET;
  //====================variabel yang dikirim==========
  //  cmd += "data=";
  //  cmd += data;
    cmd += "1&nilai2=";
  cmd += "111";
  cmd += "&nilai3=2";
  //====================variabel yang dikirim==========

  cmd += " HTTP/1.0\r\n\r\n";
  wifi.print("AT+CIPSEND="+ cmd.length());
//  wifi.println(cmd.length());
  if (wifi.find("OK")) {
    Serial.print(">");
    Serial.println(cmd);
    wifi.print(cmd);
  } else {
    sendDebug("AT+CIPCLOSE");
  }
  Serial.println(cmd);
  Serial.write(wifi.read());

  if (wifi.find("OK")) {
    Serial.println("RECEIVED: OK");
    Serial.write(wifi.read());
  } else {
    Serial.println("RECEIVED: Error");
    Serial.println("mencoba terhubung wifi");
    connect_to_wifi();
    delay(1000);
  }

  cek_data();
}


void cek_data() {
  if (wifi.find("koneksi Sukses")) {
    Serial.println("oooook");
  }
}

void sendDebug(String cmd) {
  Serial.print("SEND: ");
  Serial.println(cmd);
  wifi.println(cmd);
}
