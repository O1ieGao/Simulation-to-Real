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

static WbDeviceTag RShoulderPitchSensor, RShoulderYawSensor, RElbowYawSensor;
static WbDeviceTag LShoulderPitchSensor, LShoulderYawSensor, LElbowYawSensor;
static WbDeviceTag RHipYawSensor, RHipPitchSensor, RKneePitchSensor, RAnklePitchSensor, RAnkleYawSensor;
static WbDeviceTag LHipYawSensor, LHipPitchSensor, LKneePitchSensor, LAnklePitchSensor, LAnkleYawSensor;
static WbDeviceTag NeckRollSensor;

static WbDeviceTag RShoulderPitchMotor, RShoulderYawMotor, RElbowYawMotor;
static WbDeviceTag LShoulderPitchMotor, LShoulderYawMotor, LElbowYawMotor;
static WbDeviceTag RHipYawMotor, RHipPitchMotor, RKneePitchMotor, RAnklePitchMotor, RAnkleYawMotor;
static WbDeviceTag LHipYawMotor, LHipPitchMotor, LKneePitchMotor, LAnklePitchMotor, LAnkleYawMotor;
static WbDeviceTag NeckRollMotor;

static WbDeviceTag camera;

static void find_and_enable_devices(int time_step) {
  // Motor Sensors
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
  
  // Camera
  camera = wb_robot_get_device("camera");
  wb_camera_enable(camera, time_step);
}

// Set velocities to avoid sending the robot into orbit
// Values taken from data sheet and converted from sec/deg to rad/sec
static void set_motor_velocities() {
  wb_motor_set_velocity(RShoulderPitchMotor, 5.28887);
  wb_motor_set_velocity(RShoulderYawMotor, 5.28887);
  wb_motor_set_velocity(RElbowYawMotor, 5.28887);
  
  wb_motor_set_velocity(LShoulderPitchMotor, 5.28887);
  wb_motor_set_velocity(LShoulderYawMotor, 5.28887);
  wb_motor_set_velocity(LElbowYawMotor, 5.28887);
  
  wb_motor_set_velocity(RHipYawMotor, 5.28887);
  wb_motor_set_velocity(RHipPitchMotor, 5.28887);
  wb_motor_set_velocity(RKneePitchMotor, 5.28887);
  wb_motor_set_velocity(RAnklePitchMotor, 5.28887);
  wb_motor_set_velocity(RAnkleYawMotor, 5.28887);
  
  wb_motor_set_velocity(LHipYawMotor, 5.28887);
  wb_motor_set_velocity(LHipPitchMotor, 5.28887);
  wb_motor_set_velocity(LKneePitchMotor, 5.28887);
  wb_motor_set_velocity(LAnklePitchMotor, 5.28887);
  wb_motor_set_velocity(LAnkleYawMotor, 5.28887);
  
  wb_motor_set_velocity(NeckRollMotor, 5.28887);
}

// Set all motors to zero
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


int main(int argc, char **argv) {
  // Initialize everything for webots
  wb_robot_init();
  int time_step = wb_robot_get_basic_time_step();
  find_and_enable_devices(time_step);
  set_motor_velocities();
  
  // Ensure robot is in default pose and wait a second before starting
  default_position();
  wb_robot_step(1000);
  
  // Do the motion defined in a file
  WbMotionRef YansheeMotion = wbu_motion_new("../../motions/Dance.motion");
  wbu_motion_play(YansheeMotion);
  while (wb_robot_step(time_step) != -1) {
  // wait for termination of motion
    while (!wbu_motion_is_over(YansheeMotion)){
        // Step forward in time
        wb_robot_step(time_step);
     }
  };
  
  // Cleanup resources
  wb_robot_cleanup();
  return 0;
}
