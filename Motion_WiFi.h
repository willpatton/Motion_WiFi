/**
 * Motion_WiFi.h
 * @author:   Will Patton 
 * @url:      http://willpatton.com 
 * @license:  MIT License
 */

#ifndef __MOTION_WIFI_H
#define __MOTION_WIFI_H

#include <Arduino.h>
#include <SPI.h>
#include <WiFi101.h>

#define WIFI true  
#define SIZE_SSID 33 
extern char ssid[SIZE_SSID]; 

class CWiFi {

public:
	//prototypes
	CWiFi();
	void setup_wifi();
	void loop_wifi();
	void reset_wifi();
	int get_wifi_status();
	void printWiFiData();
	void printCurrentNet();
	void printMacAddress();
	void scanSSID();
	void printBSSID(byte bssid[]);
	void printEncryptionType(int thisType);
	void print2Digits(byte thisByte);


private:



};




#endif