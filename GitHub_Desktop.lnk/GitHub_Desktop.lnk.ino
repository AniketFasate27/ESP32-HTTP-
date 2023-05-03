#include <WiFi.h>
const char* ssid = "NCAIR IOT";
const char* password = "Asim@123Tewari";

const char* server = "www.lab40.in";
const int port = 80;

char id_[6];
char id_1[10];
char kid[12];
char k_id[12];
int device_id;

WiFiClient client;
uint64_t chipid;
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
  chipid = ESP.getEfuseMac(); //The chip ID is essentially its MAC address(length: 6 bytes).
  sprintf(id_, "%04X", (uint16_t)(chipid >> 32));
  sprintf(id_1, "%08X\n", (uint32_t)chipid);
  strcat(kid, id_);
  strcat(kid, id_1);
  sprintf(k_id, "%c%c%c%c%c%c%c%c%c%c%c%c", kid[10], kid[11], kid[8], kid[9], kid[6], kid[7], kid[4], kid[5], kid[2], kid[3], kid[0], kid[1]);//k_id is the ssid name of AP
  Serial.println(k_id);
  device_id = atoi(k_id);;
  
  String body = "&ChipId=" + String(k_id) + "&SentBy=" + "Aniket";
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
