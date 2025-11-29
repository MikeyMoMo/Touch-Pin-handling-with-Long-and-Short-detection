const int threshold = 50;

// Pins we want to monitor
const int touchPins[] = {13, 14, 15};
const int numPins = sizeof(touchPins) / sizeof(touchPins[0]);

#define FILTER_SAMPLES 5
#define FILTER_DELAY 10   // ms between filtered samples
const int longTouchThreshold = 400; // ms threshold for long press

// State tracking
bool lastState[numPins] = {false};
volatile bool touchDetected[numPins] = {false};

// Timing and duration state (avoid HAL conflict)
unsigned long pressStartMs[numPins] = {0};
bool isTouching[numPins] = {false};

// Function pointer type now accepts a bool
typedef void (*FuncPtr)(bool);

// Pin-specific routines
void Pin13Rtn(bool isLong) {
  Serial.print(millis());
  if (isLong) {
    Serial.println(" ⚡ Dispatch Rite: Pin 13 LONG press!");
  } else {
    Serial.println(" ⚡ Dispatch Rite: Pin 13 SHORT press!");
  }
}

void Pin14Rtn(bool isLong) {
  Serial.print(millis());
  Serial.println(isLong ? " ⚡ Dispatch Rite: Pin 14 LONG press!" 
                        : " ⚡ Dispatch Rite: Pin 14 SHORT press!");
}

void Pin15Rtn(bool isLong) {
  Serial.print(millis());
  Serial.println(isLong ? " ⚡ Dispatch Rite: Pin 15 LONG press!" 
                        : " ⚡ Dispatch Rite: Pin 15 SHORT press!");
}

// Dispatch table
FuncPtr pinFuncs[numPins] = {Pin13Rtn, Pin14Rtn, Pin15Rtn};

/***************************************************************************/
// Optional filtering: average multiple samples to reduce noise
bool filteredTouchRead(int pin, int samples = FILTER_SAMPLES) {
  long sum = 0;
  for (int i = 0; i < samples; i++) {
    sum += touchRead(pin);
    delay(FILTER_DELAY);
  }
  int avg = sum / samples;
  return avg < threshold;
}
/***************************************************************************/

// Macro to generate interrupt handlers
#define TOUCH_HANDLER(idx) \
  void touchHandler##idx() { touchDetected[idx] = true; }

// Generate handlers for each pin index
TOUCH_HANDLER(0)
TOUCH_HANDLER(1)
TOUCH_HANDLER(2)

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("ESP32 Touch Interrupt Test (filtered + long/short + duration)");

  // Attach interrupts using generated handlers
  touchAttachInterrupt(touchPins[0], touchHandler0, threshold);
  touchAttachInterrupt(touchPins[1], touchHandler1, threshold);
  touchAttachInterrupt(touchPins[2], touchHandler2, threshold);
}

void loop() {
  for (int i = 0; i < numPins; i++) {
    if (touchDetected[i]) {
      touchDetected[i] = false;

      // Filtered read for robustness
      bool current = filteredTouchRead(touchPins[i]);

      // Rising edge: begin touch
      if (current && !lastState[i]) {
        pressStartMs[i] = millis();
        isTouching[i] = true;
      }

      // Falling edge: end touch -> compute duration
      if (!current && lastState[i] && isTouching[i]) {
        unsigned long duration = millis() - pressStartMs[i];
        bool isLong = duration >= (unsigned long)longTouchThreshold;

        // Log with duration included
        Serial.print(millis());
        Serial.print(" ⚡ Dispatch Rite: Pin ");
        Serial.print(touchPins[i]);
        Serial.print(isLong ? " LONG press (" : " SHORT press (");
        Serial.print(duration);
        Serial.println(" ms)");

        // Pin-specific Dispatch Rite with flag
        pinFuncs[i](isLong);

        isTouching[i] = false;
      }

      // Update debounced/filtered state
      lastState[i] = current;
    }
  }
}
