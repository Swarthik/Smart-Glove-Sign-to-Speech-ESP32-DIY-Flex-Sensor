#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled!
#endif

BluetoothSerial SerialBT;

// ========================================
// HARDWARE CONFIGURATION
// ========================================
const int FLEX_PIN_1 = 34;  // Thumb
const int FLEX_PIN_2 = 35;  // Index finger
const int FLEX_PIN_3 = 32;  // Middle finger
const int FLEX_PIN_4 = 33;  // Ring finger
const int FLEX_PIN_5 = 25;  // Little finger

// ========================================
// INDIVIDUAL THRESHOLD VALUES FOR EACH SENSOR
// ========================================
const int THRESHOLD_1 = 4000;   // Thumb threshold
const int THRESHOLD_2 = 4000;   // Index threshold
const int THRESHOLD_3 = 4000;   // Middle threshold
const int THRESHOLD_4 = 4000;    // Ring threshold
const int THRESHOLD_5 = 4000;   // Little threshold (INVERTED LOGIC)

// ========================================
// TIMING CONFIGURATION
// ========================================
const int SEND_INTERVAL = 300;        // Minimum time between checking gestures (300ms)
const int COOLDOWN_PERIOD = 3000;     // Time to wait after sending (3 seconds)
const int STABILIZATION_DELAY = 250;  // Reduced from 400ms to 250ms for faster detection

// ========================================
// VARIABLES
// ========================================
int flex1_Value = 0;
int flex2_Value = 0;
int flex3_Value = 0;
int flex4_Value = 0;
int flex5_Value = 0;

bool finger1_Active = false;
bool finger2_Active = false;
bool finger3_Active = false;
bool finger4_Active = false;
bool finger5_Active = false;

unsigned long lastSendTime = 0;
unsigned long lastPrintTime = 0;
unsigned long cooldownStartTime = 0;
unsigned long gestureStartTime = 0;
unsigned long lastGestureChangeTime = 0;

int lastGesture = 0;
int currentGesture = 0;
int stableGesture = 0;
bool gestureSent = false;
bool inCooldown = false;

// ========================================
// SETUP
// ========================================
void setup() {
  Serial.begin(115200);
  Serial.println("\n========================================");
  Serial.println("ESP32 FIVE FLEX SENSOR - Hand Glove");
  Serial.println("========================================");
  
  pinMode(FLEX_PIN_1, INPUT);
  pinMode(FLEX_PIN_2, INPUT);
  pinMode(FLEX_PIN_3, INPUT);
  pinMode(FLEX_PIN_4, INPUT);
  pinMode(FLEX_PIN_5, INPUT);
  
  SerialBT.begin("ESP32_HandGlove");
  
  Serial.println("✓ Bluetooth Started!");
  Serial.println("✓ Device: ESP32_HandGlove");
  Serial.println("✓ Waiting for connection...");
  Serial.println();
  Serial.println("SINGLE FINGER GESTURES:");
  Serial.println("1. Thumb  → THANK YOU");
  Serial.println("2. Index  → HELLO");
  Serial.println("3. Middle → PLEASE");
  Serial.println("4. Ring   → SORRY");
  Serial.println("5. Little → BYE");
  Serial.println();
  Serial.println("TWO FINGER GESTURES:");
  Serial.println("6.  Thumb + Index  → WATER");
  Serial.println("7.  Thumb + Middle → FOOD");
  Serial.println("8.  Thumb + Ring   → DIRECTION");
  Serial.println("9.  Thumb + Little → ALRIGHT");
  Serial.println("10. Index + Middle → CALL");
  Serial.println();
  Serial.println("ALL FINGERS GESTURE:");
  Serial.println("11. All 5 Fingers  → WHAT IS THE TIME");
  Serial.println();
  Serial.println("Stabilization Delay: " + String(STABILIZATION_DELAY) + "ms");
  Serial.println("Cooldown Period: " + String(COOLDOWN_PERIOD / 1000) + " seconds");
  Serial.println("========================================\n");
  
  delay(1000);
}

// ========================================
// MAIN LOOP
// ========================================
void loop() {
  
  // ========================================
  // CHECK COOLDOWN STATUS
  // ========================================
  if (inCooldown) {
    if (millis() - cooldownStartTime >= COOLDOWN_PERIOD) {
      inCooldown = false;
      Serial.println("✓ Cooldown finished - Ready for next gesture");
    }
  }
  
  // ========================================
  // READ ALL SENSORS
  // ========================================
  flex1_Value = analogRead(FLEX_PIN_1);
  flex2_Value = analogRead(FLEX_PIN_2);
  flex3_Value = analogRead(FLEX_PIN_3);
  flex4_Value = analogRead(FLEX_PIN_4);
  flex5_Value = analogRead(FLEX_PIN_5);
  
  // Check if sensors are ACTIVE using individual thresholds
  finger1_Active = (flex1_Value > THRESHOLD_1);
  finger2_Active = (flex2_Value > THRESHOLD_2);
  finger3_Active = (flex3_Value > THRESHOLD_3);
  finger4_Active = (flex4_Value > THRESHOLD_4);
  finger5_Active = (flex5_Value > THRESHOLD_5);  
  
  // ========================================
  // DETERMINE GESTURE (Priority system)
  // ALL FINGERS > TWO-FINGER > SINGLE-FINGER
  // ========================================
  int newGesture = 0;
  
  // Count active fingers
  int activeCount = 0;
  if (finger1_Active) activeCount++;
  if (finger2_Active) activeCount++;
  if (finger3_Active) activeCount++;
  if (finger4_Active) activeCount++;
  if (finger5_Active) activeCount++;
  
  // Check ALL 5 FINGERS FIRST (highest priority)
  if (activeCount == 5) {
    newGesture = 11;  // All 5 fingers → WHAT IS THE TIME
  }
  // Then check TWO-FINGER gestures (exactly 2 fingers)
  else if (activeCount == 2) {
    if (finger1_Active && finger2_Active) {
      newGesture = 6;  // Thumb + Index → WATER
    }
    else if (finger1_Active && finger3_Active) {
      newGesture = 7;  // Thumb + Middle → FOOD
    }
    else if (finger1_Active && finger4_Active) {
      newGesture = 8;  // Thumb + Ring → DIRECTION
    }
    else if (finger1_Active && finger5_Active) {
      newGesture = 9;  // Thumb + Little → ALRIGHT
    }
    else if (finger2_Active && finger3_Active) {
      newGesture = 10; // Index + Middle → CALL
    }
  }
  // Then check SINGLE-FINGER gestures (exactly 1 finger)
  else if (activeCount == 1) {
    if (finger1_Active) {
      newGesture = 1;  // Only thumb → THANK YOU
    }
    else if (finger2_Active) {
      newGesture = 2;  // Only index → HELLO
    }
    else if (finger3_Active) {
      newGesture = 3;  // Only middle → PLEASE
    }
    else if (finger4_Active) {
      newGesture = 4;  // Only ring → SORRY
    }
    else if (finger5_Active) {
      newGesture = 5;  // Only little → BYE
    }
  }
  else {
    newGesture = 0;  // 0, 3, or 4 fingers → No gesture
  }
  
  // ========================================
  // GESTURE STABILIZATION LOGIC
  // ========================================
  if (newGesture != currentGesture) {
    // Gesture changed - restart the timer
    currentGesture = newGesture;
    gestureStartTime = millis();
    lastGestureChangeTime = millis();
    
    if (newGesture != 0) {
      Serial.print("⏱️  Gesture detected: ");
      printGestureName(newGesture);
      Serial.println("    Waiting for stabilization...");
    }
  }
  
  // Check if gesture has been stable long enough
  if (currentGesture != 0 && (millis() - gestureStartTime >= STABILIZATION_DELAY)) {
    stableGesture = currentGesture;
  } else if (currentGesture == 0) {
    stableGesture = 0;
  }
  
  // ========================================
  // PRINT STATUS (Every 500ms)
  // ========================================
  if (millis() - lastPrintTime > 500) {
    Serial.println("─────────────────────────────────────");
    
    Serial.print("Sensor 1 (Thumb):   ");
    Serial.print(flex1_Value);
    Serial.print(" | Threshold: ");
    Serial.print(THRESHOLD_1);
    Serial.print(" | ");
    Serial.println(finger1_Active ? "ACTIVE ✓" : "INACTIVE");
    
    Serial.print("Sensor 2 (Index):   ");
    Serial.print(flex2_Value);
    Serial.print(" | Threshold: ");
    Serial.print(THRESHOLD_2);
    Serial.print(" | ");
    Serial.println(finger2_Active ? "ACTIVE ✓" : "INACTIVE");
    
    Serial.print("Sensor 3 (Middle):  ");
    Serial.print(flex3_Value);
    Serial.print(" | Threshold: ");
    Serial.print(THRESHOLD_3);
    Serial.print(" | ");
    Serial.println(finger3_Active ? "ACTIVE ✓" : "INACTIVE");
    
    Serial.print("Sensor 4 (Ring):    ");
    Serial.print(flex4_Value);
    Serial.print(" | Threshold: ");
    Serial.print(THRESHOLD_4);
    Serial.print(" | ");
    Serial.println(finger4_Active ? "ACTIVE ✓" : "INACTIVE");
    
    Serial.print("Sensor 5 (Little):  ");
    Serial.print(flex5_Value);
    Serial.print(" | Threshold: ");
    Serial.print(THRESHOLD_5);
    Serial.print(" | ");
    Serial.print(finger5_Active ? "ACTIVE ✓" : "INACTIVE");
    Serial.println(" [INVERTED]");
    
    Serial.print("Current Gesture: ");
    printGestureName(currentGesture);
    
    Serial.print("Stable Gesture: ");
    printGestureName(stableGesture);
    
    if (currentGesture != 0 && stableGesture == 0) {
      unsigned long waitTime = STABILIZATION_DELAY - (millis() - gestureStartTime);
      Serial.print("⏱️  Stabilizing... ");
      Serial.print(waitTime);
      Serial.println("ms remaining");
    }
    
    Serial.print("BT Connected: ");
    Serial.println(SerialBT.hasClient() ? "YES ✓" : "NO ✗");
    
    Serial.print("Cooldown Status: ");
    if (inCooldown) {
      unsigned long remainingTime = COOLDOWN_PERIOD - (millis() - cooldownStartTime);
      Serial.print("ACTIVE (");
      Serial.print(remainingTime / 1000);
      Serial.println("s remaining)");
    } else {
      Serial.println("Ready");
    }
    
    lastPrintTime = millis();
  }
  
  // ========================================
  // SEND VIA BLUETOOTH
  // ========================================
  if (SerialBT.hasClient()) {
    
    if (!inCooldown && stableGesture != 0 && !gestureSent) {
      
      if (millis() - lastSendTime > SEND_INTERVAL) {
        
        SerialBT.println(stableGesture);
        
        Serial.println("\n========================================");
        Serial.print("📤 SENT TO APP: ");
        Serial.print(stableGesture);
        Serial.print(" → ");
        printGestureName(stableGesture);
        
        Serial.println("🕐 Cooldown started: " + String(COOLDOWN_PERIOD / 1000) + " seconds");
        Serial.println("========================================\n");
        
        lastSendTime = millis();
        lastGesture = stableGesture;
        gestureSent = true;
        inCooldown = true;
        cooldownStartTime = millis();
      }
    }
    
    if (stableGesture == 0) {
      gestureSent = false;
    }
    
  } else {
    static unsigned long lastStatusTime = 0;
    if (millis() - lastStatusTime > 2000) {
      Serial.println("⏳ Waiting for Bluetooth connection...");
      lastStatusTime = millis();
    }
  }
  
  delay(50);
}

// ========================================
// HELPER FUNCTION: Print gesture name
// ========================================
void printGestureName(int gesture) {
  switch(gesture) {
    case 1:
      Serial.println("1 → THANK YOU (Thumb)");
      break;
    case 2:
      Serial.println("2 → HELLO (Index)");
      break;
    case 3:
      Serial.println("3 → PLEASE (Middle)");
      break;
    case 4:
      Serial.println("4 → SORRY (Ring)");
      break;
    case 5:
      Serial.println("5 → BYE (Little)");
      break;
    case 6:
      Serial.println("6 → WATER (Thumb + Index)");
      break;
    case 7:
      Serial.println("7 → FOOD (Thumb + Middle)");
      break;
    case 8:
      Serial.println("8 → DIRECTION (Thumb + Ring)");
      break;
    case 9:
      Serial.println("9 → ALRIGHT (Thumb + Little)");
      break;
    case 10:
      Serial.println("10 → CALL (Index + Middle)");
      break;
    case 11:
      Serial.println("11 → WHAT IS THE TIME (All 5 Fingers)");
      break;
    default:
      Serial.println("--- (None)");
      break;
  }
}