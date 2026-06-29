
#include <tf/transform_listener.h>
#include "ros/ros.h"
#include "visualization_msgs/Marker.h"


int main(int argc, char** argv){
  ros::init(argc, argv, "tf_listener");
  ros::NodeHandle nh;

  ros::Publisher marker_pub = nh.advertise<visualization_msgs::Marker>("path_visualize", 10);


  tf::TransformListener listener;
  visualization_msgs::Marker points;
  points.header.frame_id = "/map";
  points.header.stamp = ros::Time::now();
  points.ns = "points_and_lines";
  points.action = visualization_msgs::Marker::ADD;
  points.pose.orientation.w = 1.0;

  points.id = 0;
  points.type = visualization_msgs::Marker::LINE_STRIP;

  points.scale.x = 0.05; // Line width
  points.color.b = 1.0;
  points.color.a = 1.0;

  ros::Rate rate(10.0);
  while (ros::ok()){
    tf::StampedTransform transform;
    try{
      listener.lookupTransform("/map", "/base_footprint", ros::Time(0), transform);

      geometry_msgs::Point p;
      p.x = transform.getOrigin().x();
      p.y = transform.getOrigin().y();
      p.z = transform.getOrigin().z();

      points.points.push_back(p);
      marker_pub.publish(points);
    }
    catch (tf::TransformException &ex){
      ROS_ERROR("%s",ex.what());
      ros::Duration(1.0).sleep();
    }
    rate.sleep();
  }
  return 0;
};
