/*Código para automatização de um modem SIM7600GH utilizando o ESP32*/
/* Partes desse código são tiradas do exemplo PPP_Basic que vem de exemplo na biblioteca PPP */

#include <PPP.h> 
#include <WiFi.h>

#define MODEM_RX        16 //pinos correspontentes ao rx e tx do esp
#define MODEM_TX        17
#define MODEM_FC        ESP_MODEM_FLOW_CONTROL_NONE //se tiver flow control, colocar ESP_MODEM_FLOW_CONTROL_HW
#define MODEM_MODEL     PPP_MODEM_SIM7600 //modelo do modem predefinido pela biblioteca PPP
#define MODEM_APN       "zap.vivo.com.br" //apn referente ao cartão SIM do modem
#define MODEM_PIN       NULL // ou "0000" se o SIM tiver PIN


String GPS_Coord;

//configuração do acess point
IPAddress ap_ip(192, 168, 4, 1);
IPAddress ap_mask(255, 255, 255, 0);
IPAddress ap_leaseStart(192, 168, 4, 2);
IPAddress ap_dns(8, 8, 4, 4);

#define AP_SSID "ESP32-SIM7600"
#define AP_PASS "123456789"

   
void setup(){
  
  //modelo de iniciação seguido exatamente como no PPP_Basic
  Serial.begin(115200);
  
  Network.onEvent(onEvent);//função de debug que estava no exemplo PPP_Wifi_Bridge

  //configuração do acess point
  WiFi.AP.begin();
  WiFi.AP.config(ap_ip, ap_ip, ap_mask, ap_leaseStart, ap_dns);
  WiFi.AP.create(AP_SSID, AP_PASS);
  if (!WiFi.AP.waitStatusBits(ESP_NETIF_STARTED_BIT, 1000)) {
    Serial.println("AP! não iniciou");
    return;
  }
  
  PPP.setApn(MODEM_APN);
  PPP.setPin(MODEM_PIN);
  PPP.setResetPin(-1,false,200);
  PPP.setPins(MODEM_TX, MODEM_RX, -1, -1, MODEM_FC);
  Serial.println("Iniciando modem...");
  PPP.begin(MODEM_MODEL);

  /*se o esp32 conseguiu se comunicar vai aparecer essas informações */
  Serial.print("Manufacturer: ");
  Serial.println(PPP.cmd("AT+CGMI", 10000));
  Serial.print("Model: ");
  Serial.println(PPP.moduleName());
  Serial.print("IMEI: ");
  Serial.println(PPP.IMEI());

  bool attached = PPP.attached();
  if (!attached) {
    int i = 0;
    unsigned int s = millis();
    Serial.print("Esperando conexão");
    while (!attached && ((++i) < 600)) {
      Serial.print(".");
      delay(100);
      attached = PPP.attached();
    }
    Serial.print((millis() - s) / 1000.0, 1);
    Serial.println("s");
    attached = PPP.attached();
  }

  Serial.print("Attached: ");
  Serial.println(attached);
  Serial.print("State: ");
  Serial.println(PPP.radioState());
  if (attached) {
    Serial.print("Operator: ");
    Serial.println(PPP.operatorName());
    Serial.print("IMSI: ");
    Serial.println(PPP.IMSI());
    Serial.print("RSSI: ");
    Serial.println(PPP.RSSI());
    int ber = PPP.BER();
    if (ber > 0) {
      Serial.print("BER: ");
      Serial.println(ber);
      Serial.print("NetMode: ");
      Serial.println(PPP.networkMode());
    }

    Serial.println("Switching to data mode...");
    PPP.mode(ESP_MODEM_MODE_CMUX);  // Data and Command mixed mode
    if (!PPP.waitStatusBits(ESP_NETIF_CONNECTED_BIT, 1000)) {
      Serial.println("Failed to connect to internet!");
    } else {
      Serial.println("Connected to internet!");
    }
  } else {
    Serial.println("Failed to connect to network!");
  }

   /*    */
  Serial.println("Iniciando GPS...");
  PPP.cmd("AT+CGPS=1",1000);//comando de inicialização do gps
  String gps_conf=PPP.cmd("AT+CGPS?",10000);//só pra ter certeza que inicializou
  Serial.println(gps_conf);
  
 }

 void loop(){
    if(PPP.connected()){
    httpPostViaPPP("testehttp.requestcatcher.com",80,"/test","oi");
    httpGetViaPPP("google.com",80);
    }
   delay(5000);
 }


 
void httpPostViaPPP(const char *host, uint16_t port, const char *path, const String &body) {
  if (!PPP.connected()) {
    Serial.println("PPP not connected");
    return;
  }

  NetworkClient client;
  Serial.printf("Connecting to %s:%u ...\n", host, port);
  if (!client.connect(host, port)) {
    Serial.println("Network connect failed");
    return;
  }

  // Monta requisição HTTP
  String req;
  req += String("POST ") + path + " HTTP/1.1\r\n";
  req += String("Host: ") + host + "\r\n";
  req += "User-Agent: esp32-ppp/1.0\r\n";
  req += "Content-Type: application/x-www-form-urlencoded\r\n";
  req += String("Content-Length: ") + String(body.length()) + "\r\n";
  req += "Connection: close\r\n\r\n";
  req += body;

  // Envia requisição
  client.print(req.c_str());

  // Lê resposta 
  while (client.connected() && !client.available());
  while (client.available()) {
    client.read();  //Serial.write(client.read());
  }

  client.stop();
  Serial.println("\nRequest finished");
}

void httpGetViaPPP(const char *host, uint16_t port) {
  NetworkClient client;
  if (!client.connect(host, port)) {
    Serial.println("Connection Failed");
    return;
  }
  client.printf("GET / HTTP/1.1\r\nHost: %s\r\n\r\n", host);
  while (client.connected() && !client.available());
  while (client.available()) {
    client.read();  //Serial.write(client.read());
  }

  Serial.println("Connection Success");
  client.stop();
}

//debug que estava em ESP32_WIFI_BRIDGE
  void onEvent(arduino_event_id_t event, arduino_event_info_t info) {
  switch (event) {
    case ARDUINO_EVENT_PPP_START:     Serial.println("PPP Started"); break;
    case ARDUINO_EVENT_PPP_CONNECTED: Serial.println("PPP Connected"); break;
    case ARDUINO_EVENT_PPP_GOT_IP:
      Serial.println("PPP Got IP");
      Serial.println(PPP);
      WiFi.AP.enableNAPT(true);
      break;
    case ARDUINO_EVENT_PPP_LOST_IP:
      Serial.println("PPP Lost IP");
      WiFi.AP.enableNAPT(false);
      break;
    case ARDUINO_EVENT_PPP_DISCONNECTED:
      Serial.println("PPP Disconnected");
      WiFi.AP.enableNAPT(false);
      break;
    case ARDUINO_EVENT_PPP_STOP: Serial.println("PPP Stopped"); break;

    case ARDUINO_EVENT_WIFI_AP_START:
      Serial.println("AP Started");
      Serial.println(WiFi.AP);
      break;
    case ARDUINO_EVENT_WIFI_AP_STACONNECTED:    Serial.println("AP STA Connected"); break;
    case ARDUINO_EVENT_WIFI_AP_STADISCONNECTED: Serial.println("AP STA Disconnected"); break;
    case ARDUINO_EVENT_WIFI_AP_STAIPASSIGNED:
      Serial.print("AP STA IP Assigned: ");
      Serial.println(IPAddress(info.wifi_ap_staipassigned.ip.addr));
      break;
    case ARDUINO_EVENT_WIFI_AP_PROBEREQRECVED: Serial.println("AP Probe Request Received"); break;
    case ARDUINO_EVENT_WIFI_AP_STOP:           Serial.println("AP Stopped"); break;

    default: break;
  }
}

