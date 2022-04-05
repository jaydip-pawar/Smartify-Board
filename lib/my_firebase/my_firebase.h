#ifndef MY_FIREBASE_H
#define MY_FIREBASE_H
#include <Firebase_ESP_Client.h>
#include <my_eeprom.h>

class MY_FIREBASE {
	private:
		FirebaseData firebaseData;
		String FIREBASE_PROJECT_ID = "smartify-9679b";
		String documentPath;
		String uid;
		int nButton = 8, nSocket = 0, fRegulator = 0;
		
		String path;
		
	public:
		
		String childPath[8] = {"/button1", "/button2", "/button3", "/button4", "/button5", "/button6", "/button7", "/button8"};
		size_t childPathSize = 8;
		String boardID = "Smartify1";
		
		MY_FIREBASE();
		void updateCloud(String uid);   // Update boards data to server
		bool updateData(String uid, bool button1, bool button2, bool button3, bool button4, bool button5, bool button6, bool button7, bool button8);		// Update button status
};

#endif
