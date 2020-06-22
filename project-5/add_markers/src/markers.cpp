#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>

class CustomMarker
{
public:
  visualization_msgs::Marker marker;
  double tolerance_x = 0.4;
  double tolerance_y = 0.4;
  double goal1_x = 2.20;
  double goal1_y = -4.31;
  double goal2_x = -2.27;
  double goal2_y = -7.03;
  //int flag  = 0;
  CustomMarker()
  {
    //Topic you want to publish
    pub_ = n_.advertise<visualization_msgs::Marker>("visualization_marker", 1);
    //Topic you want to subscribe
    sub_ = n_.subscribe("/amcl_pose", 1, &CustomMarker::callback, this);
    uint32_t shape = visualization_msgs::Marker::CUBE;
    marker.header.frame_id = "map";
    marker.header.stamp = ros::Time::now();
    marker.ns = "basic_shapes";
    marker.id = 0;
    marker.action = visualization_msgs::Marker::ADD;
    marker.pose.position.x = 0;
    marker.pose.position.y = 0;
    marker.pose.position.z = 0;
    marker.pose.orientation.x = 0.0;
    marker.pose.orientation.y = 0.0;
    marker.pose.orientation.z = 0.0;
    marker.pose.orientation.w = 1.0;
    marker.scale.x = 0.5;
    marker.scale.y = 0.5;
    marker.scale.z = 0.5;

    // Set the color -- be sure to set alpha to something non-zero!
    marker.color.r = 0.0f;
    marker.color.g = 1.0f;
    marker.color.b = 0.0f;
    marker.color.a = 1.0;

    marker.lifetime = ros::Duration();

  }
  void custom_publisher(float x,float y) 
  {   
    marker.pose.position.x = x;
    marker.pose.position.y = y;
     if (pub_.getNumSubscribers() < 1)
    {
      ROS_WARN_ONCE("Please create a subscriber to the marker");
    }
     ROS_INFO("The action message now is %d",marker.action);
    

    // Set the scale of the marker -- 1x1x1 here means 1m on a side
    
    pub_.publish(marker);
  }
  void callback(const geometry_msgs::PoseWithCovarianceStamped input)
  {
    float valx = std::abs(std::abs(goal1_x)-std::abs(input.pose.pose.position.x));
    float valy = std::abs(std::abs(goal1_y)-std::abs(input.pose.pose.position.y));

    float val2x = std::abs(std::abs(goal2_x)-std::abs(input.pose.pose.position.x));
    float val2y = std::abs(std::abs(goal2_y)-std::abs(input.pose.pose.position.y));

    ROS_INFO("The x = %f and y = %f for  first mission ",valx,valy);
    ROS_INFO("The x = %f and y = %f for  second mission ",val2x,val2y);

    if(    valx  <= tolerance_x && 
     valy <= tolerance_y){
         marker.action = visualization_msgs::Marker::DELETE;
         custom_publisher(goal1_x,goal1_y);
    }
    else if(val2x<= tolerance_x  && val2y <= tolerance_y){
         marker.action = visualization_msgs::Marker::ADD;
         custom_publisher(goal2_x,goal2_y);
    }
    else{
         custom_publisher(goal1_x,goal1_y);
    }
    
  }

private:
  ros::NodeHandle n_; 
  ros::Publisher pub_;
  ros::Subscriber sub_;

};

int main(int argc, char **argv)
{
  //Initiate ROS
  ros::init(argc, argv, "add_marker");
  ros::NodeHandle nh;
  ros::Rate r(10);
  //Create an object of class 
  CustomMarker newMarker;
  
  //newMarker.custom_publisher(2.20,-4.31);
  ros::spin();

  return 0;
}
