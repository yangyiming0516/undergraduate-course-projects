/*
Adept MobileRobots Robotics Interface for Applications (ARIA)
Copyright (C) 2004, 2005 ActivMedia Robotics LLC
Copyright (C) 2006, 2007, 2008, 2009, 2010 MobileRobots Inc.
Copyright (C) 2011, 2012, 2013 Adept Technology

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
Adept MobileRobots for information about a commercial version of ARIA at 
robots@mobilerobots.com or 
Adept MobileRobots, 10 Columbia Drive, Amherst, NH 03031; +1-603-881-7960
*/

#include "Aria.h"
#include <iostream>
#include "TaskShowCoord.h"
#include <algorithm>
#include <cmath>
#include <set>
#include <map>
#include <windows.h>
#include <cstdio>
using namespace std;

double eps=300;//设置误差
double fx,fy;
struct point{
    int num;
    int father;
    double x,y;
    double G;
	//double visited;
	double wall;
	double semiwall;
	double ner;
    double H(){
        return sqrt((fx-x)*(fx-x)+(fy-y)*(fy-y)) * (0.2 + 0.2*semiwall + 2*ner + 1*wall);
    }
    double F(){return H();}
    point(){
        num=father=0;
        x=y=G=0;
    }
};

struct mycmp{
    bool    operator () ( point b1,point b2){
        return b1.F()<b2.F();
    }
};


set<point,mycmp>  onset;
set<point,mycmp> offset;


int num=0;

ArActionStallRecover recover("stall recover", 200, 50, 150, -30);
  ArActionBumpers bumpers;
  ArActionAvoidFront avoidFrontNear("Avoid Front Near", 200, 30);
  ArActionAvoidFront avoidFrontFar;
  ArActionGoto gotoDes("goto");
  ArActionStop stopAction("stop");


void getnewpoints(point now,point* list, ArRobot &robot){
	//cout << "getnewpoints\n";

	robot.lock();
	std::map<int, ArLaser*> *lasers = robot.getLaserMap();
	std::map<int, ArLaser*>::const_iterator l = lasers->begin();
	ArLaser* laser = (*l).second;   //得到激光对象


    std::list<ArPoseWithTime*> *readings = laser->getCurrentBuffer();
	std::list<ArPoseWithTime *>::iterator it;

	
	double rx = robot.getX();
	double ry = robot.getY();

	int count = 0, c = 0;

	printf("Searching front:\n");
    for(it = readings->begin();it != readings->end();it++)
	{
		
		if(count++%10 != 0 && count != 179) continue;
		point n;
		n.wall = 0;
		double dx = (*it)->getX();
		double dy = (*it)->getY();
		//printf("dx %.f  dy %.f\n",dx,dy);
		//激光断点的像素坐标
		if((dx - rx)*(dx - fx) < 0 && (dy - ry)*(dy - fy) < 0) n.wall = 1;
		if((dx - rx)*(dx - fx) < 0 || (dy - ry)*(dy - fy) < 0) n.semiwall = 1;
		if(abs(dx-rx)<400 || abs(dy-ry) < 400) n.ner = 1;

		double laserMax = 2000, avoidWall = 300;
		double dis = sqrt((dx-rx)*(dx-rx) + (dy-ry)*(dy-ry));

		if (dis>laserMax) dx = rx+(dx-rx)*laserMax/dis;
		else if (rx < dx) dx -= avoidWall;
		else dx += avoidWall;

		if (dis>laserMax) dy = ry+(dy-ry)*laserMax/dis;
		else if(ry < dy) dy -= avoidWall;
		else dy += avoidWall;

		
		n.x = dx;
		n.y = dy;
		//n.visited = 0;
		dis = sqrt((dx-rx)*(dx-rx) + (dy-ry)*(dy-ry));
		n.G = now.G + dis;
		list[c++] = n;
		printf("point%d:%.0lf,%.0lf,%0.1f\n", c-1, n.x, n.y,n.F());
	}
	robot.unlock();
	/*
	printf("Searching back:\n");
	robot.setDeltaHeading(180);
	Sleep(6000);
	readings = laser->getCurrentBuffer();
	
	for(it = readings->begin();it != readings->end();it++)
	{
		if(count++%10 != 0 && count != 179) continue;
		double dx = (*it)->getX();
		double dy = (*it)->getY();
		//printf("dx %.f  dy %.f\n",dx,dy);
		//激光断点的像素坐标

		double laserMax = 2000, avoidWall = 400;
		double dis = sqrt((dx-rx)*(dx-rx) + (dy-ry)*(dy-ry));
		if (dis>laserMax) dx = rx+(dx-rx)*2000/dis;
		else if (rx < dx) dx -= avoidWall;
		else dx += avoidWall;

		if (dis>laserMax) dy = ry+(dy-ry)*2000/dis;
		else if(ry < dy) dy -= avoidWall;
		else dy += avoidWall;
		point n;
		n.x = dx;
		n.y = dy;
		dis = sqrt((dx-rx)*(dx-rx) + (dy-ry)*(dy-ry));
		n.G = now.G + dis;
		list[c++] = n;
		printf("point%d:%.0lf,%.0lf\n", c-1, n.x, n.y);
		
	}
	robot.setDeltaHeading(180);
	Sleep(6000);
	*/
	printf("*********************************\n");
	
}//除了num都要设置

void robotgoto(point p, ArRobot &robot){
	//new TaskShowCoord(&robot,new ArPose(fx, fy));
	robot.lock();
	double th = robot.getTh(), x = robot.getX(), y = robot.getY(), rotvel = robot.getRotVelMax();
	double px = p.x, py = p.y;
	robot.unlock();

	//cout << "robotgoto:" << px << "," << py << endl;
	printf("robotgoto:%.0lf, %.0lf\n", px, py);
	printf("now:%.0lf, %.0lf\n", x, y);

	double dx = px-x;
	double dy = py-y;
	double dis = sqrt(dx*dx+dy*dy);
	double ang = atan2(dy, dx)*180/3.1415926;
	printf("ang:%.0lf\ndis:%.0lf\n", (ang-th), dis);
	robot.setDeltaHeading(ang-th);
	printf("Turning\n");
	do{
	}while(!robot.isHeadingDone());
	

//	while (robot.getRotVel() != 0)
	{
	//ArUtil::sleep(5000);
	//cout<<"sleep2400\n"<<endl;
	}
	
	robot.move(dis-200);
	printf("Moving\n");
	do{
	}while(!robot.isMoveDone());
	//while(robot.getVel() != 0 )
	{
	ArUtil::sleep(1000);
	//cout<<"sleepdis*8\n"<<endl;
	}
	robot.lock();
	printf("arrived to:%.0lf, %.0lf\n***********************\n", robot.getX(), robot.getY());
	robot.unlock();

//	robot.clearDirectMotion();
	//ArUtil::sleep(2000);
	//system("Pause");
}

bool look(point p, ArRobot &robot){
	
	robot.lock();
	double th = robot.getTh(), x = robot.getX(), y = robot.getY();
	double px = p.x, py = p.y;
	robot.unlock();

	//new TaskShowCoord(&robot,new ArPose(fx, fy));
	//cout << "look:" << px << "," << py << endl;
	printf("look:%.0lf, %.0lf\n", px, py);
	printf("now:%.0lf, %.0lf\n", x, y);


	/*
	std::map<int, ArLaser*> *lasers = robot.getLaserMap();
	std::map<int, ArLaser*>::const_iterator l = lasers->begin();
	ArLaser* laser = (*l).second;   //得到激光对象

    std::list<ArPoseWithTime*> *readings = laser->getCurrentBuffer();
	std::list<ArPoseWithTime *>::iterator it;
	*/


	double dx = px-x;
	double dy = py-y;
	double dis = sqrt(dx*dx+dy*dy);
	double ang = atan2(dy, dx)*180/3.1415926; 
	/*
	if ((int)abs(th - ang) % 180 > 90) {
		robot.setDeltaHeading(180);
		Sleep(5000);
		th = robot.getTh();
	}
	ang = 90 - (th - ang);
	*/
	int count = 0;
	

	robot.setDeltaHeading(ang-th);
	printf("Turning\n");
	do{
	}while(!robot.isHeadingDone());
	//ArUtil::sleep(5000);


	robot.lock();
		std::map<int, ArLaser*> *lasers = robot.getLaserMap();
	std::map<int, ArLaser*>::const_iterator l = lasers->begin();
	ArLaser* laser = (*l).second;   //得到激光对象

    std::list<ArPoseWithTime*> *readings = laser->getCurrentBuffer();
	std::list<ArPoseWithTime *>::iterator it;

	bool flag = 1;
    for(it = readings->begin();it != readings->end();it++)
	{
		if(abs(count++ - 90) > 5) continue;
		double tmpx = (*it)->getX();
		double tmpy = (*it)->getY();
		if(sqrt((tmpx-x)*(tmpx-x) + (tmpy-y)*(tmpy-y)) < dis){
			flag = 0;
			break;
		}
	}
	robot.unlock();
	//system("Pause");
	printf("******************************\n");
	return flag;
}

bool reach(point p, ArRobot &robot){
	//cout << "reach\n";
    if (look(p, robot)){
		robot.lock();
        //robotgoto(p, robot);
		double x = robot.getX(), y = robot.getY();
		if(p.num > 1 && ((p.x - x)*(p.x - x) + (p.y - y)*(p.y - y) < 500)){
			robot.unlock();
			return 0;
		}
		printf("goningto:%.0lf,%.0lf\n", p.x, p.y);
        gotoDes.setGoal(ArPose(p.x, p.y));
		robot.unlock();
		//do{
		//}while(robot.isMoveDone());
		
		double dis = sqrt(pow(p.x-x,2)+pow(p.y-y, 2));
		ArUtil::sleep(dis*4+1500);
		//Sleep(8000);
		printf("arrived:%.0lf,%.0lf\n", robot.getX(), robot.getY());
		
		printf("******************************\n");
		gotoDes.cancelGoal();
		//system("Pause");
		return true;
    }else{
        //if (p.num==0) return false;
        //return reach(offset[p.num], robot);
		return 0;
    }
}


//***************************************
int main(int argc, char **argv)
{ 
  printf("Please type in the coordinates of the destination:\n");
  cin >> fx >> fy;
	Aria::init();
  ArArgumentParser argParser(&argc, argv);
  argParser.loadDefaultArguments();
  ArRobot robot;
  ArRobotConnector robotConnector(&argParser, &robot);
  ArLaserConnector laserConnector(&argParser, &robot, &robotConnector);

  // Always try to connect to the first laser:
  argParser.addDefaultArgument("-connectLaser");

  if(!robotConnector.connectRobot())
  {
    ArLog::log(ArLog::Terse, "Could not connect to the robot.");
    if(argParser.checkHelpAndWarnUnparsed())
    {
        // -help not given, just exit.
        Aria::logOptions();
        Aria::exit(1);
        return 1;
    }
  }


  // Trigger argument parsing
  if (!Aria::parseArgs() || !argParser.checkHelpAndWarnUnparsed())
  {
    Aria::logOptions();
    Aria::exit(1);
    return 1;
  }

  ArKeyHandler keyHandler;
  Aria::setKeyHandler(&keyHandler);
  robot.attachKeyHandler(&keyHandler);

  puts("This program will make the robot wander around. It uses some avoidance\n"
  "actions if obstacles are detected, otherwise it just has a\n"
  "constant forward velocity.\n\nPress CTRL-C or Escape to exit.");
  
  ArSonarDevice sonar;
  robot.addRangeDevice(&sonar);

  robot.runAsync(true);

  
  // try to connect to laser. if fail, warn but continue, using sonar only
  if(!laserConnector.connectLasers())
  {
    ArLog::log(ArLog::Normal, "Warning: unable to connect to requested lasers, will wander using robot sonar only.");
  }


  // turn on the motors, turn off amigobot sounds
  robot.enableMotors();
  robot.comInt(ArCommands::SOUNDTOG, 0);

  gotoDes.setSpeed(300);
  robot.addAction(&stopAction, 20);
  robot.addAction(&gotoDes, 25);
  robot.addAction(&recover, 100); 
  robot.addAction(&bumpers, 75);
  robot.addAction(&avoidFrontNear, 50);
  robot.addAction(&avoidFrontFar, 49);
  

  //robot.setDirectMotionPrecedenceTime(4500);
  cout<<"robotMotionTime"<<robot.getDirectMotionPrecedenceTime()<<endl;



    point start;
    start.x = 0;//填写起始坐标
    start.y = 0;
    onset.insert(start);
    num=0;
	//robot.setTransVelMax(800);
	robot.moveTo(ArPose(0,0));
	//cout << "start\n";

  while (Aria::getRunning()) 
  {
	point final;
	final.x=fx;
	final.y=fy;
    while (1){
		//cout << 1 << endl;
		//if (reach(final,robot)) break;
        bool f=false;
        point now;
       set<point,mycmp>::iterator it;
        while (1){
			//cout << 2 << endl;
            if (onset.empty()){f=true; break;}
            it=onset.begin();
            now=*it;
			//printf("seletnum%d\n",now.num);
            if (reach(now, robot)) break;
			else onset.erase(it);
			/*
			if (look(now, robot))
			{
				//robotgoto(p, robot);
				gotoDes.setGoal(ArPose(now.x, now.y));
			}
			*/
        }
        if (f){
			cout<<"empty!"<<endl;
            point start;
			robot.lock();
            start.x=robot.getX();//填写当前
            start.y=robot.getY();
			printf("now:%.0lf, %.0lf\n", start.x, start.y);
			robot.unlock();
            onset.insert(start);
            num=0;
            continue;
        }
        if (fabs(now.x-fx)<eps && fabs(now.y-fy)<eps) break;//到达终点
        onset.erase(it); 
		now.x=robot.getX();
		now.y=robot.getY();
        offset.insert(now);
		const int sampleNum = 19;
        point newlist[sampleNum];
        getnewpoints(now,newlist, robot);
		//for (int i=0;i<7;i++) printf("%.0lf %.0lf %0lf\n",newlist[i].x,newlist[i].y,newlist[i].F());	
        for (int i=0;i<sampleNum;i++){
            set<point,mycmp>::iterator iter=offset.begin();
			if (!offset.empty()){
			for (;iter!=offset.end();){
                if (fabs(iter->x-newlist[i].x)<eps && fabs(iter->y-newlist[i].y)<eps){
                    newlist[i].G+=iter->G+3000;
                }
                iter++;
            }
			}
			iter = onset.begin();

			bool flag=true;
            for (;iter!=onset.end();){
                if (fabs(iter->x-newlist[i].x)<eps && fabs(iter->y-newlist[i].y)<eps){
                    if (iter->G>newlist[i].G)
                        onset.erase(iter++);
                    else{flag=false;iter++;}
                }
                else iter++;
            }
            if (flag) {
                num++;
                newlist[i].num=num;
                onset.insert(newlist[i]);
				//printf("insert%d\n",i);
            }
        }
    }

	cout << "Finished!\n";
	robot.stop();
	break;
  }
  robot.waitForRunExit();
  
  Aria::exit(0);
  return 0;
}

