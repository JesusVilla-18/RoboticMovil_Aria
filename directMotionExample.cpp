/*
MobileRobots Advanced Robotics Interface for Applications (ARIA)
Copyright (C) 2004, 2005 ActivMedia Robotics LLC
Copyright (C) 2006, 2007, 2008, 2009, 2010 MobileRobots Inc.
Copyright (C) 2011, 2012 Adept Technology

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
MobileRobots for information about a commercial version of ARIA at 
robots@mobilerobots.com or 
MobileRobots Inc, 10 Columbia Drive, Amherst, NH 03031; 800-639-9481
*/
#include "Aria.h"
#include "Header.h"
#include <iostream>


using namespace std;

#pragma warning(disable: 4996) 
/** @example directMotionExample.cpp Sends a sequence of motion commands directly
 *     to the robot using ArRobot's direct motion command methods.
 
  This demo starts up the robot in its own thread, then calls a series of the 
  motion commands methods in ArRobot to simply send commands directly
  to the robot (rather than using e.g. ArAction objects.)
  
  Note that if you want to stop direct motion 
  commands and let ArActions take over, you must call ArRobot::clearDirectMotion() (otherwise ArRobot may continue sending motion commands that conflict
  with the requests of the action resolver). 
  See the section on motion commands and actions for details.  
*/

/*
  This is a connection handler class, to demonstrate how to run code in
  response to events such as the program connecting an disconnecting
  from the robot.
*/
class ConnHandler
{
public:
  // Constructor
  ConnHandler(ArRobot *robot);
  // Destructor, its just empty
  ~ConnHandler(void) {}
  // to be called if the connection was made
  void connected(void);
  // to call if the connection failed
  void connFail(void);
  // to be called if the connection was lost
  void disconnected(void);
protected:
  // robot pointer
  ArRobot *myRobot;
  // the functor callbacks
  ArFunctorC<ConnHandler> myConnectedCB;
  ArFunctorC<ConnHandler> myConnFailCB;
  ArFunctorC<ConnHandler> myDisconnectedCB;
};

ConnHandler::ConnHandler(ArRobot *robot) :
  myConnectedCB(this, &ConnHandler::connected),  
  myConnFailCB(this, &ConnHandler::connFail),
  myDisconnectedCB(this, &ConnHandler::disconnected)

{
  myRobot = robot;
  myRobot->addConnectCB(&myConnectedCB, ArListPos::FIRST);
  myRobot->addFailedConnectCB(&myConnFailCB, ArListPos::FIRST);
  myRobot->addDisconnectNormallyCB(&myDisconnectedCB, ArListPos::FIRST);
  myRobot->addDisconnectOnErrorCB(&myDisconnectedCB, ArListPos::FIRST);
}


// just exit if the connection failed
void ConnHandler::connFail(void)
{
  printf("directMotionDemo connection handler: Failed to connect.\n");
  myRobot->stopRunning();
  Aria::exit(1);
  return;
}

// turn on motors, and off sonar, and off amigobot sounds, when connected
void ConnHandler::connected(void)
{
  printf("directMotionDemo connection handler: Connected\n");
  myRobot->comInt(ArCommands::SONAR, 0);
  myRobot->comInt(ArCommands::ENABLE, 1);
  myRobot->comInt(ArCommands::SOUNDTOG, 0);
}

// lost connection, so just exit
void ConnHandler::disconnected(void)
{
  printf("directMotionDemo connection handler: Lost connection, exiting program.\n");
  Aria::exit(0);
}



int main(int argc, char **argv) 
{

    bool pudo_leer = false;
    pudo_leer = lee_datos("grafo.txt", grafo, cantidad_real_de_enlaces_del_grafo);
    bool hay_camino;
    string camino_1;
    string camino_2;
    string camino_3;
    hay_camino = encuentra_mejor_camino("AA", "CB", grafo, cantidad_real_de_enlaces_del_grafo, camino_3);
    string space_delimiter = " ";
    vector<string> nodos_del_camino{};
    size_t pos = 0;
    float distance = 0;
    float angle = 0;

  Aria::init();
  
  ArArgumentParser argParser(&argc, argv);
  argParser.loadDefaultArguments();


  ArRobot robot;
  ArRobotConnector con(&argParser, &robot);

  // the connection handler from above
  ConnHandler ch(&robot);

  if(!Aria::parseArgs())
  {
    Aria::logOptions();
    Aria::exit(1);
    return 1;
  }

  if(!con.connectRobot())
  {
    ArLog::log(ArLog::Normal, "directMotionExample: Could not connect to the robot. Exiting.");

    if(argParser.checkHelpAndWarnUnparsed()) 
    {
      Aria::logOptions();
	}
    Aria::exit(1);
    return 1;
  }

  ArLog::log(ArLog::Normal, "directMotionExample: Connected.");

  if(!Aria::parseArgs() || !argParser.checkHelpAndWarnUnparsed())
  {
    Aria::logOptions();
    Aria::exit(1);
  }

  // Run the robot processing cycle in its own thread. Note that after starting this
  // thread, we must lock and unlock the ArRobot object before and after
  // accessing it.
  robot.runAsync(false);


  // Encontrar camino

  
  while ((pos = camino_3.find(space_delimiter)) != string::npos)
  {
	  nodos_del_camino.push_back(camino_3.substr(0, pos));
	  camino_3.erase(0, pos + space_delimiter.length());
  }
  for (unsigned int i = 0; i < nodos_del_camino.size() - 1; i++)
  {
	  string nodo1 = "";
	  float pos_x_nodo_1 = 0.0;
	  float pos_y_nodo_1 = 0.0;
	  string nodo2 = "";
	  float pos_x_nodo_2 = 0.0;
	  float pos_y_nodo_2 = 0.0;
	  nodo1 = nodos_del_camino.at(i);
	  devuelve_datos_de_un_nodo_grafo_no_dirigido(nodo1, grafo, cantidad_real_de_enlaces_del_grafo,
		  pos_x_nodo_1, pos_y_nodo_1);
	  nodo2 = nodos_del_camino.at(i + 1);
	  devuelve_datos_de_un_nodo_grafo_no_dirigido(nodo2, grafo, cantidad_real_de_enlaces_del_grafo,
		  pos_x_nodo_2, pos_y_nodo_2);
	  float distancia = 0.0;
	  float angulo = 0.0;
	  distance = calcula_distancia(pos_x_nodo_1, pos_y_nodo_1, pos_x_nodo_2, pos_y_nodo_2, distancia, angulo);
      angle = calcula_angulo(pos_x_nodo_1, pos_y_nodo_1, pos_x_nodo_2, pos_y_nodo_2, distancia, angulo)*180/M_PI;

      printf("directMotionExample: Setting rot velocity to 45 deg/sec\n");
      robot.lock();
      robot.setRotVel(45);
      robot.unlock();
      ArUtil::sleep(angle/45 * 1000);
      printf("Stopping\n");
      robot.lock();
      robot.setRotVel(0);
      robot.unlock();
      ArUtil::sleep(200);
      printf("directMotionExample: Telling the robot to move forwards , then sleeping 5 seconds\n");
      robot.lock();
      robot.move(distance);
      robot.unlock();
      ArTime start;
      start.setToNow();
      while (1)
      {
          robot.lock();
          if (robot.isMoveDone())
          {
              printf("directMotionExample: Finished distance\n");
              robot.unlock();
              break;
          }
          if (start.mSecSince() > 5000)
          {
              printf("directMotionExample: Distance timed out\n");
              robot.unlock();
              break;
          }
          robot.unlock();
          ArUtil::sleep(50);
      }
  }

  // Fin encontrar camino

  /*
  // Send the robot a series of motion commands directly, sleeping for a 
  // few seconds afterwards to give the robot time to execute them.
  printf("directMotionExample: Setting rot velocity to 100 deg/sec then sleeping 3 seconds\n");
  robot.lock();
  robot.setRotVel(100);
  robot.unlock();
  ArUtil::sleep(3*1000);
  printf("Stopping\n");
  robot.lock();
  robot.setRotVel(0);
  robot.unlock();
  ArUtil::sleep(200);

  printf("directMotionExample: Telling the robot to go 300 mm on left wheel and 100 mm on right wheel for 5 seconds\n");
  robot.lock();
  robot.setVel2(300, 100);
  robot.unlock();
  ArTime start;
  start.setToNow();
  while (1)
  {
    robot.lock();
    if (start.mSecSince() > 5000)
    {
      robot.unlock();
      break;
    }   
    robot.unlock();
    ArUtil::sleep(50);
  }
  
  printf("directMotionExample: Telling the robot to move forwards one meter, then sleeping 5 seconds\n");
  robot.lock();
  robot.move(1000);
  robot.unlock();
  start.setToNow();
  while (1)
  {
    robot.lock();
    if (robot.isMoveDone())
    {
      printf("directMotionExample: Finished distance\n");
      robot.unlock();
      break;
    }
    if (start.mSecSince() > 5000)
    {
      printf("directMotionExample: Distance timed out\n");
      robot.unlock();
      break;
    }   
    robot.unlock();
    ArUtil::sleep(50);
  }
  printf("directMotionExample: Telling the robot to move backwards one meter, then sleeping 5 seconds\n");
  robot.lock();
  robot.move(-1000);
  robot.unlock();
  start.setToNow();
  while (1)
  {
    robot.lock();
    if (robot.isMoveDone())
    {
      printf("directMotionExample: Finished distance\n");
      robot.unlock();
      break;
    }
    if (start.mSecSince() > 10000)
    {
      printf("directMotionExample: Distance timed out\n");
      robot.unlock();
      break;
    }
    robot.unlock();
    ArUtil::sleep(50);
  }
  printf("directMotionExample: Telling the robot to turn to 180, then sleeping 4 seconds\n");
  robot.lock();
  robot.setHeading(180);
  robot.unlock();
  start.setToNow();
  while (1)
  {
    robot.lock();
    if (robot.isHeadingDone(5))
    {
      printf("directMotionExample: Finished turn\n");
      robot.unlock();
      break;
    }
    if (start.mSecSince() > 5000)
    {
      printf("directMotionExample: Turn timed out\n");
      robot.unlock();
      break;
    }
    robot.unlock();
    ArUtil::sleep(100);
  }
  printf("directMotionExample: Telling the robot to turn to 90, then sleeping 2 seconds\n");
  robot.lock();
  robot.setHeading(90);
  robot.unlock();
  start.setToNow();
  while (1)
  {
    robot.lock();
    if (robot.isHeadingDone(5))
    {
      printf("directMotionExample: Finished turn\n");
      robot.unlock();
      break;
    }
    if (start.mSecSince() > 5000)
    {
      printf("directMotionExample: turn timed out\n");
      robot.unlock();
      break;
    }
    robot.unlock();
    ArUtil::sleep(100);
  }
  printf("directMotionExample: Setting vel2 to 200 mm/sec on both wheels, then sleeping 3 seconds\n");
  robot.lock();
  robot.setVel2(200, 200);
  robot.unlock();
  ArUtil::sleep(3000);
  printf("directMotionExample: Stopping the robot, then sleeping for 2 seconds\n");
  robot.lock();
  robot.stop();
  robot.unlock();
  ArUtil::sleep(2000);
  printf("directMotionExample: Setting velocity to 200 mm/sec then sleeping 3 seconds\n");
  robot.lock();
  robot.setVel(200);
  robot.unlock();
  ArUtil::sleep(3000);
  printf("directMotionExample: Stopping the robot, then sleeping for 2 seconds\n");
  robot.lock();
  robot.stop();
  robot.unlock();
  ArUtil::sleep(2000);
  printf("directMotionExample: Setting vel2 with 0 on left wheel, 200 mm/sec on right, then sleeping 5 seconds\n");
  robot.lock();
  robot.setVel2(0, 200);
  robot.unlock();
  ArUtil::sleep(5000);
  printf("directMotionExample: Telling the robot to rotate at 50 deg/sec then sleeping 5 seconds\n");
  robot.lock();
  robot.setRotVel(50);
  robot.unlock();
  ArUtil::sleep(5000);
  printf("directMotionExample: Telling the robot to rotate at -50 deg/sec then sleeping 5 seconds\n");
  robot.lock();
  robot.setRotVel(-50);
  robot.unlock();
  ArUtil::sleep(5000);
  printf("directMotionExample: Setting vel2 with 0 on both wheels, then sleeping 3 seconds\n");
  robot.lock();
  robot.setVel2(0, 0);
  robot.unlock();
  ArUtil::sleep(3000);
  printf("directMotionExample: Now having the robot change heading by -125 degrees, then sleeping for 6 seconds\n");
  robot.lock();
  robot.setDeltaHeading(-125);
  robot.unlock();
  ArUtil::sleep(6000);
  printf("directMotionExample: Now having the robot change heading by 45 degrees, then sleeping for 6 seconds\n");
  robot.lock();
  robot.setDeltaHeading(45);
  robot.unlock();
  ArUtil::sleep(6000);
  printf("directMotionExample: Setting vel2 with 200 on left wheel, 0 on right wheel, then sleeping 5 seconds\n");
  robot.lock();
  robot.setVel2(200, 0);
  robot.unlock();
  ArUtil::sleep(5000);
  */
  printf("directMotionExample: Done, exiting.\n");
  Aria::exit(0);
  return 0;
}
