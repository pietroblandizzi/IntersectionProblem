#include <iostream>
#include "Intersection_V1.h"
#include <Math.h>
#include <random>
#include <chrono>
#include "Vehicle.h"
#include <thread>
#include "Pedestrian.h"

using namespace std;

/***
0 Pedestrian
1 Car
2 Motorcycle
3 Truck
4 Ambulance
5 Bicycle


1 Left
2 Right
3 Up
4 Down
*/


/**
Random traffic generator
generate random the type and the direction
and it sleep for a random time between 5 and 10 sec
**/

void random_generator_of_traffic (Intersection_V1& inter)
{
   while (1) {

    int minV = 0;
    int MaxV = 5;

    int minD = 1;
    int MaxD = 4;

    int randNum = rand()%(MaxV-minV + 1) + minV;

    switch (randNum) {

        case 0 : {
            //generate  a direction
          int randStart = rand()%(MaxD-minD + 1) + minD;
          int randEnd = rand()%(MaxD-minD + 1) + minD;
          while (randStart == randEnd) {
            randEnd = rand()%(MaxD-minD + 1) + minD;
          }
            inter.approachingIntersection(randStart,randEnd);
            break;
        }

        case 1 : {
           int randStart = rand()%(MaxD-minD + 1) + minD;
          int randEnd = rand()%(MaxD-minD + 1) + minD;
          while (randStart == randEnd) {
          randEnd = rand()%(MaxD-minD + 1) + minD;
          }
            inter.approachingIntersection(VehicleType::Car,randStart,randEnd);
            break;
        }

        case 2 : {
            int randStart = rand()%(MaxD-minD + 1) + minD;
            int randEnd = rand()%(MaxD-minD + 1) + minD;
            while (randStart == randEnd) {
                randEnd = rand()%(MaxD-minD + 1) + minD;
            }

           inter.approachingIntersection(VehicleType::Motorcycle,randStart,randEnd);
           break;
        }

        case 3 : {
            int randStart = rand()%(MaxD-minD + 1) + minD;
            int randEnd = rand()%(MaxD-minD + 1) + minD;
            while (randStart == randEnd) {
                randEnd = rand()%(MaxD-minD + 1) + minD;
            }

            inter.approachingIntersection(VehicleType::Truck,randStart,randEnd);
            break;

        }

        case 4 : {
            int randStart = rand()%(MaxD-minD + 1) + minD;
            int randEnd = rand()%(MaxD-minD + 1) + minD;
            while (randStart == randEnd) {
                randEnd = rand()%(MaxD-minD + 1) + minD;
            }
           inter.approachingIntersection(VehicleType::Emergency,randStart,randEnd);
           break;

        }
         case 5 : {
            int randStart = rand()%(MaxD-minD + 1) + minD;
            int randEnd = rand()%(MaxD-minD + 1) + minD;
            while (randStart == randEnd) {
                randEnd = rand()%(MaxD-minD + 1) + minD;
            }
           inter.approachingIntersection(VehicleType::Bicycle,randStart,randEnd);
           break;
        }

    }

    //random rime for sleeping betwwen 5 sec and 10
    int randSleep = rand()%(10-5 + 1) + 5;
    std::this_thread::sleep_for(std::chrono::seconds(randSleep));

   }

}


int main()
{
    Intersection_V1 inter;

    std::thread  t(random_generator_of_traffic,std::ref(inter));
    t.join ();
    return 0;
}
