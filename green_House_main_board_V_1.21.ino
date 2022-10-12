#include <EEPROM.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include "DHT.h"
//#include <WiFiManager.h>
#include <Stepper.h>
//#include <DHT.h>
#include <WiFi.h>
#include <FirebaseESP32.h>
#include <WiFiClient.h>
#include <ESP32Ping.h>
#define FIREBASE_HOST "ecotori-9c086-default-rtdb.firebaseio.com"                          // the project name address from firebase id
#define FIREBASE_AUTH "LHWvLR8nnBddFsfp58Ksq33YUsRSn5dww2cp72kj"            // the secret key generated from firebase

#define LIGHT_SENSOR_PIN 34 // ESP32 pin GIOP36 (ADC0)
TaskHandle_t Task1;
TaskHandle_t Task2;

unsigned long previous_rpm=0;
double umole = 0.0;
int WiFiConnectCount=0;
int pumpOnce=1;
int rotorOnce=1;
int ledOnce=1;
String new_ssid;
String new_pwd;
bool fireBaseFlag=false;
bool webserver=false;
bool scanning = false;
String wifilist;
String esid;
String epass = "";
bool home_wifi=false;
bool light1Flag=false;
bool light2Flag=false;
bool rotorOn = false;
bool scheduling=false;
bool pumpScheduling=false;
bool rotorScheduling=false;
bool led1SchedOn=false;
bool led2SchedOn=false;
bool led1ManualOn=false;
bool led2ManualOn=false;
bool finalSchedLed1On = false;
bool finalSchedLed2On =false;
bool overrideScheduling=false;
bool manualPumpOn=false;
bool schedulePumpOn =false;
bool overrideSchedulingPump=false;
bool manualRotorOn=false;
bool scheduleRotorOn =false;
bool overrideSchedulingRotor=false;
int ledResetOnce=0;
int pumpResetOnce=0;
int rotorResetOnce=0;
const char uid[] = "Greentest10";
const char pwd[] = "password";




IPAddress  ip(192,168,4,1);
IPAddress  gateway(192,168,4,1);
IPAddress  subnet(255,255,255,0);

WebServer serverr(80);
// JSON data buffer
StaticJsonDocument<250> jsonDocument;
char buffer[250];



//Define FirebaseESP32 data object
FirebaseData firebaseData;
FirebaseJson json;

//Stepper data
const int stepsPerRevolution = 2048;
#define EN_PIN    13 //enable (CFG6)
#define DIR_PIN   19 //direction
#define STEP_PIN  18 //step

//DHT22
#define DHTPIN 17    
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

//Flow Sensor(YF S201)
const int Flow_Pin = 21;
volatile int  Pulse_Count;
int  Liter_per_hour;
unsigned long Current_Time, Loop_Time;

//RPM Sensor
int sensor = 16; // Hall sensor at pin 2
volatile byte counts;
 int rpm; //unsigned gives only positive values
unsigned long previoustime;

//Water Pump
int  pump = 23;
int lightIntervalMilis=0;
int lightRunningMilis=0;
int previousLightMilis=0;
int lightOnOnce=0;
int lightOffOnce=0;

int pumpIntervalMilis=0;
int pumpRunningMilis=0;
int previousPumpMilis=0;
int pumpOnOnce=0;
int pumpOffOnce=0;

int rotorIntervalMilis=0;
int rotorRunningMilis=0;
int previousRotorMilis=0;
int rotorOnOnce=0;
int rotorOffOnce=0;

//LED
const int LED1 = 32;
const int LED2 = 33;
//const int LED3 = 13;
//const int LED4 = 26;

void accessPoint(){
  WiFi.mode(WIFI_AP_STA);//WIFI_AP_STA
  WiFi.softAPConfig(ip, gateway, subnet);
  WiFi.softAP(uid, pwd,7,0,3);
  delay(2000);
  //Serial.print("ESP32 IP as soft AP: ");
  //Serial.println(WiFi.softAPIP());
  }
void connectFirebase()
{
  //Serial.println("here");
  String ssid_no_space = esid;
    ssid_no_space.replace("%20"," ");
    int esid_length = ssid_no_space.length()+1;
    char ch_esid[esid_length];
    ssid_no_space.toCharArray(ch_esid,esid_length);
    Serial.println(ch_esid);
    
    

    int epass_length = epass.length()+1;
    char ch_epass[epass_length];
    epass.toCharArray(ch_epass,epass_length);
    Serial.println(ch_epass);

   
    
    WiFi.mode(WIFI_STA);
  WiFi.begin(ch_esid, ch_epass);
  
  Serial.println("\n\n=====================");
  Serial.println("Connecting to Wi-Fi");
  Serial.println("=====================\n\n");

  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

  if(WiFi.status() != WL_CONNECTED){
    EEPROM.write(0, 0);
EEPROM.write(1, 0);
EEPROM.write(2, 0);
EEPROM.write(3, 0);
EEPROM.write(4, 0);
EEPROM.write(5, 0);
EEPROM.write(6, 0);
EEPROM.write(7, 0);
EEPROM.write(8, 0);
EEPROM.write(9, 0);
EEPROM.write(10, 0);
EEPROM.write(11, 0);
EEPROM.write(12, 0);
EEPROM.write(13, 0);
EEPROM.write(14, 0);
EEPROM.write(15, 0);
EEPROM.write(16, 0);
EEPROM.write(17, 0);
EEPROM.write(18, 0);
EEPROM.write(19, 0);
EEPROM.write(20, 0);
EEPROM.write(21, 0);
EEPROM.write(22, 0);
EEPROM.write(23, 0);
EEPROM.write(24, 0);
EEPROM.write(25, 0);

EEPROM.write(26, 0);
EEPROM.write(27, 0);
EEPROM.write(28, 0);
EEPROM.write(29, 0);
EEPROM.write(30, 0);
EEPROM.write(31, 0);
EEPROM.write(32, 0);
EEPROM.write(33, 0);
EEPROM.write(34, 0);
EEPROM.write(35, 0);
EEPROM.write(36, 0);
EEPROM.write(37, 0);
EEPROM.write(38, 0);
EEPROM.write(39, 0);
EEPROM.write(40, 0);
EEPROM.write(41, 0);
EEPROM.write(42, 0);
EEPROM.write(43, 0);
EEPROM.write(44, 0);
EEPROM.write(45, 0);
EEPROM.write(46, 0);
EEPROM.write(47, 0);
EEPROM.write(48, 0);
EEPROM.write(49, 0);
EEPROM.write(50, 0);

EEPROM.write(51, 0);
EEPROM.write(52, 0);
EEPROM.write(53, 0);
EEPROM.write(54, 0);
EEPROM.write(55, 0);
EEPROM.write(56, 0);
EEPROM.write(57, 0);
EEPROM.write(58, 0);
EEPROM.write(59, 0);
EEPROM.write(60, 0);
EEPROM.write(61, 0);
EEPROM.write(62, 0);
EEPROM.write(63, 0);
EEPROM.write(64, 0);
EEPROM.write(65, 0);
EEPROM.write(66, 0);
EEPROM.write(67, 0);
EEPROM.write(68, 0);
EEPROM.write(69, 0);
EEPROM.write(70, 0);
EEPROM.write(71, 0);
EEPROM.write(72, 0);
EEPROM.write(73, 0);
EEPROM.write(74, 0);
EEPROM.write(75, 0);

EEPROM.write(76, 0);
EEPROM.write(77, 0);
EEPROM.write(78, 0);
EEPROM.write(79, 0);
EEPROM.write(80, 0);
EEPROM.write(81, 0);
EEPROM.write(82, 0);
EEPROM.write(83, 0);
EEPROM.write(84, 0);
EEPROM.write(85, 0);
EEPROM.write(86, 0);
EEPROM.write(87, 0);
EEPROM.write(88, 0);
EEPROM.write(89, 0);
EEPROM.write(90, 0);
EEPROM.write(91, 0);
EEPROM.write(92, 0);
EEPROM.write(93, 0);
EEPROM.write(94, 0);
EEPROM.write(95, 0);
EEPROM.write(96, 0);
EEPROM.commit();
ESP.restart();
    }


  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);                              // connect to firebase

  Serial.println("\n\n=====================");
  Serial.println("Connecting to Firebase");
  Serial.println("=====================\n\n");

  
  Serial.println("\n\n=====================");
  Serial.println("Firebase Connected!");
  Serial.println("=====================\n\n");

    
    //Flow Sensor
    pinMode(Flow_Pin, INPUT);
    attachInterrupt(0, Detect_Rising_Edge, RISING);                                 
    Current_Time = millis();
    Loop_Time = Current_Time;
    
    //DHT22
    dht.begin();
    
    //RPM Sensor
    attachInterrupt(0, count_function, RISING); //Interrupts are called on Rise of Input
    pinMode(sensor, INPUT); //Sets sensor as input
    counts= 0;
    rpm = 0;
    previoustime = 0; //Initialise the values

    //Water pump
    pinMode(pump,OUTPUT);

    //LED
    pinMode(LED1,OUTPUT);
    pinMode(LED2,OUTPUT);
    //pinMode(LED3,OUTPUT);
    //pinMode(LED4,OUTPUT);

    //Stepper_Motor
 
 
 //json.set("/Water_Pump", 0);
  //json.set("/Stepper_Motor", 0);
  //json.set("/LED(1)", 0);
  //json.set("/LED(2)", 0);
  //json.set("/LED(3)", 0);
  //json.set("/LED(4)", 0);
  //json.set("/IP Address", 0);
   json.set("/acknowledge", 0);



}

void setup_routing() {     
  //serverr.send(200, "application/json", buffer);
  serverr.on("/scanwifi", scanwifi);
  serverr.on("/getwifilist", sendwifi);//  http://192.168.4.1//getwifilist     to get wifilist
  serverr.on("/uid", getUID);// http://192.168.4.1/uid   to get uid
  serverr.on("/clearssid", clear_ssid_pwd);// http://192.168.4.1/clearssid  to reset ssid and password   
  serverr.on("/setwifi",HTTP_POST,connectWifiResp);// http://192.168.4.1/setwifi  this is a post request, it will contain json body having ssid and password carried in application part
  serverr.on("/connectwifi",home_network_status);// http://192.168.4.1/connectwifi //this will finally make espcam connect with home network and will get restarted to get connected with mqtt broker
   
  
  // start server    
  serverr.begin();    
}

String IpAddress2String(const IPAddress& ipAddress)
{
  return String(ipAddress[0]) + String(".") +\
  String(ipAddress[1]) + String(".") +\
  String(ipAddress[2]) + String(".") +\
  String(ipAddress[3])  ; 
}

void create_json(String tag) {  
  jsonDocument.clear();  
  jsonDocument["UID"] = tag;
  //jsonDocument["value"] = value;
  //jsonDocument["unit"] = unit;
  serializeJson(jsonDocument, buffer);
}


void create_json_1(String value1) {  
  
  serializeJson(jsonDocument, buffer);
}
void create_json_2(String value1,String value2) {  
  jsonDocument.clear();  
  
  jsonDocument["WiFi_status"] = value1;
  jsonDocument["Wifi_Local_IP"] = value2;
  serializeJson(jsonDocument, buffer);
}
void create_json_3(String tag) {  
  jsonDocument.clear();  
  jsonDocument["wifi clear status"] = tag;
  serializeJson(jsonDocument, buffer);
}


void create_json_scan(String value1) {  
  jsonDocument.clear();  
  jsonDocument["Scane"] = value1;
  serializeJson(jsonDocument, buffer);
}

void scanwifi(){
  scanning = true;
  create_json_scan("true");
  serverr.send(200, "application/json", buffer);
  }

void getWifiList() {

  jsonDocument.clear();  
  JsonArray ssid_array = jsonDocument.createNestedArray("wifi");
   
  Serial.println("scanning");
  int n = WiFi.scanNetworks();
    Serial.println("scan done");
    if (n == 0) {
        Serial.println("no networks found");
    } else {
        Serial.print(n);
        Serial.println(" networks found");
        
       
         
        for (int i = 0; i < n; ++i) {
            // Print SSID and RSSI for each network found
              wifilist += WiFi.SSID(i)+":";
              DynamicJsonDocument doc(1024);
              doc["ssid"] = WiFi.SSID(i);//{ssid:filano}
              ssid_array.add(doc);     
        }
        //Serial.println(wifilist);
    }
  serializeJson(jsonDocument, buffer);
  //create_json_1(wifilist);
  wifilist="";
  scanning=false;
  
}

void sendwifi(){
  serverr.send(200, "application/json", buffer);
  }


void connectWifiResp(){
 
if (serverr.hasArg("plain") == false) {
  }
  String body = serverr.arg("plain");
  deserializeJson(jsonDocument, body);

  String myssid = jsonDocument["ssid"];
  String mypwd = jsonDocument["pwd"];
  Serial.println(myssid+" "+mypwd);

 


 
    new_ssid = myssid;
    String ssid_no_space = new_ssid;
    ssid_no_space.replace("%20"," ");
    int new_ssid_length = ssid_no_space.length()+1;
    char ch_new_ssid[new_ssid_length];
    ssid_no_space.toCharArray(ch_new_ssid,new_ssid_length); 
    new_pwd = mypwd;

    int new_pwd_length = new_pwd.length()+1;
    char ch_new_pwd[new_pwd_length];
    new_pwd.toCharArray(ch_new_pwd,new_pwd_length);
    //Serial.println(ch_new_pwd);

     /////////////////////////

     if (new_ssid.length() > 0 && new_pwd.length() > 0) {
        Serial.println("clearing eeprom");
        for (int i = 0; i < 96; ++i) {
          EEPROM.write(i, 0);
        }
        Serial.println(new_ssid);
        Serial.println("");
        Serial.println(new_pwd);
        Serial.println("");
        Serial.println("writing eeprom ssid:");
        for (int i = 0; i < new_ssid.length(); ++i)
        {
          EEPROM.write(i, new_ssid[i]);
          Serial.print("Wrote: ");
          Serial.println(new_ssid[i]);
        }
        //Serial.println("writing eeprom pass:");
        for (int i = 0; i < new_pwd.length(); ++i)
        {
          EEPROM.write(32 + i, new_pwd[i]);
          
        }
        EEPROM.commit();
     
     ////////////////////////
    
    
  }
  
  create_json_2("Connecting",new_ssid);
  //wifilist="";
  serverr.send(200, "application/json", buffer);
   //serverr.send(200, "application/json", "{}");
}

void home_network_status(){
    String ssid_no_space = new_ssid;
    ssid_no_space.replace("%20"," ");
    int new_ssid_length = ssid_no_space.length()+1;
    char ch_new_ssid[new_ssid_length];
    ssid_no_space.toCharArray(ch_new_ssid,new_ssid_length);
    Serial.println(ch_new_ssid);
    
    //new_pwd = serverr.pathArg(1);

    int new_pwd_length = new_pwd.length()+1;
    char ch_new_pwd[new_pwd_length];
    new_pwd.toCharArray(ch_new_pwd,new_pwd_length);
    Serial.println(ch_new_pwd);

    WiFi.begin(ch_new_ssid, ch_new_pwd);
    
  Serial.println("\n\n=====================");
  Serial.println("Connecting to Wi-Fi");
  Serial.println("=====================\n\n");

  while ((WiFi.status() != WL_CONNECTED)&&(WiFiConnectCount<=20)){
    WiFiConnectCount++;
    delay(500);
    Serial.print(WiFiConnectCount);
  }
  
  if(WiFi.status() != WL_CONNECTED){
    String statuss = "Can not be connected with "+String(ch_new_ssid);
    Serial.println(statuss);
    create_json_2(statuss,"None");

  serverr.send(200, "application/json", buffer);
  
    }
    if(WiFi.status() == WL_CONNECTED){
      bool success = Ping.ping("www.google.com", 3);
 
  if(!success){
    Serial.println("Ping failed");
    create_json_2("Wifi_connected","No Internet");
  }
 
  Serial.println("Ping succesful.");
      
    home_wifi=true;
    String statuss = "connected with "+String(ch_new_ssid);
    Serial.println(statuss);
    Serial.print("Connected. IP: ");
    Serial.println(WiFi.localIP());
   
    
    
    create_json_2(statuss,IpAddress2String(WiFi.localIP()));
  serverr.send(200, "application/json", buffer);
    }


    
    
    
  }
 

void clear_ssid_pwd(){
  EEPROM.write(0, 0);
EEPROM.write(1, 0);
EEPROM.write(2, 0);
EEPROM.write(3, 0);
EEPROM.write(4, 0);
EEPROM.write(5, 0);
EEPROM.write(6, 0);
EEPROM.write(7, 0);
EEPROM.write(8, 0);
EEPROM.write(9, 0);
EEPROM.write(10, 0);
EEPROM.write(11, 0);
EEPROM.write(12, 0);
EEPROM.write(13, 0);
EEPROM.write(14, 0);
EEPROM.write(15, 0);
EEPROM.write(16, 0);
EEPROM.write(17, 0);
EEPROM.write(18, 0);
EEPROM.write(19, 0);
EEPROM.write(20, 0);
EEPROM.write(21, 0);
EEPROM.write(22, 0);
EEPROM.write(23, 0);
EEPROM.write(24, 0);
EEPROM.write(25, 0);

EEPROM.write(26, 0);
EEPROM.write(27, 0);
EEPROM.write(28, 0);
EEPROM.write(29, 0);
EEPROM.write(30, 0);
EEPROM.write(31, 0);
EEPROM.write(32, 0);
EEPROM.write(33, 0);
EEPROM.write(34, 0);
EEPROM.write(35, 0);
EEPROM.write(36, 0);
EEPROM.write(37, 0);
EEPROM.write(38, 0);
EEPROM.write(39, 0);
EEPROM.write(40, 0);
EEPROM.write(41, 0);
EEPROM.write(42, 0);
EEPROM.write(43, 0);
EEPROM.write(44, 0);
EEPROM.write(45, 0);
EEPROM.write(46, 0);
EEPROM.write(47, 0);
EEPROM.write(48, 0);
EEPROM.write(49, 0);
EEPROM.write(50, 0);

EEPROM.write(51, 0);
EEPROM.write(52, 0);
EEPROM.write(53, 0);
EEPROM.write(54, 0);
EEPROM.write(55, 0);
EEPROM.write(56, 0);
EEPROM.write(57, 0);
EEPROM.write(58, 0);
EEPROM.write(59, 0);
EEPROM.write(60, 0);
EEPROM.write(61, 0);
EEPROM.write(62, 0);
EEPROM.write(63, 0);
EEPROM.write(64, 0);
EEPROM.write(65, 0);
EEPROM.write(66, 0);
EEPROM.write(67, 0);
EEPROM.write(68, 0);
EEPROM.write(69, 0);
EEPROM.write(70, 0);
EEPROM.write(71, 0);
EEPROM.write(72, 0);
EEPROM.write(73, 0);
EEPROM.write(74, 0);
EEPROM.write(75, 0);

EEPROM.write(76, 0);
EEPROM.write(77, 0);
EEPROM.write(78, 0);
EEPROM.write(79, 0);
EEPROM.write(80, 0);
EEPROM.write(81, 0);
EEPROM.write(82, 0);
EEPROM.write(83, 0);
EEPROM.write(84, 0);
EEPROM.write(85, 0);
EEPROM.write(86, 0);
EEPROM.write(87, 0);
EEPROM.write(88, 0);
EEPROM.write(89, 0);
EEPROM.write(90, 0);
EEPROM.write(91, 0);
EEPROM.write(92, 0);
EEPROM.write(93, 0);
EEPROM.write(94, 0);
EEPROM.write(95, 0);
EEPROM.write(96, 0);
EEPROM.commit();

    create_json_3("wifi reset");
  //wifilist="";
  serverr.send(200, "application/json", buffer);
  
}


void getUID() {
  
    create_json(uid);
  
  serverr.send(200, "application/json", buffer);
  }


void setup() {


   xTaskCreatePinnedToCore(
                    Task1code,   /* Task function. */
                    "Task1",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task1,      /* Task handle to keep track of created task */
                    0);          /* pin task to core 0 */                  
  delay(500); 

  //create a task that will be executed in the Task2code() function, with priority 1 and executed on core 1
  xTaskCreatePinnedToCore(
                    Task2code,   /* Task function. */
                    "Task2",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task2,      /* Task handle to keep track of created task */
                    1);          /* pin task to core 1 */
    delay(500); 
  
  digitalWrite(LED1,LOW);
    digitalWrite(LED2,LOW);
  digitalWrite(pump,LOW);
   pinMode(EN_PIN, OUTPUT);
  digitalWrite(EN_PIN, HIGH); //deactivate driver (LOW active)
  pinMode(DIR_PIN, OUTPUT);
  
  digitalWrite(DIR_PIN, LOW); //LOW or HIGH
  pinMode(STEP_PIN, OUTPUT);
  digitalWrite(STEP_PIN, LOW);

  digitalWrite(EN_PIN, LOW); //activate driver
    Serial.begin(115200);
  EEPROM.begin(512);
  delay(500);
  //Serial.println("from eeprom");
  for (int i = 0; i < 32; ++i)
  {
    esid += char(EEPROM.read(i));
  }
  Serial.println();
  Serial.print("SSID: ");
  Serial.println(esid);
  Serial.println("Reading EEPROM pass");
  for (int i = 32; i < 96; ++i)
  {
    epass += char(EEPROM.read(i));
  }
  Serial.print("PASS: ");
  Serial.println(epass);
  Serial.println("from eeprom");

  
  Serial.println(esid.length());
  if((esid=="")&&(epass=="")){
  accessPoint();
  setup_routing();
  webserver=true;
  fireBaseFlag=false;
  }

  if ((esid != "") && (epass != "")) {
    
  connectFirebase();
  webserver=false;
  fireBaseFlag=true;
    }
  
  
    
      

    
    

}

void Task1code( void * pvParameters ){
  //Serial.print("Task1 running on core ");
  //Serial.println(xPortGetCoreID());

  for(;;){
       if((webserver==true)&&(fireBaseFlag==false)){
    if(scanning == true){
      getWifiList();
      }
    if(home_wifi==true){
      ESP.restart();
      }
  serverr.handleClient();
  }
  if((webserver==false)&&(fireBaseFlag==true)){
  //Flow Sensor
    Current_Time = millis();
    Serial.println("millis"+String(float(millis()/1000)));
    if(Current_Time >= (Loop_Time + 1000))
    {
      Loop_Time = Current_Time;
      Liter_per_hour = (Pulse_Count * 60 / 7.5);
      Pulse_Count = 0;
      Serial.print(Liter_per_hour, DEC);
      Serial.println(" Liter/hour");

          json.set("/Liter_hour", Liter_per_hour);

    }
    
    //DHT22
    
    float humi  = dht.readHumidity();
    if(isnan(humi)){
      humi=0.1;
      }
    //float humi = 15.5;
    float tempC = dht.readTemperature();
    if(isnan(tempC)){
      tempC=0.1;
      }
    //float tempC = 25.6;
    float tempF = dht.readTemperature(true);
    if(isnan(tempF)){
      tempF=0.1;
      }
    //float tempF = 10.32;
    Serial.print("Humidity: ");
    Serial.print(humi);
    Serial.print("%");
json.set("/Humidity(%)", humi);
 
    Serial.print("  |  ");
    Serial.print("Temperature: ");
    Serial.print(tempC);
    Serial.print("°C  ~  ");

            json.set("/Temperature(C)", tempC);

    Serial.print(tempF);
    Serial.println("°F");

            json.set("/Temperature(F)", tempF);

    //Current Sensor
    
    unsigned int h=0;
    float AcsValue=0.0,Samples=0.0,AvgAcs=0.0,AcsValueF=0.0;
    for (int h = 0; h < 150; h++){ //Get 150 samples
      AcsValue = analogRead(35);     //Read current sensor values   
      Samples = Samples + AcsValue;  //Add samples together
      delay (3); // let ADC settle before next sample 3ms
    }
    AvgAcs=Samples/150.0;//Taking Average of Samples

    //((AvgAcs * (5.0 / 1024.0)) is converitng the read voltage in 0-5 volts
    //2.5 is offset(I assumed that arduino is working on 5v so the viout at no current comes
    //out to be 2.5 which is out offset. If your arduino is working on different voltage than 
    //you must change the offset according to the input voltage)
    //0.185v(185mV) is rise in output voltage when 1A current flows at input
    AcsValueF = (2.5 - (AvgAcs * (5.0 / 4096.0)) )/0.66;
    Serial.println(AcsValueF);//Print the read current on Serial monitor

            json.set("/Current(A)", AcsValueF);
    
   

           

    delay(50);
    
    //RPM Sensor
    if(Current_Time-previous_rpm >=1000){
      detachInterrupt(0); //Interrupts are disabled
    rpm = 60*1000/(millis() - previoustime)*counts;
    previoustime = millis(); //Resets the clock
    counts= 0; //Resets the counter
    Serial.print("RPM=");
    Serial.println(rpm);
    previous_rpm=Current_Time;
     attachInterrupt(0, count_function, RISING); //Counter restarted
    json.set("/RPM", rpm);
      }//Update RPM every second
     
   
                

//Sketch to calculate the intensity of light using a photo-resistor sensor 


// reads the input on analog pin (value between 0 and 4095)
  int analogValue = analogRead(LIGHT_SENSOR_PIN);

  Serial.print("L-Analog Value = ");
  //Serial.print(analogValue);   // the raw analog reading
  //int myVal = map(analogValue,0,1023,1023,0);
  umole = analogValue *0.02;
  
  Serial.println(umole);
  json.set("/Light Intensity", umole);
  
  
   //Water Pump
    

Firebase.updateNode(firebaseData,"/Greentest10",json); // Enter motherboard unique Id here

 if (Firebase.get(firebaseData,"/Greentest10/schedules/lightIntervalMilis")) 
{
   if (firebaseData.dataType() == "int") {
      lightIntervalMilis = firebaseData.intData();
      Serial.println("light Interval: "+String(lightIntervalMilis));
   }
}

if (Firebase.get(firebaseData,"/Greentest10/schedules/lightRunningMilis")) 
{
   if (firebaseData.dataType() == "int") {
      lightRunningMilis = firebaseData.intData();
      Serial.println("light Running: "+String(lightRunningMilis));
   }
}

if (Firebase.get(firebaseData,"/Greentest10/schedules/pumpIntervalMilis")) 
{
   if (firebaseData.dataType() == "int") {
      pumpIntervalMilis = firebaseData.intData();
      Serial.println("pump Interval: "+String(pumpIntervalMilis));
   }
}

if (Firebase.get(firebaseData,"/Greentest10/schedules/pumpRunningMilis")) 
{
   if (firebaseData.dataType() == "int") {
      pumpRunningMilis = firebaseData.intData();
      Serial.println("pump Running: "+String(pumpRunningMilis));
   }
}

if (Firebase.get(firebaseData,"/Greentest10/schedules/rotorIntervalMilis")) 
{
   if (firebaseData.dataType() == "int") {
      rotorIntervalMilis = firebaseData.intData();
      Serial.println("rotor Interval: "+String(rotorIntervalMilis));
   }
}

if (Firebase.get(firebaseData,"/Greentest10/schedules/rotorRunningMilis")) 
{
   if (firebaseData.dataType() == "int") {
      rotorRunningMilis = firebaseData.intData();
      Serial.println("rotor Running: "+String(rotorRunningMilis));
   }
}
   if (Firebase.get(firebaseData,"/Greentest10/Water_Pump")) 
{

    if (firebaseData.dataType() == "int") {
      int D1 = firebaseData.intData();
      
          if (D1==1)
          {
             
            Serial.println("pump D1 ON");
              //digitalWrite(pump,HIGH); //Device1 is ON 
              overrideSchedulingPump=true;
              manualPumpOn=true;
          }
          if (D1==0){
             Serial.println("pump D1 OFF");
              //digitalWrite(pump,LOW);//Device1 if OFF
              overrideSchedulingPump=false;
              manualPumpOn=false;
            }}}
    
   

 if (Firebase.get(firebaseData,"/Greentest10/LED(1)"))  
{
  //Serial.println("asad");
    if (firebaseData.dataType() == "int") {
      int D2 = firebaseData.intData();
          if (D2==1)
          {
            Serial.println("LED1 ON");
              overrideScheduling=true;
              led1ManualOn=true;
              //digitalWrite(LED1,HIGH); //Device1 is ON 
              //light1Flag=true;
          }
          if (D2==0){
             Serial.println("LED1 OFF");
             overrideScheduling=false;
             led1ManualOn=false;
              //digitalWrite(LED1,LOW);//Device1 if OFF
              //light1Flag=false;
            }
    }}

             
             if (Firebase.get(firebaseData,"/Greentest10/LED(2)")) 
{
  //Serial.println("*");
    if (firebaseData.dataType() == "int") {
      int D3 = firebaseData.intData();
          if (D3==1)
          {
            //Serial.println("LED1 ON");
              //digitalWrite(LED1,HIGH); //Device1 is ON
              //led1ManualOn=true; 
           Serial.println("LED2 ON");
           led2ManualOn=true;
           //digitalWrite(LED2,HIGH); //Device1 is ON
           //light1Flag=true;
           //light2Flag=true; 
          
          }
           if (D3==0){
             //Serial.println("LED1 OFF");
              //digitalWrite(LED1,LOW);//Device1 if OFF
              //led1ManualOn=false;
              Serial.println("LED2 OFF");
              led2ManualOn=false;
              //digitalWrite(LED2,LOW);//Device1 if OFF
              //light1Flag=false;
              //light2Flag=false; 
            }
    }}

if (Firebase.get(firebaseData,"/Greentest10/schedLED(1)")) 
{
  //Serial.println("asad");
    if (firebaseData.dataType() == "int") {
      int D2 = firebaseData.intData();
          if (D2==1)
          {
            Serial.println("Sched LED1 ON");
              led1SchedOn=true;
              //digitalWrite(LED1,HIGH); //Device1 is ON 
              //light1Flag=true;
          }
          if (D2==0){
             Serial.println("Sched LED1 Off");
              led1SchedOn=false;
              //digitalWrite(LED1,LOW);//Device1 if OFF
              //light1Flag=false;
            }
    }}

             
             if (Firebase.get(firebaseData,"/Greentest10/schedLED(2)")) 
{
  //Serial.println("*");
    if (firebaseData.dataType() == "int") {
      int D3 = firebaseData.intData();
          if (D3==1)
          {
            //Serial.println("sched LED1 ON");
              //digitalWrite(LED1,HIGH); //Device1 is ON 
              //led1SchedOn=true;
           Serial.println("sched LED2 ON");
           led2SchedOn=true;
           //digitalWrite(LED2,HIGH); //Device1 is ON
           //light1Flag=true;
           //light2Flag=true; 
          
          }
           if (D3==0){
             ///Serial.println("sched LED1 OFF");
             //led1SchedOn=false;
              //digitalWrite(LED1,LOW);//Device1 if OFF
              Serial.println("sched LED2 OFF");
              led2SchedOn=false;
              //digitalWrite(LED2,LOW);//Device1 if OFF
              //light1Flag=false;
              //light2Flag=false; 
            }
    }}


 ///Stepper Motor
   if (Firebase.get(firebaseData,"/Greentest10/Stepper_Motor")) 
{
    if (firebaseData.dataType() == "int") {
      int D3 = firebaseData.intData();
          if (D3==1)
          {
            Serial.println("stepper ON");
             //Device1 is ON 
              //rotorOn=true;
              overrideSchedulingRotor=true;
              manualRotorOn=true;
         }
           if (D3==0){
             Serial.println("stepsPerRevolution OFF");
              //rotorOn=false;//Device1 if OFF
              overrideSchedulingRotor=false;
              manualRotorOn=false;
              }
    }}

    if (Firebase.get(firebaseData,"/Greentest10/reconfigure")) 
{
    if (firebaseData.dataType() == "int") {
      int D3 = firebaseData.intData();
          if (D3==1)
          {
            Serial.println("reconfiguring");
            json.set("/acknowledge", 1);
            Firebase.updateNode(firebaseData,"/Greentest10",json);
EEPROM.write(0, 0);
EEPROM.write(1, 0);
EEPROM.write(2, 0);
EEPROM.write(3, 0);
EEPROM.write(4, 0);
EEPROM.write(5, 0);
EEPROM.write(6, 0);
EEPROM.write(7, 0);
EEPROM.write(8, 0);
EEPROM.write(9, 0);
EEPROM.write(10, 0);
EEPROM.write(11, 0);
EEPROM.write(12, 0);
EEPROM.write(13, 0);
EEPROM.write(14, 0);
EEPROM.write(15, 0);
EEPROM.write(16, 0);
EEPROM.write(17, 0);
EEPROM.write(18, 0);
EEPROM.write(19, 0);
EEPROM.write(20, 0);
EEPROM.write(21, 0);
EEPROM.write(22, 0);
EEPROM.write(23, 0);
EEPROM.write(24, 0);
EEPROM.write(25, 0);

EEPROM.write(26, 0);
EEPROM.write(27, 0);
EEPROM.write(28, 0);
EEPROM.write(29, 0);
EEPROM.write(30, 0);
EEPROM.write(31, 0);
EEPROM.write(32, 0);
EEPROM.write(33, 0);
EEPROM.write(34, 0);
EEPROM.write(35, 0);
EEPROM.write(36, 0);
EEPROM.write(37, 0);
EEPROM.write(38, 0);
EEPROM.write(39, 0);
EEPROM.write(40, 0);
EEPROM.write(41, 0);
EEPROM.write(42, 0);
EEPROM.write(43, 0);
EEPROM.write(44, 0);
EEPROM.write(45, 0);
EEPROM.write(46, 0);
EEPROM.write(47, 0);
EEPROM.write(48, 0);
EEPROM.write(49, 0);
EEPROM.write(50, 0);

EEPROM.write(51, 0);
EEPROM.write(52, 0);
EEPROM.write(53, 0);
EEPROM.write(54, 0);
EEPROM.write(55, 0);
EEPROM.write(56, 0);
EEPROM.write(57, 0);
EEPROM.write(58, 0);
EEPROM.write(59, 0);
EEPROM.write(60, 0);
EEPROM.write(61, 0);
EEPROM.write(62, 0);
EEPROM.write(63, 0);
EEPROM.write(64, 0);
EEPROM.write(65, 0);
EEPROM.write(66, 0);
EEPROM.write(67, 0);
EEPROM.write(68, 0);
EEPROM.write(69, 0);
EEPROM.write(70, 0);
EEPROM.write(71, 0);
EEPROM.write(72, 0);
EEPROM.write(73, 0);
EEPROM.write(74, 0);
EEPROM.write(75, 0);

EEPROM.write(76, 0);
EEPROM.write(77, 0);
EEPROM.write(78, 0);
EEPROM.write(79, 0);
EEPROM.write(80, 0);
EEPROM.write(81, 0);
EEPROM.write(82, 0);
EEPROM.write(83, 0);
EEPROM.write(84, 0);
EEPROM.write(85, 0);
EEPROM.write(86, 0);
EEPROM.write(87, 0);
EEPROM.write(88, 0);
EEPROM.write(89, 0);
EEPROM.write(90, 0);
EEPROM.write(91, 0);
EEPROM.write(92, 0);
EEPROM.write(93, 0);
EEPROM.write(94, 0);
EEPROM.write(95, 0);
EEPROM.write(96, 0);
EEPROM.commit();
json.set("/acknowledge", 0);
Firebase.updateNode(firebaseData,"/Greentest10",json);
delay(5000);

ESP.restart();

         }
           
    }}
    //here
    
    //Serial.println("led1SchedOn"+String(led1SchedOn));
    if(lightRunningMilis >= 1000){
      scheduling=true;
      ledResetOnce=0;
    if(Current_Time -previousLightMilis <= lightRunningMilis){
      
      if(lightOnOnce==0){
        Serial.println("Im here"+String(led1SchedOn));
        if(led1SchedOn==true){
          finalSchedLed1On=true;
          //digitalWrite(LED1,LOW); //Device1 is ON
          Serial.println("***************");
      Serial.println(" ");
      Serial.println("SCHED lED 1 On");
      Serial.println(" ");
      Serial.println("***************");
          }
        if((led1SchedOn==true)&&(led2SchedOn==true)){
          finalSchedLed1On=true;
          finalSchedLed2On=true;
          //digitalWrite(LED1,LOW);
          //digitalWrite(LED2,LOW);
          Serial.println("***************");
      Serial.println(" ");
      Serial.println("SCHED lED 1 On");
      Serial.println("SCHED lED 2 On");
      Serial.println(" ");
      Serial.println("***************");
          }
          
      
      lightOnOnce=1;
      }
      
    //previousLightMilis = Current_Time;
    }
    if((Current_Time - previousLightMilis > lightRunningMilis) && (Current_Time -previousLightMilis <= lightIntervalMilis+lightRunningMilis)){
          //if(lightOffOnce==0){
            
           

          
          
              finalSchedLed1On=false;
              finalSchedLed2On=false;
              //digitalWrite(LED1,HIGH);
              //digitalWrite(LED2,HIGH);
              Serial.println("***************");
      Serial.println(" ");
      Serial.println("lED 1 OFF");
      Serial.println("lED 2 OFF");
      Serial.println(" ");
      Serial.println("***************");

            

              Serial.println("***************");
              Serial.println(" ");
              Serial.println("lightOff");
              Serial.println(" ");
              Serial.println("***************");
              //lightOffOnce=1;
            //}
      }
   if((Current_Time - previousLightMilis > lightRunningMilis) && (Current_Time -previousLightMilis > lightIntervalMilis+lightRunningMilis)){
      lightOnOnce=0;
      lightOffOnce=0;
      previousLightMilis = Current_Time;
      }
    }
    else{
      scheduling=false;
      if(ledResetOnce==0){
        digitalWrite(LED1,LOW);
        digitalWrite(LED2,LOW);
        ledResetOnce=1;
        }
      }
      if(pumpRunningMilis>=1000){
        pumpScheduling=true;
        pumpResetOnce=0;
      if(Current_Time -previousPumpMilis <= pumpRunningMilis){
      if(pumpOnOnce==0){
      //digitalWrite(pump,HIGH);
      schedulePumpOn=true;
      Serial.println("***************");
      Serial.println(" ");
      Serial.println("pumpOn");
      Serial.println(" ");
      Serial.println("***************");
      pumpOnOnce=1;
      }
    //previousPumpMilis = Current_Time;
    }
    if((Current_Time - previousPumpMilis > pumpRunningMilis) && (Current_Time -previousPumpMilis <= pumpIntervalMilis+pumpRunningMilis)){
            if(pumpOffOnce==0){
              //digitalWrite(pump,LOW);
              schedulePumpOn=false;
            Serial.println("***************");
      Serial.println(" ");
      Serial.println("pumpOff");
      Serial.println(" ");
      Serial.println("***************");
      pumpOffOnce=1;
            }
      }
   if((Current_Time - previousPumpMilis > pumpRunningMilis) && (Current_Time -previousPumpMilis > pumpIntervalMilis+pumpRunningMilis)){
      pumpOnOnce=0;
      pumpOffOnce=0;
      previousPumpMilis = Current_Time;
      }
      }
      else{
        pumpScheduling=false;
        if(pumpResetOnce==0){
        digitalWrite(pump,LOW);
        pumpResetOnce=1;
        }
        }
    if(rotorRunningMilis >= 1000){
      rotorScheduling=true;
      rotorResetOnce=0;
    if(Current_Time -previousRotorMilis <= rotorRunningMilis){
      if(rotorOnOnce==0){
       scheduleRotorOn=true;
       //rotorOn=true;
      Serial.println("***************");
      Serial.println(" ");
      Serial.println("rotorOn");
      Serial.println(" ");
      Serial.println("***************");
      rotorOnOnce=1;
      }
    //previousRotorMilis = Current_Time;
    }
    if((Current_Time - previousRotorMilis > rotorRunningMilis) && (Current_Time -previousRotorMilis <= rotorIntervalMilis+rotorRunningMilis)){
            if(rotorOffOnce==0){
              scheduleRotorOn=false;
               //rotorOn=false;
            Serial.println("***************");
      Serial.println(" ");
      Serial.println("rotorOff");
      Serial.println(" ");
      Serial.println("***************");
            rotorOffOnce=1;
            }
      }
   if((Current_Time - previousRotorMilis > rotorRunningMilis) && (Current_Time -previousRotorMilis > rotorIntervalMilis+rotorRunningMilis)){
      rotorOnOnce=0;
      rotorOffOnce=0;
      previousRotorMilis = Current_Time;
      }
    }
    else{
      rotorScheduling=false;
      if(rotorResetOnce==0){
        rotorOn=false;
        rotorResetOnce=1;
        }
      }
   
  if((scheduling==true)&&(overrideScheduling==false)){ 
  if((finalSchedLed1On==false)&&(finalSchedLed2On==false)){
      
      digitalWrite(LED1,LOW);
      digitalWrite(LED2,LOW);
    }
  
  if((finalSchedLed1On==true)&&(finalSchedLed2On==false)){
      digitalWrite(LED1,HIGH);
      digitalWrite(LED2,LOW);
    }
  if((finalSchedLed1On==true)&&(finalSchedLed2On==true)){
      digitalWrite(LED1,HIGH);
      digitalWrite(LED2,HIGH);
    }
  }
  
    if((led1ManualOn==false)&&(led2ManualOn==false)){
      if(ledOnce==0){
      digitalWrite(LED1,LOW);
      digitalWrite(LED2,LOW);
      ledOnce=1;
      }
    }
  
  if((led1ManualOn==true)&&(led2ManualOn==false)){
      digitalWrite(LED1,HIGH);
      digitalWrite(LED2,LOW);
      ledOnce=0;
    }
  if((led1ManualOn==true)&&(led2ManualOn==true)){
      digitalWrite(LED1,HIGH);
      digitalWrite(LED2,HIGH);
      //ledOnce=0;
    }
    
    if((pumpScheduling==true)&&(overrideSchedulingPump==false)){ 
  
  
  if(schedulePumpOn==true){
      digitalWrite(pump,HIGH);
      
    }
    if(schedulePumpOn==false){
      digitalWrite(pump,LOW);
      
    }
  
  }
  
    
  
  
  if(manualPumpOn==true){
      digitalWrite(pump,HIGH);
      pumpOnce=0;
      
    }
    if(manualPumpOn==false){
      if(pumpOnce==0){
      digitalWrite(pump,LOW);
      pumpOnce=1;
      }
      
    }
     if((rotorScheduling==true)&&(overrideSchedulingRotor==false)){ 
  
  
  if(scheduleRotorOn==true){
      rotorOn=true;
      
    }
    if(scheduleRotorOn==false){
      rotorOn=false;
      
    }
  
  }
  
    
  
  
  if(manualRotorOn==true){
      rotorOn=true;
      rotorOnce=0;
      
    }
    if(manualRotorOn==false){
      if(rotorOnce==0){
      rotorOn=false;
      rotorOnce=1;
      }
      
    
    }
  }
  } 
}

//Task2code: blinks an LED every 700 ms
void Task2code( void * pvParameters ){
  //Serial.print("Task2 running on core ");
  //Serial.println(xPortGetCoreID());

  for(;;){
    if(rotorOn==true){
      stepperOn();
    }
    else{
      stepperOff();
      }
  }
}

void Detect_Rising_Edge ()
{ 
   Pulse_Count++;
}
void count_function()
{
    counts++;
}
void stepperOn(){
  digitalWrite(STEP_PIN, HIGH);
  delay(2);
  digitalWrite(STEP_PIN, LOW);
  delay(2);
  }
void stepperOff(){
  digitalWrite(STEP_PIN, LOW);
  
  }
  
void loop() {   
 
    
  

   }
