
#include <CytronMotorDriver.h>

const int numMotors = 13;

// Define individual motors
CytronMD motor1(PWM_DIR,6,7); // PWM 1 = Pin 3, DIR 1 = Pin 4
CytronMD motor2(PWM_DIR,10,12); // PWM 2 = Pin 6, DIR 2 = Pin 7
CytronMD motor4(PWM_DIR,9,8); // PWM 3 = Pin 9, DIR 3 = Pin 8
CytronMD motor3(PWM_DIR,11,5); // PWM 4 = Pin 11, DIR 4 = Pin 12

void setup() {
  Serial.begin(9600);  
}

// void controlMotor(CytronMD &motor, int speed) {
//   speed = constrain(speed, -255, 255); 
//   if (speed > 0) {
//     motor.setSpeed(speed);
//   } else if (speed < 0) {
//     motor.backward(abs(speed));
//   } else {
//     motor.stop();
//   }
// }

void loop() {
  if (Serial.available() > 0) {   
    String array_string = Serial.readStringUntil('\n');  // Read until newline
    int parsed_values[numMotors];
    // Parse the string into the array
    for (int i = 0; i < numMotors; i++) {
      parsed_values[i] = array_string.toInt();
      if (i < numMotors - 1) {
        // Skip the delimiter (comma) for subsequent values
        array_string = array_string.substring(array_string.indexOf(' ')+1);
      }
    }
    // Control each motor individually
    motor1.setSpeed(parsed_values[0]);
    motor2.setSpeed(parsed_values[1]);
    motor3.setSpeed(parsed_values[2]);
    motor4.setSpeed(parsed_values[3]);

    if (parsed_values[4] == 1) {
            // DC motor goes anticlockwise (up for gripper) for 1 sec
            analogWrite(motorPins[0][0], PWM); // Move motor 1 forward
            digitalWrite(motorPins[0][1], HIGH); // Activate motor 1
            delay(1000); // Wait for 1 second
            analogWrite(motorPins[0][0], 0); // Stop motor 1
            digitalWrite(motorPins[0][1], LOW); // Deactivate motor 1
            Serial.print("gripper");
        }

        if (parsed_values[6] == 1) {
            // DC motor goes clockwise (down) for gripper 1 sec
            analogWrite(motorPins[1][0], PWM); // Move motor 2 backward
            digitalWrite(motorPins[1][1], HIGH); // Activate motor 2
            delay(1000); // Wait for 1 second
            analogWrite(motorPins[1][0], 0); // Stop motor 2
            digitalWrite(motorPins[1][1], LOW); // Deactivate motor 2
        }

        if (parsed_values[5] == 1) {
            // DC motor for hand goes clockwise (open) for 1 sec
            analogWrite(motorPins[2][0], PWM); // Move motor 3 forward
            digitalWrite(motorPins[2][1], HIGH); // Activate motor 3
            delay(1000); // Wait for 1 second
            analogWrite(motorPins[2][0], 0); // Stop motor 3
            digitalWrite(motorPins[2][1], LOW); // Deactivate motor 3
        }

        if (parsed_values[7] == 1) {
            // DC motor for hand goes anticlockwise (close) for 1 sec
            analogWrite(motorPins[3][0], PWM); // Move motor 4 backward
            digitalWrite(motorPins[3][1], HIGH); // Activate motor 4
            delay(1000); // Wait for 1 second
            analogWrite(motorPins[3][0], 0); // Stop motor 4
            digitalWrite(motorPins[3][1], LOW); // Deactivate motor 4
        }

        if (parsed_values[9] == 1) {
            // Turn on one DC motor for 2 seconds (ball shooting)
            analogWrite(motorPins[0][0], PWM); // Move motor 1 forward
            digitalWrite(motorPins[0][1], HIGH); // Activate motor 1
            delay(2000); // Wait for 2 seconds
            analogWrite(motorPins[0][0], 0); // Stop motor 1
            digitalWrite(motorPins[0][1], LOW); // Deactivate motor 1
        }

        if (parsed_values[8] == 1) {
            // Turn on linear actuator at half pressure
            // Assuming the linear actuator is connected to motorPin[0][0]
            analogWrite(motorPins[0][0], PWM / 2); // Run motor at half power
            digitalWrite(motorPins[0][1], HIGH); // Activate motor
            delay(1000); // Hold position for 1 second
            analogWrite(motorPins[0][0], 0); // Stop motor
            digitalWrite(motorPins[0][1], LOW); // Deactivate motor
        }
  }
}
