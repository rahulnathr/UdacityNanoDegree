#include "ros/ros.h"
#include "ball_chaser/DriveToTarget.h"
#include <sensor_msgs/Image.h>

// Define a global client that can request services
ros::ServiceClient client;

// This function calls the command_robot service to drive the robot in the specified direction
void drive_robot(float lin_x, float ang_z)
{
    // TODO: Request a service and pass the velocities to it to drive the robot
    ball_chaser::DriveToTarget srv;
    srv.request.linear_x = lin_x;
    srv.request.angular_z = ang_z;

    if(!client.call(srv)){
        ROS_ERROR("Failed to call service command_robot");
    }
}

// This callback function continuously executes and reads the image data
void process_image_callback(const sensor_msgs::Image img)
{	// Code inspired from https://knowledge.udacity.com/questions/222124
    //I'm checking the image into three regions based on left and right using basic maths.
    int white_pixel = 255;
    int detected_pixel_value =-1;
    int left_region = img.step * 0.3;
    int right_region = img.step*0.7;
    int column_value_based_on_step;
    
    // TODO: Loop through each pixel in the image and check if there's a bright white one
    // Then, identify if this pixel falls in the left, mid, or right side of the image
    // Depending on the white ball position, call the drive_bot function and pass velocities to it
    // Request a stop when there's no white ball seen by the camera
    for(int i=0;i<img.height * img.step;i++){
        if((img.data[i] == white_pixel)&&(img.data[i+1]==white_pixel)&&(img.data[i+2]==white_pixel)){
            detected_pixel_value = i;
            break;
        }
    }
    column_value_based_on_step = detected_pixel_value % img.step;
    if(column_value_based_on_step>=0 && column_value_based_on_step<left_region && detected_pixel_value!=-1){
        drive_robot(0.0,0.2);
    }else if (column_value_based_on_step>=left_region && column_value_based_on_step<right_region && detected_pixel_value!=-1 )
    {
        drive_robot(0.3,0.0);
    }else if (column_value_based_on_step>=right_region && column_value_based_on_step<img.step && detected_pixel_value!=-1)
    {
        drive_robot(0.0,-0.2);
    }else
    {
        drive_robot(0.0,0.0);
    }
    // ROS_INFO("The value optained for step is %d",img.step);
    // ROS_INFO("The value optained for column value is %d",column_value_based_on_step);
    // ROS_INFO("The value optained for left region is %d",left_region);
    // ROS_INFO("The value optained for right is %d",right_region);
    ROS_INFO("The value optained for pixel is %d",detected_pixel_value);
}

int main(int argc, char** argv)
{
    // Initialize the process_image node and create a handle to it
    ros::init(argc, argv, "process_image");
    ros::NodeHandle n;

    // Define a client service capable of requesting services from command_robot
    client = n.serviceClient<ball_chaser::DriveToTarget>("/ball_chaser/command_robot");

    // Subscribe to /camera/rgb/image_raw topic to read the image data inside the process_image_callback function
    ros::Subscriber sub1 = n.subscribe("/camera/rgb/image_raw", 10, process_image_callback);

    // Handle ROS communication events
    ros::spin();

    return 0;
}
