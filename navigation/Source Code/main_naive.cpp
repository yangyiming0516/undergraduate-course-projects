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
#define WIN32

#include "Aria.h"
#include <iostream>
#include "TaskShowCoord.h"
using namespace std;

ArActionGroup *teleop;	//manual
ArActionGroup *aimto;	//go to the destination

//start different modes
void teleopMode(void)
{
  teleop->activateExclusive();
  printf("You can use the arrow keys to drive, and the spacebar to stop.\nFor joystick control press the trigger button and then drive.\nPress 'w' for wander mode.\nPress 'a' for aimto mode.\nPress escape to exit.\n");
}

void aimtoMode(void)
{
  aimto->activateExclusive();
  printf("The robot will now aim to the destination.\nPress 't' for teleop mode.\nPress 'w' for wander mode.\nPress escape to exit.\n");
}

//***************************************
int main(int argc, char **argv)
{
  Aria::init();
  ArArgumentParser argParser(&argc, argv);
  argParser.loadDefaultArguments();

  // the serial connection (robot)
  //ArSerialConnection serConn;
  // tcp connection (sim)
  //ArTcpConnection tcpConn;

  ArRobot robot;
  ArRobotConnector robotConnector(&argParser, &robot);
  ArLaserConnector laserConnector(&argParser, &robot, &robotConnector);

  printf("Please type in the coordinates of the destination:\n");
  int x, y;
  cin >> x >> y;
  ArPose goal(x,y);
  // Always try to connect to the first laser:
  argParser.addDefaultArgument("-connectLaser");
   // the group of actions for teleop
  teleop = new ArActionGroup(&robot);
  // the group of actions for go to some destination
  aimto = new ArActionGroup(&robot);


  if(!robotConnector.connectRobot())
  {
	  cout << "1\n";
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
	  cout << "2\n";
    Aria::logOptions();
    Aria::exit(1);
    return 1;
  }

  //ArKeyHandler keyHandler;
  //Aria::setKeyHandler(&keyHandler);
  //robot.attachKeyHandler(&keyHandler);

  ArSonarDevice sonar;
  robot.addRangeDevice(&sonar);

  //robot.runAsync(true);

  
  // try to connect to laser. if fail, warn but continue, using sonar only
  if(!laserConnector.connectLasers())
  {
	  cout << "3\n";
    ArLog::log(ArLog::Normal, "Warning: unable to connect to requested lasers, will wander using robot sonar only.");
  }


  // turn on the motors, turn off amigobot sounds
  robot.enableMotors();
  robot.comInt(ArCommands::SOUNDTOG, 0);

  // add a set of actions that combine together to effect the wander behavior
  ArActionStallRecover recover("stall recover", 200, 50, 150, 0);
  ArActionBumpers bumpers;
  ArActionAvoidFront avoidFrontNear("Avoid Front Near", 220, 0);
  ArActionAvoidFront avoidFrontFar;
  ArActionConstantVelocity constantVelocity("Constant Velocity", 400);
  ArActionGoto gotoDes; //goto函数，自带，可让机器人从起始点向着目标点直走
  gotoDes.setGoal(goal); //设置目标点
  gotoDes.setCloseDist(100); //设置精度误差
  gotoDes.setSpeed(200);

  aimto->addAction(&recover, 100); 
  aimto->addAction(&bumpers, 75);
  aimto->addAction(&avoidFrontNear, 50);
  aimto->addAction(&avoidFrontFar, 49);
  aimto->addAction(&gotoDes, 25);

  // don't hit any tables (if we have sensors)
  teleop->addAction(new ArActionLimiterTableSensor, 100);
  // limiter for close obstacles
  teleop->addAction(new ArActionLimiterForwards("speed limiter near", 
						300, 600, 250), 95);
  // limiter for far away obstacles
  teleop->addAction(new ArActionLimiterForwards("speed limiter far", 
					       300, 1100, 400), 90);
  // limiter so we don't bump things backwards
  teleop->addAction(new ArActionLimiterBackwards, 85);
  // the joydrive action (drive from joystick)
  // we need one of these our selves, so we can mess with it later
  ArActionJoydrive joydriveAct("joydrive", 400, 15);
  teleop->addAction(&joydriveAct, 50);
  // the keydrive action (drive from keyboard)
  teleop->addAction(new ArActionKeydrive, 45);

  ArKeyHandler *keyHandler;
  // make sure there isn't already one (there should be in this example)
  if ((keyHandler = Aria::getKeyHandler()) == NULL)
  {
	  cout << "4\n";
    keyHandler = new ArKeyHandler;
    Aria::setKeyHandler(keyHandler);
    robot.attachKeyHandler(keyHandler);
  }
  // make the callbacks
  ArGlobalFunctor teleopCB(&teleopMode);
  //ArGlobalFunctor wanderCB(&wanderMode);
  ArGlobalFunctor aimtoCB(&aimtoMode);

  keyHandler->addKeyHandler('t', &teleopCB);
  keyHandler->addKeyHandler('T', &teleopCB);
  keyHandler->addKeyHandler('a', &aimtoCB);
  keyHandler->addKeyHandler('A', &aimtoCB);

  printf("This program will allow you to use a joystick or keyboard to control the robot or let the robot wander.\n");
  printf("Starting in Teleop mode, press the 'w' key to switch to wander.\n");
  aimtoMode();

  // if we don't have a joystick, let 'em know
  if (!joydriveAct.joystickInited())
  {
	  cout << "5\n";
    printf("Do not have a joystick, only the arrow keys on the keyboard will work.\n");
  }
  
  // set the joystick so it won't do anything if the button isn't pressed
  joydriveAct.setStopIfNoButtonPressed(false);

  new TaskShowCoord(&robot,&goal);
  robot.run(true);
  // wait for robot task loop to end before exiting the program
  robot.waitForRunExit();
  
  Aria::exit(0);
  return 0;
}

