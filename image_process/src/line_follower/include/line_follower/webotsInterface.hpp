/**********************************************************
**  Description: 此文件为 Webots仿真接口程序 头文件
**  Author     : ssc
**********************************************************/
#ifndef _WEBOTSINTERFACE_H_
#define _WEBOTSINTERFACE_H_

#include <ros/ros.h>
#include <std_msgs/String.h>
#include <sensor_msgs/Image.h>
#include <nav_msgs/Odometry.h>
#include <webots_ros/set_float.h>
#include <webots_ros/set_int.h>
#include <webots_ros/Float64Stamped.h>
#include <webots_ros/BoolStamped.h>
#include <webots_ros/Int32Stamped.h>
#include <opencv2/opencv.hpp>


void webotsInit();
void enableKeyboard();
void pid_init(float Kp,float Ki,float Kd);

#endif