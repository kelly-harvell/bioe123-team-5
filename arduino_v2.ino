// This code is adapted from the BIOE123 centrifuge (version 1b), by David A. Mellis, Tom Igoe, and Ross Venook; 04NOV2020 & 16JAN2023
// Edits made by Kelly Harvell, Brett Chy, and Zofia Dudek
//  
// Key elements:
//  - set the motor speed based on user input
//  - prompt user for spin cycle time
//  - uses proportional function to calculate error and refine current speed to match set speed

//This must be run at 9V
int motor_gate_pin = 5;
int write_duty = 0;
int photo_pin = 2;
unsigned long count = 0;
unsigned long start_time = 0;
unsigned long user_timespin = 0;
int user_input = 0;
unsigned long instant_time = 0;
unsigned long time_interval = 0;
unsigned long time_cycle = 0;

int set_speed = 0;
float centrifuge_time = 0.0;
const byte numChars = 32;
char receivedChars[numChars];
char tempChars[numChars];
boolean newData = false;

void setup() {
  pinMode(motor_gate_pin, OUTPUT);
  Serial.begin(9600);
  while (!Serial);  // wait until the serial monitor is open before proceeding
  Serial.println("Please enter desired motor speed in RPM.<12, 24.5>");
  attachInterrupt(digitalPinToInterrupt(photo_pin), photo, RISING);
}

void photo() {
  count = count + 1;
}

void recvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;

    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();

        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }

        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
}

void parseData() {      // split the data into its parts

    char * strtokIndx; // this is used by strtok() as an index
 
    strtokIndx = strtok(tempChars, ","); // this continues where the previous call left off
    set_speed = atoi(strtokIndx);     // convert this part to an integer

    strtokIndx = strtok(NULL, ",");
    centrifuge_time = atof(strtokIndx);     // convert this part to a float

}

void showParsedData() {
    Serial.print("Set Speed: ");
    Serial.println(set_speed);
    Serial.print("Centrifuge Cycle Time: ");
    Serial.println(centrifuge_time);
}

int compute (int current_speed, int user_input) {
      //currentTime = millis();
      //elapsedTime = currentTime - previousTime;
      int k = 5;
      int error = user_input - current_speed;
      int out = k*error;
      return out;
}

void loop() {

  recvWithStartEndMarkers();
    if (newData == true) {
        strcpy(tempChars, receivedChars);
        parseData();
        showParsedData();
      //int user_input = Serial.parseInt();
      if (set_speed > 1) {
        write_duty = (set_speed/12);
        //Serial.println("Current speed in RPM: ");
        //Serial.println(set_speed);
        start_time = millis();
      } 
      if (set_speed == 1) {
        write_duty = 0;        
      }      
      analogWrite(motor_gate_pin, write_duty);
      newData = false;
    }
      //Serial.println((String)"Duty cycle set to: "+write_duty);
      //Serial.println("Enter another integer (PWM between 0 and 255) to change speed: ");  
    time_cycle = millis() - start_time;

    if (centrifuge_time > 0) {
      if (time_cycle >= centrifuge_time) {
        write_duty = 0;
        analogWrite(motor_gate_pin, write_duty);
        Serial.println("Cycle Complete!");
        centrifuge_time = 0;      
      }
    }

  //if (millis() < 60000) {
  //  if(Serial.available()>0) {
      //write_duty = Serial.read();

  //    user_input = Serial.parseInt();
  //    if (user_input > 1) {
  //      write_duty = (user_input/12);
  //      Serial.println("Set speed in RPM: ");
  //      Serial.println(user_input);
  //      start_time = millis();

        //Serial.println("Enter desired spin cycle time in seconds: ");
        //int user_timespin = Serial.parseInt();
        //Serial.println(user_timespin);
     
  //      Serial.println("Note: If you change speed during spin cycle, timer restarts.");
  //    } 
  //    if (user_input == 1) {
  //      write_duty = 0;       
  //    }      
  //    analogWrite(motor_gate_pin, write_duty);
  //  }
    
    //current speed in RPM 
    time_interval = millis() - instant_time;

    if (time_interval > 2000) {
      //instant_time = millis() - instant_time;
      float current_speed = (count*1000*60)/time_interval;
      Serial.println(current_speed);
      //int current_speed = (count*1000*60)/time_cycle;

      instant_time = millis();
      count = 0;
      //time_interval = millis() - instant_time;
      //Serial.println("Current speed in RPM: ");
    }

    //input = current_speed;

    //int output = (current_speed/12) + ((compute(current_speed, user_input))/12);
    //Serial.println(output);
    
    //if ((output >= 0) && (output <= 255)) {
    // analogWrite(motor_gate_pin, output);           
    //}

    //time_cycle = millis() - start_time;
    //3050 RPM acheived with 255
    // user_input/12 = write_duty

    //Set desired time of centrifugation 
    //if (time_cycle == (user_timespin*10^3)) {
    //  write_duty = 0;
    //}
}
