#include <vector>
#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>

double X;
double Y;

void get_poses(const geometry_msgs::PoseWithCovarianceStamped::ConstPtr& msg)
{  
    X = msg->pose.pose.position.x;
    Y = msg->pose.pose.position.y;
    ROS_INFO("X = %f", X);
    ROS_INFO("Y = %f", Y);
}
double dist_total(double obj_X, double obj_Y, double X, double Y)
{  
  double dist_x = obj_X - X;
  double dist_y = obj_Y - Y;
  double dist = sqrt(pow(dist_y, 2) + pow(dist_x, 2)); 
  return dist; 
}  
  
int main( int argc, char** argv )
{
  ros::init(argc, argv, "basic_shapes");
  ros::NodeHandle n;
  ros::Rate r(1);
  ros::Publisher marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 1);
  ros::Subscriber sub = n.subscribe("/amcl_pose", 1000, get_poses);
  bool picked = false;
  double alpha = 0.4;  


double mark_x = 2.0;
  double mark_y = 0.0;

    
  // Set our initial shape type to be a cube
  uint32_t shape = visualization_msgs::Marker::CUBE;
  while (ros::ok())
  {
      
    visualization_msgs::Marker marker;
    // Set the frame ID and timestamp.  See the TF tutorials for information on these.
    marker.header.frame_id = "/map";
    marker.header.stamp = ros::Time::now();

    // Set the namespace and id for this marker.  This serves to create a unique ID
    // Any marker sent with the same namespace and id will overwrite the old one
    marker.ns = "basic_shapes";
    marker.id = 0;

    // Set the marker type.  Initially this is CUBE, and cycles between that and SPHERE, ARROW, and CYLINDER
    marker.type = shape;

    // Set the marker action.  Options are ADD, DELETE, and new in ROS Indigo: 3 (DELETEALL)
    
    marker.action = visualization_msgs::Marker::ADD;

    // Set the pose of the marker.  This is a full 6DOF pose relative to the frame/time specified in the header
    marker.pose.position.x = mark_x;
    marker.pose.position.y = mark_y;
    marker.pose.position.z = 0;
    marker.pose.orientation.x = 0.0;
    marker.pose.orientation.y = 0.0;
    marker.pose.orientation.z = 0.0;
    marker.pose.orientation.w = 1.0;

    // Set the scale of the marker -- 1x1x1 here means 1m on a side
    marker.scale.x = 0.3;
    marker.scale.y = 0.3;
    marker.scale.z = 0.3;

    // Set the color -- be sure to set alpha to something non-zero!
    marker.color.r = 0.0f;
    marker.color.g = 0.0f;
    marker.color.b = 1.0f;
    marker.color.a = 1.0;

    marker.lifetime = ros::Duration();

    // Publish the marker
    while (marker_pub.getNumSubscribers() < 1)
    {
      if (!ros::ok())
      {
        return 0;
      }
      ROS_WARN_ONCE("Please create a subscriber to the marker");
      sleep(1);
    }

       if(picked == false)
 {
		marker_pub.publish(marker);
	        sleep(2);
}

		double dist = dist_total(mark_x, mark_y, X, Y);                          
     if(dist <= alpha && picked == false)
   {  
     ROS_INFO("robot is about to pick the object");
      marker.action = visualization_msgs::Marker::DELETE;
      marker_pub.publish(marker);
      picked = true;
      mark_x=3.0;
      mark_y=-4.0;
      marker.action = visualization_msgs::Marker::ADD;
     }
          dist = dist_total(mark_x,mark_y, X, Y);
  if(dist <= alpha && picked == true)
      {
       ROS_INFO("robot is at the drop-off location");
       marker_pub.publish(marker);
       }     
    ros::spinOnce();
    r.sleep();
  }

 
}
     

     
