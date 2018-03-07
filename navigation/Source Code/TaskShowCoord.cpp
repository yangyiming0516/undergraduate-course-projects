#include "TaskShowCoord.h"
TaskShowCoord::TaskShowCoord(ArRobot *robot, ArPose *goal):tasks(this,&TaskShowCoord::doTask){
	myRobot = robot;
	//rr = robot;
	mygoal = goal;
	myRobot->addUserTask("StateTask", 50, &tasks);
	isArrived = false;
}
	

void TaskShowCoord::doTask(void){
 /*
	printf("\rx %6.1f  y %6.1f  tth  %6.1f vel %7.1f mpacs %3d", myRobot->getX(),
	 myRobot->getY(), myRobot->getTh(), myRobot->getVel(), 
	 myRobot->getMotorPacCount());
	 */
	
	printf("\nl:%d r:%d vel:%.0lf location:(%3.3f, %3.3f, %3.3f)", myRobot->getSonarRange(0), myRobot->getSonarRange(7), myRobot->getVel(), myRobot->getX(), myRobot->getY(), myRobot->getTh());//myRobot->getRobotParams()->getLaserX(), myRobot->getRobotParams()->getLaserY());
	if(!isArrived && abs(myRobot->getX()-mygoal->getX())<300 && abs(myRobot->getY()-mygoal->getY())<300){
		  printf("\narrived.\n");
		  isArrived = true;
		  myRobot->stop();
		  Aria::exit(0);
	}
}