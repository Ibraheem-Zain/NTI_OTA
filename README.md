# NTI ESP32 OTA Project

This project allows an ESP32 to receive Over-The-Air (OTA) firmware updates using an MQTT trigger and a local Node-RED HTTP server. 

### How It Works
1. **The Trigger:** A user clicks "UPDATE" on the Node-RED dashboard, which publishes an MQTT message via HiveMQ.
2. **The Execution:** The ESP32 receives the trigger, opens an HTTP stream to the local Node-RED server, and downloads the `.bin` file.
3. **The Flash:** The ESP32 writes the new firmware to the passive memory partition and reboots automatically.

### Setup Instructions
Before flashing the code to the ESP32, you must configure the following variables in `main.c`:
* `WIFI_NETWORK_NAME`: Your local presentation Wi-Fi network name (SSID).
* `WIFI_NETWORK_PASSWORD`: Your local presentation Wi-Fi password.
* `SERVER_IP`: The IPv4 address of the laptop running the Node-RED server.
* `BROKER_IP`: The IPv4 address of the MQTT broker that you use (i.e.. mqtt://mqtt-dashboard.com:1883)

### Hardware Required
* ESP32 Development Board (4MB Flash minimum)
* Node-RED environment
