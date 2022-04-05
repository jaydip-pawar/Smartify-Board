#ifndef MY_EEPROM_H
#define MY_EEPROM_H
#include <EEPROM.h>
#define LED 5			// TODO: Change Pairing LED Pin as needed.

class MY_EEPROM {
	
	public:

		MY_EEPROM();
	
		String readSsid();				// Read SSID from EEPROM
		
		String readPass();				// Read Password from EEPROM
		
		String readUid();				// Read User ID from EEPROM

		String readStatus();
		
		String splitString(String data, char separator, int index);					// Split SSID, Password and User ID from single String
		
		bool updateIdPassword(String qsid, String qpass, String uid);				// Update SSID, Password and User ID in EEPROM
		
		void updateStatus(String status);

		void clearEEPROM();				// Clear the already existing SSID, Password and User ID

		void pairing();					// Blink the LED

};

#endif
