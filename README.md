# Smart-Glove-Sign-to-Speech-ESP32-DIY-Flex-Sensor

# Smart Glove for Converting Sign Language into Text and Speech

## Project Overview

Communication poses a significant difficulty for people who have hearing and speech disabilities. This initiative suggests a Smart Glove setup that transforms sign language movements into text and spoken language instantly.

The system recognizes hand movements by utilizing flex sensors embedded in a wearable glove and converts them into coherent text and voice output via a mobile app.


## Main Focus of the Project

In contrast to traditional systems that employ expensive commercial flex sensors, this project uses a **homemade DIY flex sensor** created with:

- Coating of Graphite

- Aluminum Foil

This method greatly lowers the total system expense while ensuring dependable gesture detection precision.

##  Hardware Elements Utilized

- Microcontrolador ESP32
- Homemade Flex Sensors
- Aluminum Foil
- Sheet of Graphite
- Resistors
- Linking Cables
- Electricity Source

## Software Applications Utilized

- Arduino Integrated Development Environment
- MIT App Maker
- C Programming Embedded

## Operating Principle

1. Hand gestures are identified through the use of DIY flex sensors.

2. The sensor's resistance changes when it is bent.

3. The ESP32 captures analog readings from the sensors.

4. Identified gestures are linked to specific messages.

5. The information is sent to the Android app through Bluetooth.

6. The application on the mobile device shows the relevant text.

7. The integrated Text-to-Speech system transforms the text into spoken output.

## Mobile App

The Android app is created with MIT App Inventor, which:

- Sets up Bluetooth connection with ESP32
- Obtains gesture information
- Shows text output
- Produces audio through the Text-to-Speech component
  
## Folder Organization for the Project

ESP32_Code/
Hardware/
MIT_App/
Mobile_App_APK/
Results/
Research_paper/

## Future Prospects

- Application of Machine Learning for real-time gesture identification
- Assistance for various sign languages
- Cloud data storage utilizing IoT technology
- Immediate construction of sentences

