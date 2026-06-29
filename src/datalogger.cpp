#include <ros/ros.h>
#include <nav_msgs/OccupancyGrid.h>
#include <fstream>
#include <vector>
#include <sstream>
#include <ctime>

class GridMapLogger {
public:
    GridMapLogger() {
        sub = nh.subscribe("map", 10, &GridMapLogger::mapCallback, this);
        timer = nh.createTimer(ros::Duration(1), &GridMapLogger::timerCallback, this);
        start_time = std::time(nullptr);  // 记录启动时间
        time_count = 0;
    }

    void mapCallback(const nav_msgs::OccupancyGrid::ConstPtr& msg) {
        current_map = *msg;
        map_received = true;
    }

    void timerCallback(const ros::TimerEvent&) {
        if (map_received) {
            int free_count = countFreeCell(current_map);
            free_counts.push_back(free_count);
            time_count++;
            ROS_INFO("%d Number of free cells: %d", time_count,free_count);
        }
    }

    void saveCountsToFile() {
        std::ostringstream filename_stream;
        filename_stream <<"free_counts_" << std::put_time(std::localtime(&start_time), "%Y%m%d_%H%M%S") << ".txt";
        std::string filename = filename_stream.str();

        std::ofstream file(filename);
        if (file.is_open()) {
            for (int count : free_counts) {
                file << count << ",";
            }
            file.close();
        } else {
            ROS_ERROR("Unable to open file: %s", filename.c_str());
        }
    }

private:
    int countFreeCell(const nav_msgs::OccupancyGrid& map) {
        int free_count = 0;
        for (auto cell : map.data) {
            if (cell == 0) free_count++;
        }
        return free_count;
    }

    ros::NodeHandle nh;
    ros::Subscriber sub;
    ros::Timer timer;
    nav_msgs::OccupancyGrid current_map;
    bool map_received = false;
    std::vector<int> free_counts;
    std::time_t start_time;
    std::int32_t time_count;
};

int main(int argc, char** argv) {
    ros::init(argc, argv, "grid_map_logger");
    GridMapLogger logger;
    ros::spin();
    logger.saveCountsToFile();
    return 0;
}


