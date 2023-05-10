# IoT_Security_Smart_House

# IoT Security Project

The importance of IoT security cannot be overstated, especially in the context of smart homes. The data collected by these devices, such as temperature and humidity levels, are critical pieces of information that could be used to determine the presence or absence of individuals in a home. Protecting the privacy and integrity of this data is of utmost importance, and our project takes significant steps to ensure that only authorized individuals have access to it.

## Project Description

This IoT security project is of paramount importance as it ensures the confidentiality and authenticity of the collected data. The project has been developed to gather data about the temperature and humidity in a house while ensuring a certain level of security, with the use of RFID authentication, blockchain technology, and encrypted communication over TLS. Unauthorized access to the data is prevented by implementing AES-256 encryption to save the credentials in the code. By ensuring the privacy and integrity of the data, this project helps to mitigate the risks associated with IoT devices and enhances the overall security of smart homes.

## Responsibilities

My role in the project was to develop and implement the IoT node, including writing the code for MQTT communication based on TLS and cryptography of credentials. I spent 70% of my time on development and 30% on implementation.

## Tasks Carried Out

- Implement hardware technology
- Connect ESP-32 board with DHT-11 sensor to gather humidity and temperature data
- Connect ESP-32 board with LCD display (I2C interface)
- Connect ESP-32 with NFC reader
- Implement code in the Blockchain using Fabric to implement a challenge-response mechanism based on hashing to ensure the correct RFID smart card for authentication
- Implement MQTT Broker Mosquitto
- Obtain certificate to implement TLS communication for encrypted data transmission
- Implement code for ESP-32 MCU:
  - Implement mechanism in the code to gather data from the humidity and temperature sensor
  - Implement mechanism in the code to send data through the Publish/Subscribe mechanism using MQTT on WiFi with TLS instead of TCP/IP
  - Implement AES-256 cryptography algorithm to encrypt username and password credentials for authentication to the MQTT Broker

## Results

The results show a robust security mechanism for gathering data in a smart house. The implemented security features ensure that only authorized individuals have access to the data collected by the IoT device, making it difficult for anyone to steal them.
