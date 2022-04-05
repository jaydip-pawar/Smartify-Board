#include "my_firebase.h"

MY_FIREBASE::MY_FIREBASE() {
  MY_EEPROM eeprom;
  uid = eeprom.readUid();
}

void MY_FIREBASE::updateCloud(String uid) {
  FirebaseJson query;
  
  documentPath = "boards/" + uid + "/" + boardID + "/boardData";
  
  query.set("fields/id/stringValue", uid);
  query.set("fields/name/stringValue", boardID);
  query.set("fields/buttons/integerValue", nButton);
  query.set("fields/sockets/integerValue", nSocket);
  query.set("fields/fan_regulator/integerValue", fRegulator);
  query.set("fields/paired/booleanValue", true);
  
  if (Firebase.Firestore.patchDocument(&firebaseData, FIREBASE_PROJECT_ID, "", documentPath.c_str(), query.raw(), "id,name,buttons,sockets,fan_regulator,paired")) {
    Serial.printf("ok\n%s\n\n", firebaseData.payload().c_str());
    Serial.println("done");
  } else {
    Serial.println( firebaseData.errorReason());
    Serial.println("error");
  }
}


bool MY_FIREBASE::updateData(String uid, bool button1, bool button2, bool button3, bool button4, bool button5, bool button6, bool button7, bool button8) {
	
	path = "/";
	path += uid;
	
	FirebaseJson json;
	json.set(boardID + "/button1", button1);
	json.set(boardID + "/button2", button2);
	json.set(boardID + "/button3", button3);
	json.set(boardID + "/button4", button4);
	json.set(boardID + "/button5", button5);
	json.set(boardID + "/button6", button6);
	json.set(boardID + "/button7", button7);
	json.set(boardID + "/button8", button8);
	if(Firebase.RTDB.setJSON(&firebaseData, path.c_str(), &json)) {
		return true;
	} else 
		return false;
}












