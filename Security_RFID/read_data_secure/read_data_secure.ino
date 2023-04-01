//Libraries
#include <SPI.h>
#include <MFRC522.h>
#include <DHT.h>
#include "LiquidCrystal.h"
#include "src/dependencies/Seeed_Arduino_mbedtls/src/Seeed_mbedtls.h"
#include <WiFi.h>
//#include <WiFiMulti.h>
//#include <WiFiClientSecure.h>  //included WiFiClientSecure does not work!
#include "src/dependencies/WiFiClientSecure/WiFiClientSecure.h" //using older WiFiClientSecure
#include <time.h>
#include <PubSubClient.h>

//Constants
#define SS_PIN 5
#define RST_PIN 27
#define DHT_SENSOR_PIN  17 // ESP32 pin GIOP21 connected to DHT11 sensor
#define DHT_SENSOR_TYPE DHT11
#define HOSTNAME "mqtt_test"

float humi;
float tempC;
float tempF;

boolean sub = false;
//Variables
const char ssid[] = "";
const char pass[] = "";
const char *MQTT_HOST = "";
const int MQTT_PORT = 8883;
const char *MQTT_USER = ""; // leave blank if no credentials used//errore qui
const char *MQTT_PASS = ""; // leave blank if no credentials used/
char password[8];
char user[5];

const char* local_root_ca =
  "-----BEGIN CERTIFICATE-----\n" \
  "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\n" \
  "yzLg1Q05GHbhRhVlqWUCSuuBJRa9o1e3YA==\n" \
  "-----END CERTIFICATE-----";
byte centigrade[] = {
  0x1C,
  0x14,
  0x1C,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00
};


const char MQTT_SUB_TOPIC[] = "home/" HOSTNAME "/in";
const char MQTT_PUB_TOPIC[] = "home/" HOSTNAME "/in";
WiFiClientSecure net;
PubSubClient clientPubSub(net);

time_t now;
unsigned long lastMillis = 0;
byte nuidPICC[4] = {0, 0, 0, 0};
MFRC522::MIFARE_Key key;
MFRC522 rfid = MFRC522(SS_PIN, RST_PIN);
DHT dht_sensor(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);
LiquidCrystal lcd(13, 12, 14, 25, 26, 33);//RS,E,D4,D5,D6,D7

//WiFiMulti WiFiMulti;
void decrypt(unsigned char * chipherText, char * key, unsigned char * outputBuffer){
 
  mbedtls_aes_context aes;
 
  mbedtls_aes_init( &aes );
  mbedtls_aes_setkey_dec( &aes, (const unsigned char*) key, strlen(key) * 8 );
  mbedtls_aes_crypt_ecb(&aes, MBEDTLS_AES_DECRYPT, (const unsigned char*)chipherText, outputBuffer);
  mbedtls_aes_free( &aes );
}

void mqtt_connect()
{
  while (!clientPubSub.connected()) {
    Serial.print("Time:");
    Serial.print(ctime(&now));
    Serial.print("MQTT connecting....");
    if (clientPubSub.connect(HOSTNAME, user, password)) {
      Serial.println("connected");
      clientPubSub.subscribe(MQTT_SUB_TOPIC);
      Serial.println("subscribed!");
    } else {
      Serial.print("failed, status code =");
      Serial.print(clientPubSub.state());
      Serial.println("try again in 5 seconds");
      /* Wait 5 seconds before retrying */
      delay(5000);
    }
  }

}

void receivedCallback(char* topic, byte* payload, unsigned int length) {
    lcd.clear();
  lcd.setCursor(0,0);
  String str = "";
  Serial.print("Received [");
  Serial.print(topic);
  Serial.print("]: ");
  for (int i = 0; i < length; i++) {
    str = str + (char)payload[i];
  }
  Serial.println(str);
  lcd.print(str);
  lcd.write(1);
  delay(5000);
  lcd.clear();
  Serial.println();
  Serial.println("fine");
}

void setup() {
  Serial.begin(115200);
  SPI.begin();

  String k = WiFi.macAddress();
  k.remove(2, 1);
  Serial.println(k);
  char * key = (char*)k.c_str();
  int sizePName = 4;
  int sizeCName = 16;

  int sizePPass = 7;
  int sizeCPass = 16;
  unsigned char cipherTextOutputName[sizeCName] = {char(198),char(8),char(140),char(154),char(22),char(202),char(211),char(63),char(146),char(81),char(182),char(160),char(42),char(132),char(154),char(33)}; //Cyphered Text MQTT_USER
  unsigned char decipheredTextOutputName[sizePName];
  unsigned char cipherTextOutputPass[sizeCPass] = {char(255),char(36),char(39),char(133),char(33),char(69),char(253),char(188),char(55),char(9),char(104),char(44),char(23),char(104),char(75),char(42)}; //Cyphered Text MQTT_PASS  
  unsigned char decipheredTextOutputPass[sizePPass];
  decrypt(cipherTextOutputName, key, decipheredTextOutputName);
  decrypt(cipherTextOutputPass, key, decipheredTextOutputPass);
  String pss = "";
  String usr = "";
  Serial.println("");
  Serial.println(sizeof(decipheredTextOutputPass));
  Serial.println("\n\nDeciphered text:");
  for (int i = 0; i < 7; i++) {
    pss = pss + (char)decipheredTextOutputPass[i];
  }
  //password = str1Pass;
  pss.toCharArray(password, 8);
   
  Serial.print("pass ");
  Serial.println(password);
  
  Serial.println("");
  Serial.println(sizeof(decipheredTextOutputName));
  Serial.println("\n\nDeciphered text:");
  for (int i = 0; i < 4; i++) {
    usr = usr + (char)decipheredTextOutputName[i];
    Serial.println((char)decipheredTextOutputName[i]);
  }
  usr.toCharArray(user, 5);
  Serial.print("user ");
  Serial.println(user);
  
  lcd.begin(16, 2);
  lcd.clear();
  // Turn on the backlight on LCD.
  //lcd.backlight();
  lcd.createChar(1, centigrade);
  lcd.setCursor(0,0);


  dht_sensor.begin(); // initialize the DHT sensor
  //lcd.init();

  // Turn on the backlight on LCD.
  //lcd.backlight();
  lcd.print( "Waiting Wi-fi" );
  Serial.println("stampa topic");
  Serial.println(MQTT_SUB_TOPIC);
  Serial.print("Attempting to connect to SSID: ");
  Serial.println(ssid);
  WiFi.setHostname(HOSTNAME);
  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(1000);
  }
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(ssid);

  Serial.print("Setting time using SNTP");
  configTime(-5 * 3600, 0, "pool.ntp.org", "time.nist.gov");
  now = time(nullptr);
  while (now < 1510592825) {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }



  rfid.PCD_Init();
  //Serial.print(F("Reader :"));
  rfid.PCD_DumpVersionToSerial();
  lcd.clear();
  lcd.print( "Waiting RFID!" );

}

void loop() {
  readRFID();
  if (sub) {
    sendData(tempC, humi);
  }
}



void readRFID(void ) { /* function readRFID */
  ////Read RFID card
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
  // Look for new 1 cards
  if (!rfid.PICC_IsNewCardPresent())
    return;
  // Verify if the NUID has been readed
  if (!rfid.PICC_ReadCardSerial())
    return;
  // Store NUID into nuidPICC array
  for (byte i = 0; i < 4; i++) {
    nuidPICC[i] = rfid.uid.uidByte[i];
  }
  lcd.clear();
  lcd.print("Hi!");
  lcd.setCursor(0, 1);
  lcd.print("Please wait");
  //Serial.print(F("RFID In dec: "));
  String UIDCode = printHex(rfid.uid.uidByte, rfid.uid.size);
  if (FabricAuthentication(UIDCode)) {
    lcd.clear();
    lcd.print( "Accesso consentito!" );
    humi  = dht_sensor.readHumidity();
    // read temperature in Celsius
    tempC = dht_sensor.readTemperature();
    // read temperature in Fahrenheit
    tempF = dht_sensor.readTemperature(true);

    // check whether the reading is successful or not
    if ( isnan(tempC) || isnan(tempF) || isnan(humi)) {
      Serial.println("Failed to read from DHT sensor!");
    } else {
      if (!sub) {
        if (WiFi.status() != WL_CONNECTED)
        {
          Serial.print("Checking wifi");
          while (WiFi.waitForConnectResult() != WL_CONNECTED)
          {
            WiFi.begin(ssid, pass);
            Serial.print(".");
            delay(10);
          }
          Serial.println("try connect subriber 1");
        }
        Serial.println("try connect subriber 2");
        net.setCACert(local_root_ca);
        Serial.println("setting cert");
        clientPubSub.setServer(MQTT_HOST, MQTT_PORT);
        Serial.println("setting server");
        clientPubSub.setCallback(receivedCallback);
        Serial.println("callback");
        mqtt_connect();
        sub = true;
      }
      lcd.setCursor(0, 1);
      lcd.print( "Invio i dati" );
    }
  } else {
    lcd.clear();
    lcd.print( "Accesso negato!" );
  }
  Serial.println();
  // Halt PICC
  rfid.PICC_HaltA();
  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();
}


String printHex(byte *buffer, byte bufferSize) {
  String str;
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    str.concat(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
    str.concat(String(buffer[i], HEX));
  }
  str.toUpperCase();
  return str;
}


bool FabricAuthentication(String code) {
  bool connection = false;
  do {
    const uint16_t port = 8080;
    const char * host = "192.168.43.21"; // ip or dns

    Serial.print("Connecting to ");
    Serial.println(host);

    // Use WiFiClient class to create TCP connections
    WiFiClient client;
    // bootloader_random_enable();
    if (!client.connect(host, port)) {
      Serial.println("Connection failed.");
      Serial.println("Waiting 5 seconds before retrying...");
      delay(5000);
    } else {
      connection = true;
    }
    String seed = String(random(1, 100000));
    char ar[seed.length() + 1];
    seed.toCharArray(ar, seed.length() + 1);
    char *key = ar;
    size_t keyLength = strlen(key);
    char charCode[12];
    code.substring(1, 12).toCharArray(charCode, 12);
    char *payload = charCode;
    Serial.println(charCode);
    byte hmacResult[32];
    mbedtls_md_context_t ctx;
    mbedtls_md_type_t md_type = MBEDTLS_MD_SHA256;
    lcd.clear();
    lcd.print("Hi!");
    lcd.setCursor(0, 1);
    lcd.print("Please wait.");
    const size_t payloadLength = strlen(payload);
    Serial.println("GET /api/querycode/" + seed + " HTTP/1.1\n\n");
    client.print("GET /api/querycode/" + seed + " HTTP/1.1\n\n");
    mbedtls_md_init(&ctx);
    mbedtls_md_setup(&ctx, mbedtls_md_info_from_type(md_type), 1);
    mbedtls_md_hmac_starts(&ctx, (const unsigned char *) key, keyLength);
    mbedtls_md_hmac_update(&ctx, (const unsigned char *) payload, payloadLength);
    mbedtls_md_hmac_finish(&ctx, hmacResult);
    mbedtls_md_free(&ctx);
    //Serial.print("Hash: ");
    String finalsha = "";
    for (int i = 0; i < sizeof(hmacResult); i++) {
      char str[3];

      sprintf(str, "%02x", (int)hmacResult[i]);
      finalsha.concat(str);
      Serial.print(str);
    }
    int maxloops = 0;
    lcd.clear();
    lcd.print("Hi!");
    lcd.setCursor(0, 1);
    lcd.print("Please wait..");
    String line;
    //wait for the server's reply to become available
    while (!client.available() && maxloops < 5000)
    {
      maxloops++;
      delay(1); //delay 1 msec
    }
    if (client.available() > 0)
    {
      //read back one line from the server
      line = client.readString();
      Serial.println(line);
    }
    else
    {
      Serial.println("client.available() timed out ");
    }
    lcd.clear();
    lcd.print("Hi!");
    lcd.setCursor(0, 1);
    lcd.print("Please wait...");

    Serial.println("Closing connection.");
    client.stop();
    //}
    //Serial.println("Waiting 1 seconds before restarting...");
    delay(10000);
    if (line.indexOf(finalsha) > -1) {
      return true;
    } else {
      return false;
    }
  } while (!connection);
}


void sendData(int temp, int humi) {

  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.print("Checking wifi");
    while (WiFi.waitForConnectResult() != WL_CONNECTED)
    {
      WiFi.begin(ssid, pass);
      Serial.print(".");
      delay(10);
    }
    Serial.println("connected");
  }
  else
  {
    if (!clientPubSub.connected())
    {
      mqtt_connect();
    }
    else
    {
      clientPubSub.loop();
    }
  }

  Serial.println("published ");
  String pubData = "Hum: " + String(humi) + " Temp: " + String(temp);
  char ch[pubData.length()];
  for (int i = 0; i < pubData.length(); i++) {
    ch[i] = pubData.charAt(i);
  }
  ch[pubData.length()] = '\0';
  clientPubSub.publish(MQTT_PUB_TOPIC, ch, false);
  lcd.clear();
  lcd.print( "Accesso consentito!" );
  lcd.setCursor(0, 1);
  lcd.print("Dati inviati!");

}
