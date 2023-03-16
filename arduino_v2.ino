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

float current_speed = 0;

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
  Serial.println("Please enter desired motor speed in RPM and spin cycle time in seconds in this format: <NUMBER, NUMBER>");
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
    centrifuge_time = centrifuge_time*1000;      // convert to ms from seconds
}

void showParsedData() {
    Serial.print("Set Speed (in RPM): ");
    Serial.println(set_speed);
    Serial.print("Centrifuge Cycle Time (in seconds): ");
    Serial.println(centrifuge_time/1000);
}

int compute (int current_speed, int set_speed) {
      float k = 0.03;
      float error = set_speed - current_speed;      
      int out = floor(k*error);
      return out;
}

void loop() {
  
  recvWithStartEndMarkers();
    if (newData == true) {
        strcpy(tempChars, receivedChars);
        parseData();
        showParsedData();
      if (set_speed >= 1) {
        write_duty = 36.4 - (.0102*set_speed) + (pow(set_speed, 2.0) * .0000351);
        start_time = millis();
      } 
      if (set_speed == 0) {
        write_duty = 0;   
      }      
      analogWrite(motor_gate_pin, write_duty);
      newData = false;
    }

    time_cycle = millis() - start_time;

    if (centrifuge_time > 0) {
      if (time_cycle >= centrifuge_time) {
        write_duty = 0;
        analogWrite(motor_gate_pin, write_duty);
        Serial.print("Cycle Complete!");
        centrifuge_time = 0;      
      }
    }
    
    time_interval = millis() - instant_time;

    if (time_interval > 250) {
      current_speed = (count*1000*60)/time_interval; //current speed in RPM 
      instant_time = millis();
      count = 0;
    }

    if (centrifuge_time > 0) {
      unsigned long output = write_duty + (compute(current_speed, set_speed));
      if ((output > 0) && (output <= 255)) {
        write_duty = output;    
        }  
    }

    analogWrite(motor_gate_pin, write_duty);

}
