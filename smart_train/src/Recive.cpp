#include <esp_now.h>
#include <WiFi.h>

const int RedLED = 2;
unsigned long Time;
unsigned long OldTime1;
unsigned long OldTime2;
bool Trainstate;

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
    int a;
    bool b;
    
} struct_message;

// Create a struct_message called myData
struct_message myData;

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Bytes received: ");
  Serial.println(myData.a);
  Serial.print("Int: ");
  OldTime1 = Time;

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
  pinMode(RedLED, OUTPUT);
  Trainstate = true;
}
 
void loop() {
   Time = millis();
  if(myData.a == 1){
    digitalWrite(RedLED ,HIGH);
    }

  if(myData.a == 0) {
    digitalWrite(RedLED, LOW);
  }
   if(Time - OldTime1 >= 10000){
    Trainstate = false;
  }
  if(Time - OldTime1 <= 10000){
    Trainstate = true;
  }

  if(Trainstate == false && Time - OldTime2 >= 1000){
    Serial.println("No Connection with train");
     OldTime2 = Time;
  }
  if(Trainstate == true && Time - OldTime2 >= 1000){
    Serial.println("Connection with train OK");
     OldTime2 = Time;
  }
 
  

}