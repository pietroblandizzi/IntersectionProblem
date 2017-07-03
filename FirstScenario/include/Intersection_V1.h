#ifndef INTERSECTION_V1_H
#define INTERSECTION_V1_H

#include <deque>
#include <thread>
#include <mutex>
#include <condition_variable>

#include "TSQueue.h"
#include "Entity.h"
#include "Vehicle.h"
#include "Pedestrian.h"

/**
Deadlock handler class
**/
class Deadlock
{
    public:
        Deadlock () {l = false; r = false; u = false; d = false;}

        void setLeft ();
        void setRight ();
        void setUp ();
        void setDown ();

        bool isDeadlock ();
        void reset () {l = false; r = false; u = false; d = false;}

    private:
        bool l,r,u,d;
        std::mutex m;
};

/**
Intersection handler class
**/
class Intersection_V1
{
    public:
        Intersection_V1();
        virtual ~Intersection_V1();

        void handler_l ();
        void handler_r ();
        void handler_u ();
        void handler_d ();

        void approachingIntersection (int s ,int e);
        void approachingIntersection (VehicleType vt, int s ,int e);
    private:

        TSQueue<std::unique_ptr<Entity>> l,r,u,d;

        std::thread tl,tr,tu,td;
        //producer consumer
        std::mutex m_l,m_r,m_u, m_d;
        std::condition_variable cv_l,cv_r,cv_u,cv_d;

        //crossing lock
        std::mutex cross_m;

        Deadlock dl;
        int c = 0;
};

#endif // INTERSECTION_V1_H
