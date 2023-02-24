// This code is test code for a BIOE123 centrifuge (version 1b)
//  
// Key elements:
//
//  - set the motor speed based on user input
//  - read motor encoder (using interrupts)
//
//  The averaging code is based on code by by David A. Mellis  <dam@mellis.org> and by Tom Igoe
//  The encoder code is by Ethan Li
//  The rest is by Ross Venook; 04NOV2020 & 16JAN2023

//define variables
//analog input + pwm variables
//int write_duty = 0; // duty cycle to send to mosfet to run motor (PWM)
float angular_velocity = 0;
int motor_gate_pin = 5;

char tmp_str[7]; // temporary variable used in convert function

void setup() {
  pinMode(motor_gate_pin, OUTPUT);
  
  Serial.begin(9600);
  while (!Serial);  // wait until the serial monitor is open before proceeding
  Serial.println("Please enter desired motor speed (integer sets PWM between 0 and 255): ");
  //current_time = millis();
}
int write_duty = 0;
void loop() {
// get input from Serial monitor to set duty cycle
  //while (Serial.available() == 0) {
  //}
  //int time_set = millis() - start_time;
  Serial.println(write_duty);
  //if (millis() < 60000) {
    if(Serial.available()>0){
      //write_duty = Serial.read();
      write_duty = Serial.parseInt();
      analogWrite(motor_gate_pin, write_duty);
      //Serial.println((String)"Duty cycle set to: "+write_duty);
      //Serial.println("Enter another integer (PWM between 0 and 255) to change speed: ");   

      //Set desired time of centrifugation
    }
      delay(500);      
    
  //}
}  
