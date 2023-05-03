#include <WiFi.h>
const char* ssid = "NCAIR IOT";
const char* password = "Asim@123Tewari";

const char* server = "www.lab40.in";
const int port = 80;
int i =10;
WiFiClient client;

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);          //The WiFi is in station mode. The other is the softAP mode
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");  Serial.print("WiFi connected to: "); Serial.println(ssid);  Serial.println("IP address: ");  Serial.println(WiFi.localIP());
  delay(2000);
}
void loop() {
  int  conn;
  int chip_id = ESP.getEfuseMac();;
  Serial.printf("  Flash Chip id = %08X\t", chip_id);
  Serial.println();
  Serial.println();
  String body = "&ChipId=" + String(i) + "&SentBy=" + "Aniket";
  int body_len = body.length();
  Serial.println(".....");
  Serial.println(); Serial.print("For sending parameters, connecting to ");      Serial.println(server);
  conn = client.connect(server, port);
  if (conn == 1) {
    String path = String("/mmmf/test_vib/test/test_post_new.php") + String("?") + body;
    Serial.println(body);
    Serial.println(); Serial.print("Sending Parameters...");
    //Request
    client.println("GET " + path + " HTTP/1.1");
    //Headers
    client.print("Host: "); client.println(server);
    client.println("Connection: Close");
    client.println();
    //No Body

    //Wait for server response
    while (client.available() == 0);

    //Print Server Response
    while (client.available()) {
      char c = client.read();
      Serial.write(c);
    }
  } else {
    client.stop();
    Serial.println("Connection Failed");
  }
  delay(5000);
}
