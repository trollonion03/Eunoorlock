#include <WiFiNINA.h>
#include <SPI.h>

const char* ssid = "Eunhasu_test";
const char* password = "highground";
int status = WL_IDLE_STATUS;
WiFiServer server(80);
void setup() {
  Serial.begin(9600);
  pinMode(3, OUTPUT); //drv8835 needed
  pinMode(4, OUTPUT);
  digitalWrite(3, LOW); //initialize motor driver 
  analogWrite(4, 0);
  while (status != WL_CONNECTED) { 
    Serial.print("Attempting to connect to SSID: "); 
    Serial.println(ssid); 
    status = WiFi.begin(ssid, password); 
    // WiFi에 접속할 때까지 10초간 대기
    delay(10000); 
    } 
    Serial.println(""); 
    Serial.println("WiFi connected"); 
    // 서버 시작
    server.begin(); 
    Serial.println("Server started"); 
    // 할당받은 IP 주소를 시리얼 모니터로 출력
    char ips[24]; 
    IPAddress ip = WiFi.localIP(); 
    sprintf(ips, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);  // IP형식으로 출력
    Serial.println(ips);
}

void loop() {
  WiFiClient client = server.available(); 
  if (!client) { 
    return; 
  } 
  // 클라이언트로부터 데이터 수신을 기다림
  Serial.println("new client"); 
  while(!client.available()){ 
    delay(1); 
  } 
  // 요청(데이터)의 첫 줄을 읽어 req에 저장
  String req = client.readStringUntil('\r'); 
  Serial.println(req); 
  client.flush(); 
  int key = 0;
  if (req.indexOf("/gpio1/highground") !=-1 && key == 1) {
    key = 1;
    if (req.indexOf("/gpio2/lock")) {
      digitalWrite(3, HIGH);
      analogWrite(4, 175);
      delay(1500);
      analogWrite(4, 0);
      }
    else if (req.indexOf("/gpio2/unlock")) {
      digitalWrite(3, LOW);
      analogWrite(4, 175);
      delay(1500);
      analogWrite(4, 0);
      }
    else if (req.indexOf("/gpio2/disconnect")) {
      key = 0;
      client.stop();
      }
    }
  else {
    client.print("n");
    //client.stop(); //보류
  }
}
