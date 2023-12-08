#include "Aria.h"
#include "Header.h"
#include <iostream>


using namespace std;
#pragma warning(disable: 4996) 

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
    hay_camino = encuentra_mejor_camino("AA", "CC", grafo, cantidad_real_de_enlaces_del_grafo, camino_3);
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

  float angulo_anterior = 0;
  float angulo_a_rotar = 0;
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
      angulo_a_rotar = angle - angulo_anterior;

	  ArTime start;
      printf("directMotionExample: Telling the robot to turn to %.2f, then sleeping 4 seconds\n",angle);
      robot.lock();
      robot.setHeading(angle);
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

      printf("Stopping\n");
      robot.lock();
      robot.setRotVel(0);
      robot.unlock();
      ArUtil::sleep(200);
      printf("directMotionExample: Telling the robot to move forwards %.2f milimeters, then sleeping 5 seconds\n",distance);
      robot.lock();
      robot.move(distance);
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
  }

  // Fin encontrar camino

  printf("directMotionExample: Done, exiting.\n");
  Aria::exit(0);
  return 0;
}
