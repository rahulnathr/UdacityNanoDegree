#!/bin/sh
xterm  -e  "source /home/workspace/catkin_ws/devel/setup.bash; roslaunch turtlebot_gazebo turtlebot_world.launch" &
sleep 5
xterm -e " source /home/workspace/catkin_ws/devel/setup.bash; roslaunch turtlebot_gazebo amcl_demo.launch " &
sleep 18

xterm -e "  source /home/workspace/catkin_ws/devel/setup.bash; roslaunch my_robot customrvizlauncher.launch " &
sleep 15

xterm -e " source /home/workspace/catkin_ws/devel/setup.bash;rosrun add_markers add_markers_1"

