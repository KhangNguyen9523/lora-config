#include <HardwareSerial.h>

// Define pins for M0, M1, and AUX
#define M0_PIN 17  // GPIO 17
#define M1_PIN 16  // GPIO 16
#define AUX_PIN 5  // GPIO 5

// Create a serial instance for the LoRa module
HardwareSerial LoRaSerial(1); // Use UART1

// Function to enter configuration mode
void enterConfigMode() {
  digitalWrite(M0_PIN, HIGH);
  digitalWrite(M1_PIN, HIGH);
  delay(10); // Wait for mode change
}

// Function to enter normal mode
void enterNormalMode() {
  digitalWrite(M0_PIN, LOW);
  digitalWrite(M1_PIN, LOW);
  delay(10); // Wait for mode change
}

// Send AT command and wait for response
void sendATCommand(const String &command) {
  Serial.print("Sending: "); Serial.println(command); // Log command
  LoRaSerial.print(command);
  delay(100); // Allow the module to process
  while (LoRaSerial.available()) {
    Serial.write(LoRaSerial.read()); // Print response to Serial Monitor
  }
  Serial.println(); // Newline for better readability
}

void setup() {
  // Start Serial for debugging
  Serial.begin(9600);

  // Start LoRaSerial at 9600
  LoRaSerial.begin(9600, SERIAL_8N1, 19, 18); // TX=GPIO19, RX=GPIO18

  // Configure pins
  pinMode(M0_PIN, OUTPUT);
  pinMode(M1_PIN, OUTPUT);
  pinMode(AUX_PIN, INPUT);

  // Enter configuration mode
  enterConfigMode();
  Serial.println("Entered Configuration Mode");

  // Load default factory configuration
  sendATCommand("AT+DEFAULT\r\n");
  delay(500); // Give some time for the reset to complete
  Serial.println("Loaded Factory Default Configuration");

  // Set UART baud rate, parity, and stop bits (9600 baud, no parity, 1 stop bit)
  sendATCommand("AT+UART=9600,0,1\r\n");

  // Set air data rate (e.g., 2.4kbps)
  sendATCommand("AT+AIR=24\r\n");

  // Set transmission power (maximum)
  sendATCommand("AT+PWR=4\r\n");

  // Set channel (example: channel 77)
  sendATCommand("AT+CHAN=4D\r\n"); // Channel in hex (4D = 77 in decimal)

  // Set address (example: 0001)
  sendATCommand("AT+ADDR=0001\r\n");

  // Save settings and exit configuration mode
  sendATCommand("AT+WRITE\r\n");
  enterNormalMode();
  Serial.println("Configuration Complete, Entered Normal Mode");
}

void loop() {
  // LoRa communication can happen here
}
