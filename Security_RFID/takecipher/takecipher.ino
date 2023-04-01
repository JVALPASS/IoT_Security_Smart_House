#include "mbedtls/aes.h"
#include <WiFi.h>
 
void encrypt(char * plainText, char * key, unsigned char * outputBuffer){
 
  mbedtls_aes_context aes;
 
  mbedtls_aes_init( &aes );
  mbedtls_aes_setkey_enc( &aes, (const unsigned char*) key, strlen(key) * 8 );
  mbedtls_aes_crypt_ecb( &aes, MBEDTLS_AES_ENCRYPT, (const unsigned char*)plainText, outputBuffer);
  mbedtls_aes_free( &aes );
}
 
void decrypt(unsigned char * chipherText, char * key, unsigned char * outputBuffer){
 
  mbedtls_aes_context aes;
 
  mbedtls_aes_init( &aes );
  mbedtls_aes_setkey_dec( &aes, (const unsigned char*) key, strlen(key) * 8 );
  mbedtls_aes_crypt_ecb(&aes, MBEDTLS_AES_DECRYPT, (const unsigned char*)chipherText, outputBuffer);
  mbedtls_aes_free( &aes );
}

void encryptP(char * plainText, char * key, unsigned char * outputBuffer){
 
  mbedtls_aes_context aes;
 
  mbedtls_aes_init( &aes );
  mbedtls_aes_setkey_enc( &aes, (const unsigned char*) key, strlen(key) * 8 );
  mbedtls_aes_crypt_ecb( &aes, MBEDTLS_AES_ENCRYPT, (const unsigned char*)plainText, outputBuffer);
  mbedtls_aes_free( &aes );
}
 
void decryptP(unsigned char * chipherText, char * key, unsigned char * outputBuffer){
 
  mbedtls_aes_context aes;
 
  mbedtls_aes_init( &aes );
  mbedtls_aes_setkey_dec( &aes, (const unsigned char*) key, strlen(key) * 8 );
  mbedtls_aes_crypt_ecb(&aes, MBEDTLS_AES_DECRYPT, (const unsigned char*)chipherText, outputBuffer);
  mbedtls_aes_free( &aes );
}
 
void setup() {
 
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  delay(2000);
  Serial.println(WiFi.macAddress().c_str());
  String k = WiFi.macAddress();
  k.remove(2, 1);
  Serial.println(k);
  char * key = (char*)k.c_str();
 
  char *plainTextName = "";
  char *plainTextPass = "";
  
  char *user;
  char *pass;
  int sizePName = 4;
  int sizeCName = 16;

  int sizePPass = 7;
  int sizeCPass = 16;
  unsigned char cipherTextOutputName[sizeCName];
  unsigned char decipheredTextOutputName[sizePName];

  unsigned char cipherTextOutputPass[sizeCPass];
  unsigned char decipheredTextOutputPass[sizePPass];
  
  encrypt(plainTextName, key, cipherTextOutputName);
  //decrypt(cipherTextOutputName, key, decipheredTextOutputName);
  
  encryptP(plainTextPass, key, cipherTextOutputPass);
  //decryptP(cipherTextOutputPass, key, decipheredTextOutputPass);
 
  Serial.println("\nOriginal plain text name:");
  Serial.println(plainTextName);

  Serial.println("\nOriginal plain text pass:");
  Serial.println(plainTextPass);
  

  Serial.println("\nCiphered text name:");
  for (int i = 0; i < sizeCName; i++) {
    Serial.println(cipherTextOutputName[i]);
  }


  Serial.println();
  Serial.println("\nCiphered text pass:");
  for (int i = 0; i < sizeCPass; i++) {
     Serial.println(cipherTextOutputPass[i]);
  }
  Serial.println();

}
 
void loop() {}
