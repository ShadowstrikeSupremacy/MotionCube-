// 3D Motion Tracker Visualization using Processing
// Receives roll and pitch data from a microcontroller (e.g., Arduino) via Serial
// and visualizes the orientation of a 3D box in real time.

import processing.serial.*; // Import the serial library for communication
import java.awt.event.KeyEvent;
import java.io.IOException;

Serial myPort; // Serial port object
String data = ""; // Incoming serial data as a string
float roll, pitch; // Orientation angles

void setup() {
  size(960, 640, P3D); // Set up a 3D canvas
  // List all available serial ports and select the first one automatically
  String portName = Serial.list()[0]; // (Optional) For auto-detection
  myPort = new Serial(this, "COM4", 115200); // Open serial port (update COM port as needed)
  myPort.bufferUntil('\n'); // Read until newline character
}

void draw() {
  // Center the origin and clear the background
  translate(width / 2, height / 2, 0);
  background(33);
  
  // Display roll and pitch values
  textSize(22);
  fill(255);
  text("Roll: " + int(roll) + "     Pitch: " + int(pitch), -100, 265);

  // Rotate the 3D box based on received roll and pitch values
  rotateX(radians(roll));  // Roll: rotation around X-axis
  rotateZ(radians(-pitch)); // Pitch: rotation around Z-axis (negated for correct direction)

  // Draw the 3D box representing the tracked object
  fill(0, 76, 153);
  box(200, 40, 386);
  
  // Display project title
  textSize(25);
  fill(255);
  text("Motion Tracker", -85, 10, 200);
}

// This function is called automatically whenever new serial data is available
void serialEvent(Serial myPort) {
  data = myPort.readStringUntil('\n'); // Read data until newline
  if (data != null) {
    data = trim(data); // Remove whitespace
    String items[] = split(data, '/'); // Split data by '/': expected format "roll/pitch"
    if (items.length > 1) {
      // Smoothly interpolate to new values for smooth animation
      roll = lerp(roll, float(items[0]), 0.1); // Smooth the movement
      pitch = lerp(pitch, float(items[1]), 0.1);
    }
  }
}