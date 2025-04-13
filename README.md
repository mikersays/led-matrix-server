# Wi-Fi LED Matrix Scroller for Arduino UNO R4 WiFi

This Arduino sketch runs on the **Arduino UNO R4 WiFi**. It connects to a Wi-Fi network and displays scrolling messages on the onboard LED matrix. You can update the message using a web interface hosted on the board.

## Features

- Connects to Wi-Fi using credentials stored in `arduino_secrets.h`.
- Displays the device's IP address on the onboard LED matrix after connecting.
- Hosts a simple web page for sending custom messages to the LED matrix.
- Scrolls messages across the matrix display.

## Hardware Requirements

- **Arduino UNO R4 WiFi**
- Built-in 12x8 LED matrix
- USB cable for programming and power

## Required Libraries

Install the following libraries from the Arduino Library Manager:

- `WiFiS3`: For Wi-Fi networking
- `ArduinoGraphics`: For drawing text on the LED matrix
- `Arduino_LED_Matrix`: For controlling the onboard LED matrix

## Setup Instructions

1. **Create a secrets file**

   Create a file named `arduino_secrets.h` in your sketch directory with the following content:

   ```cpp
   #define SECRET_SSID "<your_wifi_ssid>"
   #define SECRET_PASS "<your_wifi_password>"
   ```

2. **Upload the sketch**

   Use the Arduino IDE to upload the sketch to your Arduino UNO R4 WiFi.

3. **Monitor the Serial Output**

   Open the Serial Monitor at 115200 baud. Once connected, the board prints the assigned IP address:

   ```
   Connected! IP Address: 192.168.1.42
   ```

4. **Send a Message**

   - Open a web browser and visit the IP address displayed in the Serial Monitor or on the board's LED matrix.
   - Use the web form to submit a message.
   - The message will scroll across the LED matrix.


## License

Open-source under the MIT License. You are free to modify and redistribute this code.
