/*
ActivMedia Robotics Interface for Applications (ARIA)
Copyright (C) 2004,2005 ActivMedia Robotics, LLC


This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

If you wish to redistribute ARIA under different terms, contact 
ActivMedia Robotics for information about a commercial version of ARIA at 
robots@activmedia.com or 
ActivMedia Robotics, 19 Columbia Drive, Amherst, NH 03031; 800-639-9481

*/

#include "Aria.h"
#include <cmath>
/// Action to deal with if the bumpers trigger
/**
This class basically responds to the bumpers the robot has, what
the activity things the robot has is decided by the param file.  If
the robot is going forwards and bumps into something with the front
bumpers, it will back up and turn.  If the robot is going backwards
and bumps into something with the rear bumpers then the robot will
move forward and turn.  
*/

class TaskShowCoord
{
public:
	/// Constructor
	TaskShowCoord(ArRobot *robot, ArPose *goal);
	void doTask(void);
	ArRobot *myRobot;  //定义操纵的小车类
	//ArRobotParams *rr;
	ArPose *mygoal;    //定义目的地
	ArFunctorC<TaskShowCoord> tasks;
	bool isArrived;
};


