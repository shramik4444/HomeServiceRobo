#!/bin/sh

export TURTLEBOT_GAZEBO_WORLD_FILE="$(rospack find my_robot)/worlds/myworld.world"
export TURTLEBOT_GAZEBO_MAP_FILE="$(rospack find my_robot)/maps/map.yaml"
export ROBOT_INITIAL_POSE="-x 0 -y 0 -Y 1.5708"

xterm -e "roslaunch turtlebot_gazebo turtlebot_world.launch" &
sleep 5
xterm -e "roslaunch turtlebot_gazebo amcl_demo.launch " &
sleep 5
xterm -e "roslaunch turtlebot_rviz_launchers view_navigation.launch"

