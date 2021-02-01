#include <SPI.h>
#include <Ethernet.h>
#include <SoftwareSerial.h>

//two Arduino pins to use for software serial
int RXPin = 3;
int TXPin = 2;

//Default baud of NEO-6M is 9600
int GPSBaud = 9600;

//Create a software serial port called "gpsSerial"
SoftwareSerial gpsSerial(RXPin, TXPin);

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = {
  0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02
};
byte server[] = {192, 168, 83, 150};
int serverPort = 5000;

EthernetClient tcpClient;

const unsigned short gpsBufferSize = 512;
char gpsBuffer[gpsBufferSize];
unsigned short receivedCharCount = 0;

void setup() {
  // You can use Ethernet.init(pin) to configure the CS pin
  //Ethernet.init(10);  // Most Arduino shields
  //Ethernet.init(5);   // MKR ETH shield
  //Ethernet.init(0);   // Teensy 2.0
  //Ethernet.init(20);  // Teensy++ 2.0
  //Ethernet.init(15);  // ESP8266 with Adafruit Featherwing Ethernet
  //Ethernet.init(33);  // ESP32 with Adafruit Featherwing Ethernet
  
  // Start the software serial port at the GPS's default baud
  gpsSerial.begin(GPSBaud);

  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  //while (!Serial) {
    //; // wait for serial port to connect. Needed for native USB port only
  //}

  // start the Ethernet connection:
  //Serial.println("Initialize Ethernet with DHCP:");
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    } else if (Ethernet.linkStatus() == LinkOFF) {
      Serial.println("Ethernet cable is not connected.");
    }
    // no point in carrying on, so do nothing forevermore:
    while (true) {
      delay(1);
    }
  }
  // print your local IP address:
  Serial.print("My IP address: ");
  Serial.println(Ethernet.localIP());

  Serial.println("connecting...");
  
  if (tcpClient.connect(server, serverPort)) {
    Serial.println("connected");
  } else {
    Serial.println("connection failed!");
  }

  memset(gpsBuffer, 0, gpsBufferSize);
}

bool connectToServer() {
  if (tcpClient.connect(server, serverPort)) {
    return true;
  } else {
    return false;
  }
}

void loop() {
  // Displays information when new sentence is available.
  receivedCharCount = 0;
  while (gpsSerial.available() > 0) {
    gpsBuffer[receivedCharCount++] = gpsSerial.read();
  }
  
  if (receivedCharCount > 0) {
    if (tcpClient.connected()) {
      tcpClient.write(gpsBuffer, receivedCharCount-1);
    }
    else 
    {
      Serial.write(gpsBuffer, receivedCharCount-1);
      Serial.println();
      connectToServer();
    }
  }

  switch (Ethernet.maintain()) {
    case 1:
      //renewed fail
      Serial.println("Error: renewed fail");
      break;

    case 2:
      //renewed success
      Serial.println("Renewed success");
      //print your local IP address:
      Serial.print("My IP address: ");
      Serial.println(Ethernet.localIP());
      break;

    case 3:
      //rebind fail
      Serial.println("Error: rebind fail");
      break;

    case 4:
      //rebind success
      Serial.println("Rebind success");
      //print your local IP address:
      Serial.print("My IP address: ");
      Serial.println(Ethernet.localIP());
      break;

    default:
      delay(500);
      break;
  }
}
