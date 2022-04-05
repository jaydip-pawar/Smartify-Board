#include "my_eeprom.h"

MY_EEPROM::MY_EEPROM() {
  pinMode(LED, OUTPUT);
}

String MY_EEPROM::readSsid() {
  EEPROM.begin(512);
  String esid = "";
  for (int i = 0; i < 32; ++i)
  {
    esid += char(EEPROM.read(i));
  }
  EEPROM.end();
  
  const char *ssid = esid.c_str();
  esid = ssid;
  return esid;
}

String MY_EEPROM::readPass() {
  EEPROM.begin(512);
  String epass = "";
  for (int i = 32; i < 64; ++i)
  {
    epass += char(EEPROM.read(i));
  }
  EEPROM.end();
  
  const char *pass = epass.c_str();
  epass = pass;
  return epass;
}

String MY_EEPROM::readUid() {
  EEPROM.begin(512);
  String uid = "";
  for (int i = 64; i < 96; ++i)
  {
    uid += char(EEPROM.read(i));
  }
  EEPROM.end();
  
  const char *UID = uid.c_str();
  uid = UID;
  return uid;
}

String MY_EEPROM::readStatus() {
  EEPROM.begin(512);
  String status = "";
  for (int i = 96; i < 110; ++i)
  {
    status += char(EEPROM.read(i));
  }
  EEPROM.end();
  
  const char *STATUS = status.c_str();
  status = STATUS;
  return status;
}

void MY_EEPROM::clearEEPROM() {
  EEPROM.begin(512);
  for (int i = 0; i < 160; ++i) 
  {
    EEPROM.write(i, 0);
    EEPROM.commit();
  }
  EEPROM.end();
}

String MY_EEPROM::splitString(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void MY_EEPROM::pairing() {
  digitalWrite(LED, HIGH);
  delay(100);
  digitalWrite(LED, LOW);
  delay(100);
}

bool MY_EEPROM::updateIdPassword(String qsid, String qpass, String uid) {
  EEPROM.begin(512);
  if (qsid.length() > 0 && qpass.length() > 0) {
    Serial.println("clearing eeprom");
    for (int i = 0; i < 96; ++i) {
      EEPROM.write(i, 0);
    }
    for (int i = 0; i < qsid.length(); ++i)
    {
      EEPROM.write(i, qsid[i]);
      Serial.print("Wrote: ");
      Serial.println(qsid[i]);
    }
    Serial.println("writing eeprom pass:");
    for (int i = 0; i < qpass.length(); ++i)
    {
      EEPROM.write(32 + i, qpass[i]);
      Serial.print("Wrote: ");
      Serial.println(qpass[i]);
    }
    for (int i = 0; i < uid.length(); ++i)
    {
      EEPROM.write(64 + i, uid[i]);
      Serial.print("Wrote: ");
      Serial.println(uid[i]);
    }
    EEPROM.commit();
    return true;
    
  } else {
    Serial.println("Sending 404");
    return false;
  }
  EEPROM.end();
}

void MY_EEPROM::updateStatus(String status) {
  EEPROM.begin(512);
  Serial.println("clearing eeprom");
  for (int i = 96; i < 110; ++i) {
    EEPROM.write(i, 0);
  }
  for (int i = 0; i < status.length(); ++i)
  {
    EEPROM.write(96 + i, status[i]);
    Serial.print("Wrote: ");
    Serial.println(status[i]);
  }
  EEPROM.commit();
  EEPROM.end();
  ESP.restart();
}
