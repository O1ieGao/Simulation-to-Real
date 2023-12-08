// Required includes
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <webots/motor.h>
#include <webots/robot.h>
#include <webots/position_sensor.h>
#include <webots/utils/motion.h>
#include <webots/touch_sensor.h>
#include <webots/supervisor.h>
#include <webots/camera.h>


// Define number of steps before and after motion for collecting data
#define STEPS_PRE 25
#define STEPS_POST 25


// Main robot node
static WbNodeRef Robot_Node;

// Rotational motor objects
static WbDeviceTag RShoulderPitchMotor, RShoulderYawMotor, RElbowYawMotor;
static WbDeviceTag LShoulderPitchMotor, LShoulderYawMotor, LElbowYawMotor;
static WbDeviceTag RHipYawMotor, RHipPitchMotor, RKneePitchMotor, RAnklePitchMotor, RAnkleYawMotor;
static WbDeviceTag LHipYawMotor, LHipPitchMotor, LKneePitchMotor, LAnklePitchMotor, LAnkleYawMotor;
static WbDeviceTag NeckRollMotor;

// Motor sensor objects
// Disable to improve performance
/*
static WbDeviceTag RShoulderPitchSensor, RShoulderYawSensor, RElbowYawSensor;
static WbDeviceTag LShoulderPitchSensor, LShoulderYawSensor, LElbowYawSensor;
static WbDeviceTag RHipYawSensor, RHipPitchSensor, RKneePitchSensor, RAnklePitchSensor, RAnkleYawSensor;
static WbDeviceTag LHipYawSensor, LHipPitchSensor, LKneePitchSensor, LAnklePitchSensor, LAnkleYawSensor;
static WbDeviceTag NeckRollSensor;
*/

// Foot sensor objects
static WbDeviceTag LFootSensor1, LFootSensor2, LFootSensor3, LFootSensor4;
static WbDeviceTag RFootSensor5, RFootSensor6, RFootSensor7, RFootSensor8;

// Foot sensor nodes
static WbNodeRef LFootNode1, LFootNode2, LFootNode3, LFootNode4;
static WbNodeRef RFootNode5, RFootNode6, RFootNode7, RFootNode8;

// Camera object
static WbDeviceTag camera;


// Configure the nodes
static void setup_nodes() {
  // Main robot node
  Robot_Node = wb_supervisor_node_get_self();
  
  // TouchSensor nodes
  LFootNode1 = wb_supervisor_node_get_from_device(LFootSensor1);
  LFootNode2 = wb_supervisor_node_get_from_device(LFootSensor2);
  LFootNode3 = wb_supervisor_node_get_from_device(LFootSensor3);
  LFootNode4 = wb_supervisor_node_get_from_device(LFootSensor4);
  RFootNode5 = wb_supervisor_node_get_from_device(RFootSensor5);
  RFootNode6 = wb_supervisor_node_get_from_device(RFootSensor6);
  RFootNode7 = wb_supervisor_node_get_from_device(RFootSensor7);
  RFootNode8 = wb_supervisor_node_get_from_device(RFootSensor8);
}


// Find the objects in the scene and enable them
static void setup_devices(int time_step) {
  // Motors
  RShoulderPitchMotor = wb_robot_get_device("RShoulderPitchMotor");
  RShoulderYawMotor = wb_robot_get_device("RShoulderYawMotor");
  RElbowYawMotor = wb_robot_get_device("RElbowYawMotor");
  
  LShoulderPitchMotor = wb_robot_get_device("LShoulderPitchMotor");
  LShoulderYawMotor = wb_robot_get_device("LShoulderYawMotor");
  LElbowYawMotor = wb_robot_get_device("LElbowYawMotor");
  
  RHipYawMotor = wb_robot_get_device("RHipYawMotor");
  RHipPitchMotor = wb_robot_get_device("RHipPitchMotor");
  RKneePitchMotor = wb_robot_get_device("RKneePitchMotor");
  RAnklePitchMotor = wb_robot_get_device("RAnklePitchMotor");
  RAnkleYawMotor = wb_robot_get_device("RAnkleYawMotor");
  
  LHipYawMotor = wb_robot_get_device("LHipYawMotor");
  LHipPitchMotor = wb_robot_get_device("LHipPitchMotor");
  LKneePitchMotor = wb_robot_get_device("LKneePitchMotor");
  LAnklePitchMotor = wb_robot_get_device("LAnklePitchMotor");
  LAnkleYawMotor = wb_robot_get_device("LAnkleYawMotor");
  
  NeckRollMotor = wb_robot_get_device("NeckRollMotor");
  
  // Motor torque feedback
  // Disable to improve performance
  /*
  wb_motor_enable_torque_feedback(RShoulderPitchMotor, time_step);
  wb_motor_enable_torque_feedback(RShoulderYawMotor, time_step);
  wb_motor_enable_torque_feedback(RElbowYawMotor, time_step);
  
  wb_motor_enable_torque_feedback(LShoulderPitchMotor, time_step);
  wb_motor_enable_torque_feedback(LShoulderYawMotor, time_step);
  wb_motor_enable_torque_feedback(LElbowYawMotor, time_step);
  
  wb_motor_enable_torque_feedback(RHipYawMotor, time_step);
  wb_motor_enable_torque_feedback(RHipPitchMotor, time_step);
  wb_motor_enable_torque_feedback(RKneePitchMotor, time_step);
  wb_motor_enable_torque_feedback(RAnklePitchMotor, time_step);
  wb_motor_enable_torque_feedback(RAnkleYawMotor, time_step);
  
  wb_motor_enable_torque_feedback(LHipYawMotor, time_step);
  wb_motor_enable_torque_feedback(LHipPitchMotor, time_step);
  wb_motor_enable_torque_feedback(LKneePitchMotor, time_step);
  wb_motor_enable_torque_feedback(LAnklePitchMotor, time_step);
  wb_motor_enable_torque_feedback(LAnkleYawMotor, time_step);
  
  wb_motor_enable_torque_feedback(NeckRollMotor, time_step);
  */
  
  // Motor Sensors
  // Comment out to improve performance
  /*
  RShoulderPitchSensor = wb_robot_get_device("RShoulderPitchSensor");
  RShoulderYawSensor = wb_robot_get_device("RShoulderYawSensor");
  RElbowYawSensor = wb_robot_get_device("RElbowYawSensor");
  
  LShoulderPitchSensor = wb_robot_get_device("LShoulderPitchSensor");
  LShoulderYawSensor = wb_robot_get_device("LShoulderYawSensor");
  LElbowYawSensor = wb_robot_get_device("LElbowYawSensor");
  
  RHipYawSensor = wb_robot_get_device("RHipYawSensor");
  RHipPitchSensor = wb_robot_get_device("RHipPitchSensor");
  RKneePitchSensor = wb_robot_get_device("RKneePitchSensor");
  RAnklePitchSensor = wb_robot_get_device("RAnklePitchSensor");
  RAnkleYawSensor = wb_robot_get_device("RAnkleYawSensor");
  
  LHipYawSensor = wb_robot_get_device("LHipYawSensor");
  LHipPitchSensor = wb_robot_get_device("LHipPitchSensor");
  LKneePitchSensor = wb_robot_get_device("LKneePitchSensor");
  LAnklePitchSensor = wb_robot_get_device("LAnklePitchSensor");
  LAnkleYawSensor = wb_robot_get_device("LAnkleYawSensor");
  
  NeckRollSensor = wb_robot_get_device("NeckRollSensor"); 
  
  wb_position_sensor_enable(RShoulderPitchSensor, time_step);
  wb_position_sensor_enable(RShoulderYawSensor, time_step);
  wb_position_sensor_enable(RElbowYawSensor, time_step);
  
  wb_position_sensor_enable(LShoulderPitchSensor, time_step);  
  wb_position_sensor_enable(LShoulderYawSensor, time_step);  
  wb_position_sensor_enable(LElbowYawSensor, time_step);  
  
  wb_position_sensor_enable(RHipYawSensor, time_step);  
  wb_position_sensor_enable(RHipPitchSensor, time_step);  
  wb_position_sensor_enable(RKneePitchSensor, time_step);  
  wb_position_sensor_enable(RAnklePitchSensor, time_step);  
  wb_position_sensor_enable(RAnkleYawSensor, time_step);  
  
  wb_position_sensor_enable(LHipYawSensor, time_step); 
  wb_position_sensor_enable(LHipPitchSensor, time_step);  
  wb_position_sensor_enable(LKneePitchSensor, time_step);  
  wb_position_sensor_enable(LAnklePitchSensor, time_step);   
  wb_position_sensor_enable(LAnkleYawSensor, time_step);  
  
  wb_position_sensor_enable(NeckRollSensor, time_step);
  */
  
  // Force Sensors
  LFootSensor1 = wb_robot_get_device("LFootSensor1");
  LFootSensor2 = wb_robot_get_device("LFootSensor2");
  LFootSensor3 = wb_robot_get_device("LFootSensor3");
  LFootSensor4 = wb_robot_get_device("LFootSensor4");
  RFootSensor5 = wb_robot_get_device("RFootSensor5");
  RFootSensor6 = wb_robot_get_device("RFootSensor6");
  RFootSensor7 = wb_robot_get_device("RFootSensor7");
  RFootSensor8 = wb_robot_get_device("RFootSensor8");
  
  wb_touch_sensor_enable(LFootSensor1, time_step);
  wb_touch_sensor_enable(LFootSensor2, time_step);
  wb_touch_sensor_enable(LFootSensor3, time_step);
  wb_touch_sensor_enable(LFootSensor4, time_step);
  wb_touch_sensor_enable(RFootSensor5, time_step);
  wb_touch_sensor_enable(RFootSensor6, time_step);
  wb_touch_sensor_enable(RFootSensor7, time_step);
  wb_touch_sensor_enable(RFootSensor8, time_step);
  
  // Camera
  camera = wb_robot_get_device("camera");
  wb_camera_enable(camera, time_step);
}


// Set motors to default positions
static void default_position() {
  wb_motor_set_position(RShoulderPitchMotor, 1.57);
  wb_motor_set_position(RShoulderYawMotor, 2.44);
  wb_motor_set_position(RElbowYawMotor, 2.88);
  
  wb_motor_set_position(LShoulderPitchMotor, 1.57);
  wb_motor_set_position(LShoulderYawMotor, 0.70);
  wb_motor_set_position(LElbowYawMotor, 0.26);
  
  wb_motor_set_position(RHipYawMotor, 1.57);
  wb_motor_set_position(RHipPitchMotor, 1.05);
  wb_motor_set_position(RKneePitchMotor, 1.33);
  wb_motor_set_position(RAnklePitchMotor, 1.92);
  wb_motor_set_position(RAnkleYawMotor, 1.57);
  
  wb_motor_set_position(LHipYawMotor, 1.57);
  wb_motor_set_position(LHipPitchMotor, 2.09);
  wb_motor_set_position(LKneePitchMotor, 1.81);
  wb_motor_set_position(LAnklePitchMotor, 1.22);
  wb_motor_set_position(LAnkleYawMotor, 1.57);
  
  wb_motor_set_position(NeckRollMotor, 1.57);
}


// Print CoM data
static void print_com(FILE *fp, double start_time) {
  double out_time = wb_robot_get_time() - start_time;
  const double *com = wb_supervisor_node_get_center_of_mass(Robot_Node);
  fprintf(fp, "%.3f,%.6f,%.6f,%.6f\n", out_time, com[0], com[1], com[2]);
}


// Print pressure data
static void print_pressure(FILE *fp, double start_time) {
  double out_time = wb_robot_get_time() - start_time;
  const double *sensor1Loc = wb_supervisor_node_get_position(LFootNode1);
  const double *sensor2Loc = wb_supervisor_node_get_position(LFootNode2);
  const double *sensor3Loc = wb_supervisor_node_get_position(LFootNode3);
  const double *sensor4Loc = wb_supervisor_node_get_position(LFootNode4);
  const double *sensor5Loc = wb_supervisor_node_get_position(RFootNode5);
  const double *sensor6Loc = wb_supervisor_node_get_position(RFootNode6);
  const double *sensor7Loc = wb_supervisor_node_get_position(RFootNode7);
  const double *sensor8Loc = wb_supervisor_node_get_position(RFootNode8);
  const double sensor1Val = wb_touch_sensor_get_value(LFootSensor1);
  const double sensor2Val = wb_touch_sensor_get_value(LFootSensor2);
  const double sensor3Val = wb_touch_sensor_get_value(LFootSensor3);
  const double sensor4Val = wb_touch_sensor_get_value(LFootSensor4);
  const double sensor5Val = wb_touch_sensor_get_value(RFootSensor5);
  const double sensor6Val = wb_touch_sensor_get_value(RFootSensor6);
  const double sensor7Val = wb_touch_sensor_get_value(RFootSensor7);
  const double sensor8Val = wb_touch_sensor_get_value(RFootSensor8);
  fprintf(fp, "%.3f,", out_time);
  fprintf(fp, "%.6f,%.6f,%.6f,", sensor1Loc[0], sensor1Loc[2], sensor1Val);
  fprintf(fp, "%.6f,%.6f,%.6f,", sensor2Loc[0], sensor2Loc[2], sensor2Val);
  fprintf(fp, "%.6f,%.6f,%.6f,", sensor3Loc[0], sensor3Loc[2], sensor3Val);
  fprintf(fp, "%.6f,%.6f,%.6f,", sensor4Loc[0], sensor4Loc[2], sensor4Val);
  fprintf(fp, "%.6f,%.6f,%.6f,", sensor5Loc[0], sensor5Loc[2], sensor5Val);
  fprintf(fp, "%.6f,%.6f,%.6f,", sensor6Loc[0], sensor6Loc[2], sensor6Val);
  fprintf(fp, "%.6f,%.6f,%.6f,", sensor7Loc[0], sensor7Loc[2], sensor7Val);
  fprintf(fp, "%.6f,%.6f,%.6f\n", sensor8Loc[0], sensor8Loc[2], sensor8Val);
}


// Main function
int main(int argc, char **argv) {
  // Do webots initialization
  wb_robot_init();
  int time_step = wb_robot_get_basic_time_step();
  
  // Do robot initialization
  setup_devices(time_step);
  setup_nodes();
  
  // Prepare CoM output file
  FILE *com_file = NULL;
  com_file = fopen("../../output/com_info.csv","w+");
  
  // Prepare pressure output file
  FILE *pressure_file = NULL;
  pressure_file = fopen("../../output/pressure_info.csv", "w+");
  
  // Load the motion file
  WbMotionRef YansheeMotion = wbu_motion_new("../../motions/Dance.motion");
  
  // Set robot to default pose, give it 500ms to get there
  default_position();
  wb_robot_step(500);
  
  // Set up timer
  double start_time = wb_robot_get_time();
  wb_supervisor_movie_start_recording("../../output/simvid.mp4", 1920, 1080, 0, 100, 1, false);

  // Collect data for a short time before motion starts
  int i;
  for (i = 0; i < STEPS_PRE; i++) {
    print_com(com_file, start_time);
    print_pressure(pressure_file, start_time);
    wb_robot_step(time_step);
  }
  
  // Run motion until it completes
  // Collect data during runtime
  wbu_motion_play(YansheeMotion);
  while (!wbu_motion_is_over(YansheeMotion)) {
    print_com(com_file, start_time);
    print_pressure(pressure_file, start_time);
    wb_robot_step(time_step);
  }
  
  // Collect data for a short time after motion ends
  for (i = 0; i < STEPS_POST; i++) {
    print_com(com_file, start_time);
    print_pressure(pressure_file, start_time);
    wb_robot_step(time_step);
  }
  wb_supervisor_movie_stop_recording();
  
  // Close files
  fclose(com_file);
  fclose(pressure_file);
   
  // Cleanup webots resources and exit
  wb_robot_cleanup();
  return 0;
}
