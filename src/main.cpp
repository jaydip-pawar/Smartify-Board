#include <WiFi.h>
#include <Adafruit_MCP23X17.h>
#include <ESP32Ping.h>
#include <BluetoothSerial.h>
#include <my_eeprom.h>
#include <my_firebase.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

#define DATABASE_URL "smartify-9679b-default-rtdb.firebaseio.com"
#define API_KEY "AIzaSyBXZh3I-uLxBWrwU6YBOJ7A2IQ9Jcpbloc"
#define USER_EMAIL "pawarservice@pp.com"
#define USER_PASSWORD "PP@SERVICE@1234"
#define FIREBASE_PROJECT_ID "smartify-9679b"

#define relay1 1
#define relay2 2
#define relay3 3
#define relay4 4
#define relay5 5
#define relay6 6
#define relay7 7
#define relay8 8

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

const char *host = "httpbin.org";

Adafruit_MCP23X17 mcp;

FirebaseData firebaseData;
FirebaseData stream;
FirebaseAuth auth;
FirebaseConfig config;

BluetoothSerial SerialBT;
TaskHandle_t Task1;
MY_EEPROM eeprom;
MY_FIREBASE mf;

bool firstTime = true;

// Variables for button status
bool button1_status = false;
bool button2_status = false;
bool button3_status = false;
bool button4_status = false;
bool button5_status = false;
bool button6_status = false;
bool button7_status = false;
bool button8_status = false;

bool isButton1 = false;
bool isButton2 = false;
bool isButton3 = false;
bool isButton4 = false;
bool isButton5 = false;
bool isButton6 = false;
bool isButton7 = false;
bool isButton8 = false;

// button Declaration
int bPair = 4;
int button1 = 13;
int button2 = 14;
int button3 = 15;
int button4 = 16;
int button5 = 17;
int button6 = 18;
int button7 = 19;
int button8 = 23;

String cmd = "", id = "", password = "", uid = "", status = "";
int pairBtouched = 0;

bool offline = false;
bool isFromBoard = false;

int currentState; // the current reading from the input pin
unsigned long pressedTime = 0;

IRAM_ATTR void button1Touch()
{
  button1_status = !button1_status;
  button1 = true;
  isFromBoard = true;
}

IRAM_ATTR void button2Touch()
{
  Serial.printf("before button2: ");
  Serial.println(button2_status);
  button2_status = !button2_status;
  Serial.printf("after button2: ");
  Serial.println(button2_status);
  button2 = true;
  isFromBoard = true;
}

IRAM_ATTR void button3Touch()
{
  button3_status = !button3_status;
  button3 = true;
  isFromBoard = true;
}

IRAM_ATTR void button4Touch()
{
  button4_status = !button4_status;
  button4 = true;
  isFromBoard = true;
}

IRAM_ATTR void button5Touch()
{
  button5_status = !button5_status;
  button5 = true;
  isFromBoard = true;
}

IRAM_ATTR void button6Touch()
{
  button6_status = !button6_status;
  button6 = true;
  isFromBoard = true;
}

IRAM_ATTR void button7Touch()
{
  button7_status = !button7_status;
  button7 = true;
  isFromBoard = true;
}

IRAM_ATTR void button8Touch()
{
  button8_status = !button8_status;
  button8 = true;
  isFromBoard = true;
}

bool testWifi(void)
{
  int c = 0;
  while (c < 20)
  {
    if (WiFi.status() == WL_CONNECTED)
    {
      return true;
    }
    delay(500);
    Serial.print("*");
    c++;
  }
  return false;
}

bool state(String value) {
  if(value == "true") {
    return true;
  }
  return false;
}

void streamCallback(MultiPathStream stream)
{
  size_t numChild = sizeof(mf.childPath) / sizeof(mf.childPath[0]);

  for (size_t i = 0; i < numChild; i++)
  {
    if (stream.get(mf.childPath[i]))
    {
      if(!firstTime) {
        if (stream.dataPath == "/button1")
        {
          Serial.printf("board: ");
          Serial.println(button1_status);
          Serial.printf("stream: ");
          Serial.println(state(stream.value));
          if(button1_status != state(stream.value)) {
            button1_status = stream.value == "true";
            mcp.digitalWrite(relay1, button1_status);
          }
        }

        else if (stream.dataPath == "/button2")
        {
          if(button2_status != state(stream.value)) {
            button2_status = stream.value == "true";
            mcp.digitalWrite(relay2, button2_status);
          }
        }

        else if (stream.dataPath == "/button3")
        {
          if(button3_status != state(stream.value)) {
            button3_status = stream.value == "true";
            mcp.digitalWrite(relay3, button3_status);
          }
        }

        else if (stream.dataPath == "/button4")
        {
          if(button4_status != state(stream.value)) {
            button4_status = stream.value == "true";
            mcp.digitalWrite(relay4, button4_status);
          }
        }

        else if (stream.dataPath == "/button5")
        {
          if(button5_status != state(stream.value)) {
            button5_status = stream.value == "true";
            mcp.digitalWrite(relay5, button5_status);
          }
        }

        else if (stream.dataPath == "/button6")
        {
          if(button6_status != state(stream.value)) {
            button6_status = stream.value == "true";
            mcp.digitalWrite(relay6, button6_status);
          }
        }

        else if (stream.dataPath == "/button7")
        {
          if(button7_status != state(stream.value)) {
            button7_status = stream.value == "true";
            mcp.digitalWrite(relay7, button7_status);
          }
        }

        else if (stream.dataPath == "/button8")
        {
          if(button8_status != state(stream.value)) {
            button8_status = stream.value == "true";
            mcp.digitalWrite(relay8, button8_status);
          }
        }
      } else {
        firstTime = false;
        if (stream.dataPath == "/button1")
        {
            button1_status = stream.value == "true";
            mcp.digitalWrite(relay1, button1_status);
        }

        else if (stream.dataPath == "/button2")
        {
            button2_status = stream.value == "true";
            mcp.digitalWrite(relay2, button2_status);
        }

        else if (stream.dataPath == "/button3")
        {
            button3_status = stream.value == "true";
            mcp.digitalWrite(relay3, button3_status);
        }

        else if (stream.dataPath == "/button4")
        {
            button4_status = stream.value == "true";
            mcp.digitalWrite(relay4, button4_status);
        }

        else if (stream.dataPath == "/button5")
        {
            button5_status = stream.value == "true";
            mcp.digitalWrite(relay5, button5_status);
        }

        else if (stream.dataPath == "/button6")
        {
            button6_status = stream.value == "true";
            mcp.digitalWrite(relay6, button6_status);
        }

        else if (stream.dataPath == "/button7")
        {
            button7_status = stream.value == "true";
            mcp.digitalWrite(relay7, button7_status);
        }

        else if (stream.dataPath == "/button8")
        {
            button8_status = stream.value == "true";
            mcp.digitalWrite(relay8, button8_status);
        }
      }
    }
  }
}

void streamTimeoutCallback(bool timeout)
{
  if (timeout)
    Serial.println("stream timeout, resuming...\n");

  if (!stream.httpConnected())
    Serial.printf("error code: %d, reason: %s\n\n", stream.httpCode(), stream.errorReason().c_str());
}

bool updateData() {
    bool result = mf.updateData(uid, button1_status, button2_status, button3_status, button4_status, button5_status, button6_status, button7_status, button8_status);
    return result;
  }

void setup()
{
  Serial.begin(115200);
  // button declaration 
  pinMode(bPair, INPUT_PULLUP);
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  pinMode(button3, INPUT_PULLUP);
  pinMode(button4, INPUT_PULLUP);
  pinMode(button5, INPUT_PULLUP);
  pinMode(button6, INPUT_PULLUP);
  pinMode(button7, INPUT_PULLUP);
  pinMode(button8, INPUT_PULLUP);

  // button click listeners
  attachInterrupt(digitalPinToInterrupt(button1), button1Touch, RISING);
  attachInterrupt(digitalPinToInterrupt(button2), button2Touch, RISING);
  attachInterrupt(digitalPinToInterrupt(button3), button3Touch, RISING);
  attachInterrupt(digitalPinToInterrupt(button4), button4Touch, RISING);
  attachInterrupt(digitalPinToInterrupt(button5), button5Touch, RISING);
  attachInterrupt(digitalPinToInterrupt(button6), button6Touch, RISING);
  attachInterrupt(digitalPinToInterrupt(button7), button7Touch, RISING);
  attachInterrupt(digitalPinToInterrupt(button8), button8Touch, RISING);

  if (!mcp.begin_I2C()) {
    Serial.println("Error.");
    while (1);
  } else {
    Serial.println("Started.");
  }
  mcp.pinMode(relay1, OUTPUT);
  mcp.pinMode(relay2, OUTPUT);
  mcp.pinMode(relay3, OUTPUT);
  mcp.pinMode(relay4, OUTPUT);
  mcp.pinMode(relay5, OUTPUT);
  mcp.pinMode(relay6, OUTPUT);
  mcp.pinMode(relay7, OUTPUT);
  mcp.pinMode(relay8, OUTPUT);

  id = eeprom.readSsid();
  password = eeprom.readPass();
  uid = eeprom.readUid();
  status = eeprom.readStatus();

  Serial.println(id);
  Serial.println(password);
  Serial.println(status);

  if (status == "true")
  {
    SerialBT.begin("SMARTIFY");
  }
  else
  {
    WiFi.begin(id.c_str(), password.c_str());

    if (testWifi())
    {
      Serial.println("Succesfully Connected!!!");
    }
    else
    {
      Serial.println("Not connected!!!");
    }

    config.api_key = API_KEY;

    auth.user.email = USER_EMAIL;
    auth.user.password = USER_PASSWORD;

    config.database_url = DATABASE_URL;
    config.token_status_callback = tokenStatusCallback;

    Firebase.begin(&config, &auth);

    if(Firebase.ready()) {
      Serial.println("ready");
      mf.updateCloud(uid);
    }

    // Stream data

    String parentPath = "/";
    parentPath += uid;
    parentPath += "/";
    parentPath += mf.boardID;

    if (!Firebase.RTDB.beginMultiPathStream(&stream, parentPath.c_str()))
      Serial.printf("sream begin error, %s\n\n", stream.errorReason().c_str());

    Firebase.RTDB.setMultiPathStreamCallback(&stream, streamCallback, streamTimeoutCallback);

    Firebase.reconnectWiFi(true);
  }
}

void loop()
{
    if(button1) {
      Serial.printf("button1: ");
      Serial.println(button1_status);
      mcp.digitalWrite(relay1, button1_status);
      button1 = false;
    }

    if(button2) {
      Serial.printf("button2: ");
      Serial.println(button2_status);
      mcp.digitalWrite(relay2, button2_status);
      button2 = false;
    }

    if(button3) {
      Serial.printf("button3: ");
      Serial.println(button3_status);
      mcp.digitalWrite(relay3, button3_status);
      button3 = false;
    }

    if(button4) {
      Serial.printf("button4: ");
      Serial.println(button4_status);
      mcp.digitalWrite(relay4, button4_status);
      button4 = false;
    }

    if(button5) {
      Serial.printf("button5: ");
      Serial.println(button5_status);
      mcp.digitalWrite(relay5, button5_status);
      button5 = false;
    }

    if(button6) {
      Serial.printf("button6: ");
      Serial.println(button6_status);
      mcp.digitalWrite(relay6, button6_status);
      button6 = false;
    }

    if(button7) {
      Serial.printf("button7: ");
      Serial.println(button7_status);
      mcp.digitalWrite(relay7, button7_status);
      button7 = false;
    }

    if(button8) {
      Serial.printf("button8: ");
      Serial.println(button8_status);
      mcp.digitalWrite(relay8, button8_status);
      button8 = false;
    }
  
  if (status == "true")
  {
    eeprom.pairing();
    if (millis() >= pressedTime + 300000)
    {
      eeprom.updateStatus("false");
      ESP.restart();
    }
    if (SerialBT.available())
    {
      SerialBT.print("Devicename:ESP32");
      String myData = SerialBT.readString();

      id = eeprom.splitString(myData, '\n', 0);
      password = eeprom.splitString(myData, '\n', 1);
      uid = eeprom.splitString(myData, '\n', 2);

      if (eeprom.updateIdPassword(id, password, uid))
      {
        // SerialBT.print("BoardName:"+mf.boardID);
        eeprom.updateStatus("false");
      }
    }
  }
  else
  {
    currentState = digitalRead(bPair);

    if (currentState == HIGH)
    {
      if (pairBtouched == 0)
      {
        pressedTime = millis();
        pairBtouched = 1;
      }
      unsigned long fixVal = pressedTime + 5000;
      if (fixVal < millis())
      {
        pressedTime = millis();
        eeprom.updateStatus("true");
      }
    } else {
      pairBtouched = 0;
    }

    if (WiFi.status() == WL_CONNECTED) {
      if(isFromBoard) {
        if(Ping.ping("www.google.com", 1)) {
          if(updateData()) {
            isFromBoard = false;
            offline = false;
          }
        } else {
          isFromBoard = false;
          offline = true;
        }
      }

      if(offline) {
        if(Ping.ping("www.google.com", 1)) {
          if(updateData()) {
            offline = false;
          }
        }
      }
    }
  }
}