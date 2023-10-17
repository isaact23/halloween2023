// Node 2 - The Nun

#include "painlessMesh.h"
#include "Skull.hpp"

#define   MESH_PREFIX     "whateverYouLike"
#define   MESH_PASSWORD   "somethingSneaky"
#define   MESH_PORT       5555

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#define SERVOMIN  140 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  520 // this is the 'maximum' pulse length count (out of 4096)
#define SERVOCOUNT 12 // number of servos to update each loop

uint8_t servonum = 0; // I'm  not really sure what this does

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
Scheduler userScheduler; 
painlessMesh  mesh;

void send_Message(); 
void mode_Idle(); 
void mode_Attract(); 
void mode_Approach(); 
void mode_Scare(); 
void update_Servos();
void manual_Calibration();
//void move_Eyes();

void set_Jaw(float jaw_Open);
void set_Eyes(float l_e_h, float r_e_h, float l_e_v, float r_e_v, float lid);
void set_Head(float w_a, float w_b, float w_c, float w_d);

//update frequency counter
unsigned long previousMillis = 0UL;
unsigned long interval = 500UL;

//int eye_mode = 0;

//servo ranges and initial values {lowval,highval,curval}

int l_jaw[3] = {380,500,380};
int r_jaw = map(l_jaw[2],l_jaw[0],l_jaw[1],l_jaw[1],l_jaw[0]);
int l_eye_v[3] = {140,240,140};
int r_eye_v[3] = {240,140,140};
int l_eye_h[3] = {140,240,140};
int r_eye_h[3] = {140,240,140};
int t_lid[3] = {250,500,500};
int b_lid[3] = {500,250,500};
int wire_a[3] = {500,150,500};
int wire_b[3] = {150,500,150};
int wire_c[3] = {500,150,500};
int wire_d[3] = {150,500,150};

//initial servo value to avoid crash - finalize these values prior to connecting all servo arms
int pwm_val[12] = {l_eye_h[2],r_eye_h[2],l_eye_v[2],r_eye_v[2],t_lid[2],b_lid[2],l_jaw[2],r_jaw,wire_a[2],wire_b[2],wire_c[2],wire_d[2]};

Task task_Send_Message(1000, TASK_FOREVER, &send_Message);
Task task_mode_Idle (1000,0,&mode_Idle);
Task task_mode_Attract (interval,TASK_FOREVER,&mode_Attract);
Task task_mode_Approach (1000,0,&mode_Approach);
Task task_mode_Scare (1000,0,&mode_Scare);
Task task_update_Servos (5, TASK_FOREVER, &update_Servos);
//Task task_manual_Calibration (1000, TASK_FOREVER, &manual_Calibration);
//Task task_move_Eyes (100, TASK_FOREVER, &move_Eyes);

// Broadcast message
void send_Message() {
  String msg = "The nun says hi";
  mesh.sendBroadcast( msg );
  Serial.println(msg);
}

//Map jaw movement, pass percentage 0 = closed, 1 = open
void set_Jaw(float jaw_Open) {
  
  l_jaw[2] = map(jaw_Open,0,1,l_jaw[0],l_jaw[1]);
  r_jaw = map(l_jaw[2],l_jaw[0],l_jaw[1],l_jaw[1],l_jaw[0]);

  pwm_val[6] = l_jaw[2];
  pwm_val[7] = r_jaw;

}

//Map eye position, pass percentage 0 = left / down / closed, 1 = right / up / open
void set_Eyes(float l_e_h, float r_e_h, float l_e_v, float r_e_v, float lid) {

  l_eye_h[2] = map(l_e_h, 0, 1, l_eye_h[0], l_eye_h[1]);
  r_eye_h[2] = map(r_e_h, 0, 1, r_eye_h[0], r_eye_h[1]);
  l_eye_v[2] = map(l_e_v, 0, 1, l_eye_v[0], l_eye_v[1]);
  r_eye_v[2] = map(r_e_v, 0, 1, r_eye_v[0], r_eye_v[1]);

  // still need to map the eyelid movement to match

  pwm_val[0] = l_eye_h[2];
  pwm_val[1] = r_eye_h[2];
  pwm_val[2] = l_eye_v[2];
  pwm_val[3] = r_eye_v[2];
 
}

//Map Head movement, pass percentage, 0 = down, 1=up
void set_Head(float w_a, float w_b, float w_c, float w_d) {

  wire_a[2] = map(w_a, 0, 1, wire_a[0], wire_a[1]);
  wire_b[2] = map(w_b, 0, 1, wire_b[0], wire_b[1]);
  wire_c[2] = map(w_c, 0, 1, wire_c[0], wire_c[1]);
  wire_d[2] = map(w_d, 0, 1, wire_d[0], wire_d[1]);

  pwm_val[8] = wire_a[2];
  pwm_val[9] = wire_b[2];
  pwm_val[10] = wire_c[2];
  pwm_val[11] = wire_d[2];

}

/* Back to the drawing board
//Movement functions//
void move_Eyes() {

  //mode 0 = move eyes toward center
  if (eye_mode == 0) {

    l_eye_h[2] = (l_eye_h[1]-l_eye_h[0])/2 + (l_eye_h[2]-(l_eye_h[1]-l_eye_h[0])/2)/2;
    r_eye_h[2] = (r_eye_h[1]-r_eye_h[0])/2 + (r_eye_h[2]-(r_eye_h[1]-r_eye_h[0])/2)/2;
    
    l_eye_v[2] = (l_eye_v[1]-l_eye_v[0])/2 + (l_eye_v[2]-(l_eye_v[1]-l_eye_v[0])/2)/2;
    r_eye_v[2] = (r_eye_v[1]-r_eye_v[0])/2 + (r_eye_v[2]-(r_eye_v[1]-r_eye_v[0])/2)/2;

    pwm_val[0] = l_eye_h[2];
    pwm_val[1] = r_eye_h[2];

    pwm_val[2] = l_eye_v[2];
    pwm_val[3] = r_eye_v[2];
  }

  //mode 1 = move eyes toward left
  else if (eye_mode == 1) {

    l_eye_h[2] = l_eye_h[0] + (l_eye_h[2]-l_eye_h[0])/2;
    r_eye_h[2] = r_eye_h[0] + (r_eye_h[2]-r_eye_h[0])/2;
    
    l_eye_v[2] = (l_eye_v[1]-l_eye_v[0])/2 + (l_eye_v[2]-(l_eye_v[1]-l_eye_v[0])/2)/2;
    r_eye_v[2] = (r_eye_v[1]-r_eye_v[0])/2 + (r_eye_v[2]-(r_eye_v[1]-r_eye_v[0])/2)/2;

    pwm_val[0] = l_eye_h[2];
    pwm_val[1] = r_eye_h[2];

    pwm_val[2] = l_eye_v[2];
    pwm_val[3] = r_eye_v[2];
  }

  //mode 2 = move eyes top left
  else if (eye_mode == 2) {

    l_eye_h[2] = l_eye_h[0] + (l_eye_h[2]-l_eye_h[0])/2;
    r_eye_h[2] = r_eye_h[0] + (r_eye_h[2]-r_eye_h[0])/2;
    
    l_eye_v[2] = l_eye_v[0] + (l_eye_v[2]-l_eye_v[0])/2;
    r_eye_v[2] = r_eye_v[1] + (r_eye_v[2]-r_eye_v[1])/2;

    pwm_val[0] = l_eye_h[2];
    pwm_val[1] = r_eye_h[2];

    pwm_val[2] = l_eye_v[2];
    pwm_val[3] = r_eye_v[2];
  }

  //mode 3 = move eyes up
  else if (eye_mode == 3) {

    l_eye_h[2] = (l_eye_h[1]-l_eye_h[0])/2 + (l_eye_h[2]-(l_eye_h[1]-l_eye_h[0])/2)/2;
    r_eye_h[2] = (r_eye_h[1]-r_eye_h[0])/2 + (r_eye_h[2]-(r_eye_h[1]-r_eye_h[0])/2)/2;
    
    l_eye_v[2] = l_eye_v[0] + (l_eye_v[2]-l_eye_v[0])/2;
    r_eye_v[2] = r_eye_v[1] + (r_eye_v[2]-r_eye_v[1])/2;

    pwm_val[0] = l_eye_h[2];
    pwm_val[1] = r_eye_h[2];

    pwm_val[2] = l_eye_v[2];
    pwm_val[3] = r_eye_v[2];
  }

  //mode 4 = move eyes toward top right
  else if (eye_mode == 4) {

    l_eye_h[2] = l_eye_h[1] + (l_eye_h[2]-l_eye_h[1])/2;
    r_eye_h[2] = r_eye_h[1] + (r_eye_h[2]-r_eye_h[1])/2;
    
    l_eye_v[2] = l_eye_v[0] + (l_eye_v[2]-l_eye_v[0])/2;
    r_eye_v[2] = r_eye_v[1] + (r_eye_v[2]-r_eye_v[1])/2;

    pwm_val[0] = l_eye_h[2];
    pwm_val[1] = r_eye_h[2];

    pwm_val[2] = l_eye_v[2];
    pwm_val[3] = r_eye_v[2];
  }

  //mode 5 = move eyes toward right
  else if (eye_mode == 5) {

    l_eye_h[2] = l_eye_h[1] + (l_eye_h[2]-l_eye_h[1])/2;
    r_eye_h[2] = r_eye_h[1] + (r_eye_h[2]-r_eye_h[1])/2;
    
    l_eye_v[2] = (l_eye_v[1]-l_eye_v[0])/2 + (l_eye_v[2]-(l_eye_v[1]-l_eye_v[0])/2)/2;
    r_eye_v[2] = (r_eye_v[1]-r_eye_v[0])/2 + (r_eye_v[2]-(r_eye_v[1]-r_eye_v[0])/2)/2;

    pwm_val[0] = l_eye_h[2];
    pwm_val[1] = r_eye_h[2];

    pwm_val[2] = l_eye_v[2];
    pwm_val[3] = r_eye_v[2];
  }

  //mode 6 = move eyes toward bottom right 
  else if (eye_mode == 6) {

    l_eye_h[2] = l_eye_h[1] + (l_eye_h[2]-l_eye_h[1])/2;
    r_eye_h[2] = r_eye_h[1] + (r_eye_h[2]-r_eye_h[1])/2;
    
    l_eye_v[2] = l_eye_v[1] + (l_eye_v[2]-l_eye_v[1])/2;
    r_eye_v[2] = r_eye_v[0] + (r_eye_v[2]-r_eye_v[0])/2;

    pwm_val[0] = l_eye_h[2];
    pwm_val[1] = r_eye_h[2];

    pwm_val[2] = l_eye_v[2];
    pwm_val[3] = r_eye_v[2];
  }

  //mode 7 = move eyes toward bottom 
  else if (eye_mode == 7) {

    l_eye_h[2] = (l_eye_h[1]-l_eye_h[0])/2 + (l_eye_h[2]-(l_eye_h[1]-l_eye_h[0])/2)/2;
    r_eye_h[2] = (r_eye_h[1]-r_eye_h[0])/2 + (r_eye_h[2]-(r_eye_h[1]-r_eye_h[0])/2)/2;
    
    l_eye_v[2] = l_eye_v[1] + (l_eye_v[2]-l_eye_v[1])/2;
    r_eye_v[2] = r_eye_v[0] + (r_eye_v[2]-r_eye_v[0])/2;

    pwm_val[0] = l_eye_h[2];
    pwm_val[1] = r_eye_h[2];

    pwm_val[2] = l_eye_v[2];
    pwm_val[3] = r_eye_v[2];
  }

  //mode 8 = move eyes toward bottom left
  else if (eye_mode == 8) {

    l_eye_h[2] = l_eye_h[0] + (l_eye_h[2]-l_eye_h[0])/2;
    r_eye_h[2] = r_eye_h[0] + (r_eye_h[2]-r_eye_h[0])/2;
    
    l_eye_v[2] = l_eye_v[1] + (l_eye_v[2]-l_eye_v[1])/2;
    r_eye_v[2] = r_eye_v[0] + (r_eye_v[2]-r_eye_v[0])/2;

    pwm_val[0] = l_eye_h[2];
    pwm_val[1] = r_eye_h[2];

    pwm_val[2] = l_eye_v[2];
    pwm_val[3] = r_eye_v[2];
  }

}
*/

/*

//**CALIBRATION FUNCTION, ALLOWS TO SET INDIVIDUAL SERVO BY SERIAL USING [ADDRESS],[PWM VALUE]
void manual_Calibration() {
  int inc_address;
  int inc_value;
  String inString = "";

  while (Serial.available() > 0) {
    int inChar = Serial.read();
    if (isDigit(inChar)) {
      // convert the incoming byte to a char and add it to the string:
      inString += (char)inChar;
    }
    // if you get a newline, print the string, then the string's value:
    if (inChar == ',') {
      Serial.print("(");
      Serial.print(inString.toInt());
      Serial.print(", ");
      inc_address = inString.toInt();
      // clear the string for new input:
      inString = "";
    }
    if (inChar == '\n') {

      Serial.print(inString.toInt());
      Serial.println(")");
      inc_value = inString.toInt();
      // clear the string for new input:
      inString = "";

      pwm_val[inc_address] = inc_value;
      if (inc_address == 6) {
        r_jaw = map(inc_value,l_jaw[0],l_jaw[1],l_jaw[1],l_jaw[0]);
        pwm_val[7] = r_jaw;
      }

    }
  }
}
//**END CALIBRATION FUNCTION

*/

//Constantly running - update servos with current values
void update_Servos() {   
  for (int i = 0; i < SERVOCOUNT; i++) {
    pwm.setPWM(i, 0, pwm_val[i]);
  }
}

//Idle mode, default state
void mode_Idle() {

}

//Attract mode, get the people going
void mode_Attract() {

  if (l_jaw[2] > 400) {
    set_Jaw(0);
  }
  else {
    set_Jaw(1);
  }      
  
}

void mode_Approach() {

}

void mode_Scare() {

}

// Log messages received, change mode
void receivedCallback( uint32_t from, String &msg ) {
  
  Serial.printf("startHere: Received from %u msg=%s\n", from, msg.c_str());
  String new_mode = msg.c_str();
  
  if (new_mode == "IDLE"){ 
    task_mode_Approach.disable();
    task_mode_Attract.disable();
    task_mode_Scare.disable();
    task_mode_Idle.enable();
  }
  else if (new_mode == "ATTRACT"){
    task_mode_Approach.disable();
    task_mode_Scare.disable();
    task_mode_Idle.disable();
    task_mode_Attract.enable();
  }
  else if (new_mode == "SCARE"){
    task_mode_Approach.disable();
    task_mode_Idle.disable();
    task_mode_Attract.disable(); 
    task_mode_Scare.enable();
  }
  else if (new_mode == "WELCOME"){
    task_mode_Idle.disable();
    task_mode_Attract.disable(); 
    task_mode_Scare.disable();   
    task_mode_Approach.enable();  
  }
}

void newConnectionCallback(uint32_t nodeId) {
  Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
}

void changedConnectionCallback() {
  Serial.printf("Changed connections\n");
}

void nodeTimeAdjustedCallback(int32_t offset) {
  Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(),offset);
}

void setup() {
  Serial.begin(115200);

//mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
  mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages

  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);

  //Initiate PWM module for Servo Control
  pwm.begin();
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
  delay(10);

  userScheduler.addTask( task_Send_Message );
  userScheduler.addTask( task_mode_Idle );
  userScheduler.addTask( task_mode_Attract );
  userScheduler.addTask( task_mode_Approach );
  userScheduler.addTask( task_mode_Scare );
  userScheduler.addTask( task_update_Servos );
  //userScheduler.addTask( task_manual_Calibration );
  
  task_update_Servos.enable();
  //task_manual_Calibration.enable();
  task_mode_Idle.enable();
  task_Send_Message.enable();
}

void loop() {
  // it will run the user scheduler as well
  mesh.update();
}