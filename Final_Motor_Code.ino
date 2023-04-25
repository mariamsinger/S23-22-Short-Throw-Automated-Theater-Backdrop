// Define the digital output pins for the driver control
#define PUL_PIN 2
#define DIR_PIN 3
#define ENA_PIN 4

// Define the digital input pins for the switch
#define SWITCH_LEFT_PIN 5
#define SWITCH_RIGHT_PIN 6

// Define the digital pins used for the E-stop buttons
const int ESTOP_PIN = 7;

// Define the motor direction constants
#define CW 0
#define CCW 1

// Define the switch state constants
#define SWITCH_LEFT 0
#define SWITCH_MIDDLE 1
#define SWITCH_RIGHT 2

// Define the rotation counters and rotations the motor can do
// 1000 steps are 1 rotation

int Rotations = 20; // 4 rotations
int steps = 200; // 200 steps set for more accuracy control
int clockwiseRotations = 0; // Initialize cw rotations
int counterclockwiseRotations = 0; // Initialize ccw rotations

void setup() {
  // Configure the digital output pins for the driver control
  pinMode(PUL_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(ENA_PIN, OUTPUT);

  // Configure the digital input pins for the switch
  pinMode(SWITCH_LEFT_PIN, INPUT_PULLUP);
  pinMode(SWITCH_RIGHT_PIN, INPUT_PULLUP);

  // Set the E-stop button pin to input with pull-up resistors
  pinMode(ESTOP_PIN, INPUT_PULLUP);
}

void loop() {
  // Read the state of the E-stop button
  bool estop_pressed = digitalRead(ESTOP_PIN) == LOW;
  // If either E-stop button is pressed, disable the stepper motor drivers
  if (estop_pressed) {
    digitalWrite(ENA_PIN, HIGH);
  } else {
    digitalWrite(ENA_PIN, LOW);
  }

  // Read the switch state
  int switchState = readSwitch();

  // Control the motor direction based on the switch position
  // Check if the clockwise rotations are less that the max rotations
  // Chcck E-stop is not pressed
  if (switchState == SWITCH_RIGHT && clockwiseRotations <= Rotations && !(estop_pressed)) {
    
    // Rotate the motor clockwise
    digitalWrite(DIR_PIN, CW);
    digitalWrite(ENA_PIN, LOW);
    // Rotate 200 steps
    for (int i = 0; i < steps; i++) {
      digitalWrite(PUL_PIN, LOW);
      delayMicroseconds(500);
      digitalWrite(PUL_PIN, HIGH);
      delayMicroseconds(500);   
    }
    
    // If E-stop button is pressed, disable the stepper motor drivers
    if (estop_pressed) {
      digitalWrite(ENA_PIN, HIGH);
    } else {
      digitalWrite(ENA_PIN, LOW);
    } 
    
    // If E-stop button is not pressed, update the rotation values
    if (!(estop_pressed)) {
      // Adjust the motor's rotation counts
      clockwiseRotations = clockwiseRotations + 1;
      counterclockwiseRotations = Rotations - clockwiseRotations;
      // If the motor exceeds the specified rotations, initialize the counterclockwiseRotations variable to 0
      if (clockwiseRotations >= Rotations) {
        counterclockwiseRotations = 0;
      }
    }

    // Disable Motors
    digitalWrite(ENA_PIN, HIGH);

  // Check if the counterclockwise rotations are less that the max rotations
  } else if (switchState == SWITCH_LEFT && counterclockwiseRotations <= Rotations && !(estop_pressed)) {
    // Rotate the motor counterclockwise
    digitalWrite(DIR_PIN, CCW);
    digitalWrite(ENA_PIN, LOW);
    // Rotate 200 steps
    for (int i = 0; i < steps; i++) {
      digitalWrite(PUL_PIN, LOW);
      delayMicroseconds(500);
      digitalWrite(PUL_PIN, HIGH);
      delayMicroseconds(500);
    }
    
    // If either E-stop button is pressed, disable the stepper motor drivers
    if (estop_pressed) {
      digitalWrite(ENA_PIN, HIGH);
    } else {
      digitalWrite(ENA_PIN, LOW);
    }

    // If E-stop button is not pressed, update the rotation values
    if (!(estop_pressed)) {
      // Adjust the motor's rotation counts
      counterclockwiseRotations = counterclockwiseRotations + 1;
      clockwiseRotations = Rotations - counterclockwiseRotations;
      // If the motor exceeds the specified rotations, initialize the clockwiseRotations variable to 0
      if (counterclockwiseRotations >= Rotations) {
        clockwiseRotations = 0;
      }
    }
    // Disable Motors
    digitalWrite(ENA_PIN, HIGH);
    
  } else {
    digitalWrite(ENA_PIN, HIGH);
  }
}

// Function to read the switch state
int readSwitch() {
  int leftState = digitalRead(SWITCH_LEFT_PIN);
  int rightState = digitalRead(SWITCH_RIGHT_PIN);

  if (leftState == LOW) {
    return SWITCH_LEFT;
  } else if (rightState == LOW) {
    return SWITCH_RIGHT;
  } else {
    return SWITCH_MIDDLE;
  }
}
