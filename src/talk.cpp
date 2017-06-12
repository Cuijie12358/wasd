#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <sstream>
#include <stdio.h>
#include <cstdlib>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

ros::Publisher cmdVelPub;
int kbhit(void);
char getch(void);


int main(int argc, char **argv)
{
  ros::init(argc, argv, "talk");
  std::string topic = "/turtle1/cmd_vel"; //topic name
  ros::NodeHandle n;
  cmdVelPub = n.advertise<geometry_msgs::Twist>(topic, 1);
  char temp;
  ros::Rate loopRate(2);
  ROS_INFO("talk cpp start...");//输出显示信息
  geometry_msgs::Twist speed; // 控制信号载体 Twist message

  
	while(ros::ok())
	{
		while(kbhit())
		{
			temp = getch();
			switch (temp)
			{
				case 'w': 
					speed.linear.x = 1; // 设置线速度为1m/s，正为前进，负为后退
					cmdVelPub.publish(speed); // 将刚才设置的指令发送给机器人

					ros::spinOnce();
					loopRate.sleep();//按loopRate(2)设置的2HZ将程序挂起break;
				case 'a':
					
					speed.angular.z = 1; // 设置角速度为1rad/s，正为左转，负为右转
					cmdVelPub.publish(speed); // 将刚才设置的指令发送给机器人

					ros::spinOnce();
					loopRate.sleep();//按loopRate(2)设置的2HZ将程序挂起break;

					break;
				case 's':
					speed.linear.x = -1; // 设置线速度为1m/s，正为前进，负为后退
					cmdVelPub.publish(speed); // 将刚才设置的指令发送给机器人

					ros::spinOnce();
					loopRate.sleep();//按loopRate(2)设置的2HZ将程序挂起break;
					break;
				case 'd':
					speed.angular.z = -1; // 设置角速度为1rad/s，正为左转，负为右转
					cmdVelPub.publish(speed); // 将刚才设置的指令发送给机器人

					ros::spinOnce();
					loopRate.sleep();//按loopRate(2)设置的2HZ将程序挂起break;
					break;
				default:break;
			}
		}
	}
  return 0;
}

int kbhit(void)

{

    struct termios oldt, newt;

    int ch;

    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);

    newt = oldt;

    newt.c_lflag &= ~(ICANON | ECHO);

    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);

    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if(ch != EOF)

    {

        ungetc(ch, stdin);

        return 1;

    }

    return 0;

}

char getch()
{
	char buf = 0;
	struct termios old = {0};
	if(tcgetattr(0,&old) < 0)
		perror("tcsetattr()");
	old.c_lflag &= ~ICANON;
	old.c_lflag &= ~ECHO;
	old.c_cc[VMIN] = 1;
	old.c_cc[VTIME] = 0;
	if(tcsetattr(0,TCSANOW,&old) < 0)
		perror("tcsetattr ICANON");
	if(read(0,&buf,1) < 0)
		perror("read()");
	old.c_lflag |= ~ICANON;
	old.c_lflag |= ~ECHO;
	if(tcsetattr(0,TCSADRAIN,&old) < 0)
		perror("tcsetattr ~ICANON");
	return (buf);
}
