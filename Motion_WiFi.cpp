/**
 * Motion_WiFi.cpp
 * @author:   Will Patton 
 * @url:      http://willpatton.com 
 * @license:  MIT License
 */

#include "Motion_WiFi.h"

//ssid
//#define SIZE_SSID 33         //SSIDs are case sensitive and up to 32 chars + null
char ssid[SIZE_SSID];          //the current ssid
char key[SIZE_SSID];           //the current key

//ssid table type - holds WIFI ssid's discoverd by scanning the network
typedef struct {int index; char ssid[SIZE_SSID]; char key[SIZE_SSID];} ssid_table_type;

//SSID & PASSWORDS
//ssid table of scanned values
#define SIZE_SSID_TABLE 25
ssid_table_type ssid_table[SIZE_SSID_TABLE]; 


//ssid table of known values 
//UNCOMMENT
#define WIFI_SECRETS  //comment this line for github
#ifndef WIFI_SECRETS
#define SIZE_SSID_TABLE_KNOWN 2
ssid_table_type ssid_table_known[SIZE_SSID_TABLE_KNOWN]{
  0,  "MySSID",       "password",
  1,  "Xfinity",      "password"
};
#endif  
#ifdef WIFI_SECRETS
  #include "secrets.h"    //hidden .gitignore
#endif

int status_wifi = WL_IDLE_STATUS;              // the WiFi radio's status
int wifi_attempts = 3;

//timers
unsigned long timer_wifi_signal;
unsigned long timer_wifi_scan;

/**
 * constructor
 */
CWiFi::CWiFi(){

}

/**
 * setup
 */
void CWiFi::setup_wifi() {
  Serial.println("Setup WiFi:");

  //Configure pins for Adafruit ATWINC1500 Feather
  //void setPins(int8_t cs, int8_t irq, int8_t rst, int8_t en = -1);
  //WiFi.setPins(/*CS*/ 8, /*irq*/ 7, /*rst*/ 4, /*en*/ 2);   //save this line for back-up troubleshooting
  WiFi.setPins(/*CS*/ 8, /*irq*/ 7, /*rst*/ 4, /*en*/ 2);

  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  // scan for networks for additional SSIDs
  Serial.println("Scanning networks...");
  scanSSID();      //serial output of the network scan


  // attempt to connect to WiFi network:
  while (status_wifi != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: "); Serial.println(ssid);
    //tft.setCursor(0,0);
    //tft.print("Waiting for WIFI SSID: "); tft.println(String(ssid)); tft.print(" "); tft.println(String(wifi_attempts));
    
    // Connect to WPA/WPA2 network:
    status_wifi = WiFi.begin(ssid, key);

    // wait n seconds per connection attempt:
    delay(1000);
    
    if(--wifi_attempts < 1){
      //tft.clr(); //clear TFT
      //tft.setCursor(0, 0);
      break;
    }
  }


  if(status_wifi == WL_CONNECTED){
    // you're connected now, so print out the data:
    Serial.println("WIFI CONNECTED");
    printCurrentNet();
    printWiFiData();
  } else {
    Serial.println("WIFI Not connected");
  }

  //timers
  timer_wifi_signal = timer_wifi_scan = millis();
}

/**
 * loop
 */
void CWiFi::loop_wifi() {

/*
  // periodically check the network
  if(millis() - timer_wifi_signal >(1000 * 3)){
    //printCurrentNet();  //serial
    //tftCurrentNet();      //tft
  }

  //periodically rescan for nearby networks
  if(millis() - timer_wifi_scan >(1000 * 10)){
    //tft.clr(); //clear screen
    tftScanSSID();
    timer_wifi_scan = millis();
  }
*/

  
}

/**
 * reset
 */
void CWiFi::reset_wifi(){
  Serial.println("** RESET WIFI: asserting hard reset pin.");
  
  //assert the reset pin and then call setup again
  digitalWrite(4, LOW); //assert
  delay(500);
  digitalWrite(4, HIGH); //release

  //try to restart wifi
  setup_wifi();
}

/**
 * status
 */
int CWiFi::get_wifi_status(){
  if(status_wifi == WL_CONNECTED){
    return true;
  } else {
    return false;
  }
}

/**
 * print data
 */
void CWiFi::printWiFiData() {
  // print WiFi address:
  IPAddress ip = WiFi.localIP();
  Serial.print("My IP Address: ");
  Serial.println(ip);

  // print MAC address:
  byte mac[6];
  WiFi.macAddress(mac);
  Serial.print("MAC address: ");
  Serial.print(mac[5], HEX);
  Serial.print(":");
  Serial.print(mac[4], HEX);
  Serial.print(":");
  Serial.print(mac[3], HEX);
  Serial.print(":");
  Serial.print(mac[2], HEX);
  Serial.print(":");
  Serial.print(mac[1], HEX);
  Serial.print(":");
  Serial.println(mac[0], HEX);

}

/**
 * print the SSID
 */
void CWiFi::printCurrentNet() {
  // 
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print the MAC address of the router you're attached to:
  byte bssid[6];
  WiFi.BSSID(bssid);
  Serial.print("BSSID: ");
  Serial.print(bssid[5], HEX);
  Serial.print(":");
  Serial.print(bssid[4], HEX);
  Serial.print(":");
  Serial.print(bssid[3], HEX);
  Serial.print(":");
  Serial.print(bssid[2], HEX);
  Serial.print(":");
  Serial.print(bssid[1], HEX);
  Serial.print(":");
  Serial.println(bssid[0], HEX);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI): ");
  Serial.println(rssi);

  // print the encryption type:
  byte encryption = WiFi.encryptionType();
  Serial.print("Encryption Type: ");
  Serial.println(encryption, HEX);
  //Serial.println();
}



/**
 * 
 * BELOW code FROM:
 * ScanNetworksAdvanced.ino 
 * 
 */

 void CWiFi::printMacAddress() {
  // the MAC address of your WiFi shield
  byte mac[6];

  // print your MAC address:
  WiFi.macAddress(mac);
  Serial.print("MAC: ");
  print2Digits(mac[5]);
  Serial.print(":");
  print2Digits(mac[4]);
  Serial.print(":");
  print2Digits(mac[3]);
  Serial.print(":");
  print2Digits(mac[2]);
  Serial.print(":");
  print2Digits(mac[1]);
  Serial.print(":");
  print2Digits(mac[0]);
}

/**
 * Scan for nearby wifi and build a table of SSID's 
 * Echo to serial port, too.
 */
void CWiFi::scanSSID() {
  
  //scan
  Serial.println("** Scan Wifi for SSID **");
  int numSsid = WiFi.scanNetworks();
  if (numSsid == -1)
  {
    //no SSID's found
    Serial.println("No SSID's found.");
    return;
  }
  Serial.print("Number of SSID's found: ");
  Serial.println(numSsid);

  //SSID
  //SAVE to TABLE
  //save the index number and name for each SSID found:
  for (int i = 0; i < numSsid; i++) {
    ssid_table[i].index = i; 
    Serial.print(i); Serial.print(") ");
    Serial.print(WiFi.SSID(i));
    if(strlen(WiFi.SSID(i)) < 1){
      Serial.print("\t");
    }
    strcpy(ssid_table[i].ssid,WiFi.SSID(i));
    Serial.print("\tSignal: ");
    Serial.print(WiFi.RSSI(i));
    Serial.print(" dBm");
    Serial.print("\tChannel: ");
    Serial.print(WiFi.channel(i));
    //byte bssid[6];
    //Serial.print("\tBSSID: ");
    //printBSSID(WiFi.BSSID(i, bssid));
    Serial.print("\tEncryption: ");
    printEncryptionType(WiFi.encryptionType(i));
    Serial.flush();
    Serial.println();
  }

  //DEBUG - display known hosts table and secrets
  Serial.println("SSID Table Known:");
  for(int i = 0; i< SIZE_SSID_TABLE_KNOWN; i++){
    Serial.print( ssid_table_known[i].index);
    Serial.print(") ");
    Serial.print(ssid_table_known[i].ssid);
    Serial.print("\t");
    Serial.print(ssid_table_known[i].key);   //PASSWORD
    Serial.println();
  }

    
  //COMPARE - if known SSID exist in scanned SSID, then assign that as current SSID
  for(int i = 0; i< SIZE_SSID_TABLE_KNOWN; i++){
    for(int j = 0; j < numSsid; j++){
      int cmp = strcmp(ssid_table_known[i].ssid, ssid_table[j].ssid);
      if(cmp == 0){
        //if exact match, then copy known ssid to current ssid
        //ssid_table_id = ssid_table_known[j].id;
        strcpy(ssid, ssid_table_known[i].ssid);
        strcpy(key,  ssid_table_known[i].key);
      }
    }
  }
  
}//end scsan


void CWiFi::printBSSID(byte bssid[]) {
  print2Digits(bssid[5]);
  Serial.print(":");
  print2Digits(bssid[4]);
  Serial.print(":");
  print2Digits(bssid[3]);
  Serial.print(":");
  print2Digits(bssid[2]);
  Serial.print(":");
  print2Digits(bssid[1]);
  Serial.print(":");
  print2Digits(bssid[0]);
}

void CWiFi::printEncryptionType(int thisType) {
  // read the encryption type and print out the name:
  switch (thisType) {
    case ENC_TYPE_WEP:
      Serial.print("WEP");
      break;
    case ENC_TYPE_TKIP:
      Serial.print("WPA");
      break;
    case ENC_TYPE_CCMP:
      Serial.print("WPA2");
      break;
    case ENC_TYPE_NONE:
      Serial.print("None");
      break;
    case ENC_TYPE_AUTO:
      Serial.print("Auto");
      break;
  }
}



void CWiFi::print2Digits(byte thisByte) {
  if (thisByte < 0xF) {
    Serial.print("0");
  }
  Serial.print(thisByte, HEX);
}
