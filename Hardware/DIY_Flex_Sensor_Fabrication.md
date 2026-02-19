#  DIY Flex Sensor Fabrication

## Overview

Commercial flex sensors used for gesture recognition are often expensive and exhibit inconsistent threshold values across different units. To overcome this limitation and to make the system economically viable, a self-fabricated flex sensor was designed and developed using low-cost materials.

The fabricated sensor works on the principle that the electrical resistance of a conductive layer varies when subjected to bending. These resistance variations are utilized to detect finger movements in the smart glove system.


## Materials Used

The following materials were used for the fabrication of the DIY flex sensors:

- Paper strip (base conductive layer)  
- Flexible cardboard / plastic strip (substrate support)  
- Aluminium foil (electrode layer)  
- Graphite coating (pencil coated layer)  
- Connecting wires  
- Silicone / adhesive glue (for encapsulation)  


## Fabrication Procedure

The fabrication of the flex sensor was carried out in the following steps:

### Step 1: Material Preparation  
A strip of paper along with two flexible cardboard/plastic strips were cut according to the approximate length of a human finger to ensure comfortable mounting on the glove.

### Step 2: Conductive Layer Formation  
Both sides of the paper strip were coated uniformly with graphite using a pencil to form a resistive conductive layer. This layer acts as the sensing medium whose resistance changes with bending.

### Step 3: Electrode Preparation  
Aluminium foil pieces were fixed onto the cardboard strips to function as electrodes for electrical contact with the graphite layer.

### Step 4: Electrical Connection  
Connecting wires were attached to the aluminium foil electrodes using adhesive or solder to establish electrical continuity for signal transmission.

### Step 5: Encapsulation  
The assembled layers were sealed using silicone or glue to protect the sensor from mechanical damage and environmental factors.

### Step 6: Sensor Replication  
The above procedure was repeated to fabricate five flex sensors corresponding to the five fingers of the glove.



## Working Principle

When the sensor is in a straight position, the conductive graphite layer offers minimum electrical resistance.

Upon bending:

- The conductive path length increases  
- Contact area between graphite layers decreases  
- Electrical resistance increases proportionally  

This change in resistance is converted into an analog voltage signal using a voltage divider circuit, which is then processed by the ESP32 microcontroller for gesture recognition.


## Cost Comparison

Commercial Flex Sensor - ₹800 
DIY Flex Sensor - ₹40 

The proposed self-fabricated sensor significantly reduces the overall system cost while maintaining satisfactory performance for real-time gesture detection.


## Research Contribution

A self-fabricated low-cost flex sensor was developed using a graphite conductive layer and aluminium foil electrodes to reduce system cost and improve accessibility of wearable assistive communication devices.
