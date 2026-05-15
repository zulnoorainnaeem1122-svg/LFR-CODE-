
#define ENA 9
#define IN1 8
#define IN2 7

#define ENB 6
#define IN3 5
#define IN4 4

int sensorPins[5] = {A0, A1, A2, A3, A4};
int sensorValues[5];

int threshold = 500;  
int baseSpeed = 150;
int turnSpeed = 180;


void setup() {
  Serial.begin(9600);

  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
}

// ===========================
// Read All 5 Sensors
// ===========================
void readSensors() {
  for (int i = 0; i < 5; i++) {
    sensorValues[i] = analogRead(sensorPins[i]);
  }
}

// ===========================
// Motor Controls
// ===========================
void moveForward(int L, int R) {
  analogWrite(ENA, L);
  analogWrite(ENB, R);

  digitalWrite(IN1, LOW);   // FORWARD
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, LOW);   // FORWARD
  digitalWrite(IN4, HIGH);
}

void turnLeft() {
  analogWrite(ENA, 80);
  analogWrite(ENB, turnSpeed);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void turnRight() {
  analogWrite(ENA, turnSpeed);
  analogWrite(ENB, 80);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void stopRobot() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

// ===========================
// MAIN LOOP
// ===========================
void loop() {

  readSensors();

  bool S0 = sensorValues[0] < threshold; // LEFT MOST
  bool S1 = sensorValues[1] < threshold;
  bool S2 = sensorValues[2] < threshold; // CENTER
  bool S3 = sensorValues[3] < threshold;
  bool S4 = sensorValues[4] < threshold; // RIGHT MOST

  // Debug (optional)
  Serial.print(S0); Serial.print(" ");
  Serial.print(S1); Serial.print(" ");
  Serial.print(S2); Serial.print(" ");
  Serial.print(S3); Serial.print(" ");
  Serial.println(S4);

  // ===========================
  // PERFECT CENTER
  // ===========================
  if (S2 && !S0 && !S1 && !S3 && !S4) {
    moveForward(baseSpeed, baseSpeed);
    return;
  }

  // ===========================
  // Slight Left Correction
  // ===========================
  if (S1 && !S3) {
    turnLeft();
    return;
  }

  // ===========================
  // Slight Right Correction
  // ===========================
   (S3 && !S1) {
   // turnRight();
   // return;
  //}

  // ===========================
  // Hard Left (S0 detects)
  // ===========================
  if (S0||S1&!S2&!S3&!S4) {
    turnLeft();
    return;
  }

  // ===========================
  // Hard Right (S4 detects)
  // ===========================
  if (S4||S3||S2&!S1&!S0) {
    turnRight();
    return;
  }
  if(S4||S3||S2||S1||S0){
  turnRight();
  return;
}

  // ===========================
  // If NO sensor detects line → stop OR last direction
  // ===========================
  moveForward(80, 80); // small forward to search
}
