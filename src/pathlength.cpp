#include <ros/ros.h>
#include <nav_msgs/Odometry.h>
#include <cmath>
#include <vector>
#include <fstream>
#include <sstream>

class PathLengthCalculator {
public:
    PathLengthCalculator() : total_distance_(0.0),point_pos_x(0.0),point_pos_y(0.0), last_x_(0.0), last_y_(0.0), first_measurement_(true) {
        ros::NodeHandle nh;
        odom_sub_ = nh.subscribe("/odom", 10, &PathLengthCalculator::odomCallback, this);
        timer_ = nh.createTimer(ros::Duration(1), &PathLengthCalculator::timerCallback, this);
    }

    void odomCallback(const nav_msgs::Odometry::ConstPtr& msg) {
        double x = msg->pose.pose.position.x;
        double y = msg->pose.pose.position.y;
        point_pos_x = x;
        point_pos_y = y;

        if (!first_measurement_) {
            double dx = x - last_x_;
            double dy = y - last_y_;
            total_distance_ += sqrt(dx * dx + dy * dy);
        } else {
            first_measurement_ = false;
        }
        last_x_ = x;
        last_y_ = y;
    }

    void timerCallback(const ros::TimerEvent&) {
        ROS_INFO("Total path length at %f seconds: %f meters", ros::Time::now().toSec(), total_distance_);
        distance_log_.push_back(total_distance_);
        x_log_.push_back(point_pos_x);
        y_log_.push_back(point_pos_y);
        saveDistancesToFile();
    }

    void saveDistancesToFile() {
        std::ofstream file("path_length_log.txt");
        for (double distance : distance_log_) {
            file << distance << ",";
        }
        file << "\n";
        file.close();

        file.open("point_x_log.txt");
        for (double pos_x : x_log_) {
            file << pos_x << ",";
        }
        file << "\n";
        file.close();

        file.open("point_y_log.txt");
        for (double pos_y : y_log_) {
            file << pos_y << ",";
        }
        file << "\n";
        file.close();
    }

private:
    ros::Subscriber odom_sub_;
    ros::Timer timer_;
    double total_distance_;
    double point_pos_x;
    double point_pos_y;
    double last_x_;
    double last_y_;
    bool first_measurement_;
    std::vector<double> distance_log_;
    std::vector<double> x_log_;
    std::vector<double> y_log_;
};

int main(int argc, char** argv) {
    ros::init(argc, argv, "path_length_calculator");
    PathLengthCalculator calculator;
    ros::spin();
    return 0;
}
