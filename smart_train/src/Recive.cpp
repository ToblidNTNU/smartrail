#include <esp_now.h>
#include <WiFi.h>
uint8_t broadcastAddress[] = {0x38, 0x18, 0x2b, 0x2f, 0x56, 0x50};

esp_now_peer_info_t peerInfo;

const int RedLED = 2;
unsigned long Time;
unsigned long OldTime1;
unsigned long OldTime2;
unsigned long OldTime3;
bool Trainstate;
int i = 0;
int PingTime; 

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
    int a;
    float x;
    float y;
    float z;
    int knapp;
    
} struct_message;

// Create a struct_message called myData
struct_message myData;
struct_message urData;


// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  //Serial.print("  Bytes received: ");
  Serial.println(myData.a);
  Serial.print("x-akse: ");
  Serial.print(myData.x);
  Serial.print("  y-akse: ");
  Serial.print(myData.y);
  Serial.print("  z-akse: ");
  Serial.print(myData.z);
  Serial.print("  knapp status: ");
  Serial.println(myData.knapp);
  PingTime = Time - myData.a;
  Serial.print("Reply from ESP1, Time=");
  Serial.print(PingTime);
  Serial.print(".00ms");
  OldTime1 = Time;

}
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
 
 
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
  esp_now_register_send_cb(esp_now_send_cb_t(OnDataSent));

 // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false; 

    // Add peer 
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }

  pinMode(RedLED, OUTPUT);

  Trainstate = false;
}
 
void loop() {
   Time = millis();
  if(myData.a == 1){
    digitalWrite(RedLED ,HIGH);
    }
  
  
   
  
  // Send message via ESP-NOW
  if(Time - OldTime1 >= 3000){
      urData.a = Time;
      esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &urData, sizeof(urData));
      OldTime1 = Time;      
  }
    /*
    if(Trainstate == false && Time <= 10000 && Time - OldTime3 >= 1000){
      Serial.println("connecting" + dots*i);
       i = i + 1;
       if(i >= 5) i = 1;
        OldTime3 = Time;
       }*/

  if(myData.a == 0) {
    digitalWrite(RedLED, LOW);
  }
   if(Time - OldTime1 >= 10000){
    Trainstate = false;
  }
  if(Time - OldTime1 <= 10000){
    Trainstate = true;
  }
 

  if(Trainstate == false && Time - OldTime2 >= 5000){
    Serial.println("No Connection with train");
     OldTime2 = Time;
  }
  if(Trainstate == true && Time - OldTime2 >= 5000){
    Serial.println("Connection with train OK");
     OldTime2 = Time;
  }
 
  

}