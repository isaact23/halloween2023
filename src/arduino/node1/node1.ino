// Node 1 - The messenger
// This node is directly connected to the laptop running the python
// host program.

/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp-mesh-esp32-esp8266-painlessmesh/
  
  This is a simple example that uses the painlessMesh library: https://github.com/gmag11/painlessMesh/blob/master/examples/basic/basic.ino
*/

#include "painlessMesh.h"

#define   NODE_NAME       "Node 1 (Messenger)"

#define   MESH_PREFIX     "whateverYouLike"
#define   MESH_PASSWORD   "somethingSneaky"
#define   MESH_PORT       5555

Scheduler userScheduler; // to control your personal task
painlessMesh  mesh;

// User stub
void sendPCToMesh();

Task taskPCToMesh(
  1,  // Interval in milliseconds
  TASK_FOREVER,     // Number of times to execute task
  &sendPCToMesh      // Callback
);

void sendPCToMesh() {
  if (Serial.available()) {
    String str = Serial.readString();
    str.trim();
    Serial.printf("Message from computer to node 1 - %s\n", str);
    mesh.sendBroadcast(str);
  }
  //mesh.sendBroadcast("Check-in from messenger");
}

// Needed for painless library
void receivedCallback( uint32_t from, String &msg ) {
  Serial.printf("%s: Received from %u msg=%s\n", NODE_NAME, from, msg.c_str());
}

void newConnectionCallback(uint32_t nodeId) {
  Serial.printf("%s: New Connection, nodeId = %u\n", NODE_NAME, nodeId);
}

void changedConnectionCallback() {
  Serial.printf("Changed connections\n");
}

void nodeTimeAdjustedCallback(int32_t offset) {
  //Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(),offset);
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

  userScheduler.addTask( taskPCToMesh );
  taskPCToMesh.enable();
}

void loop() {
  // it will run the user scheduler as well
  mesh.update();
}
