#include "Intersection_V1.h"
#include <chrono>
#include <iostream>

/**
Class for deadlock handling
If everyone has the right end blocked
with a good sense role the
first that is able to go just cross the
intersection
**/


bool Deadlock::isDeadlock(){
std::unique_lock<std::mutex> lck(m);
return l && r && u && d ? true : false;
}

void Deadlock::setLeft()
{
    std::unique_lock<std::mutex> lck(m);
    l = true;
}

void Deadlock::setRight()
{
    std::unique_lock<std::mutex> lck(m);
    r = true;
}

void Deadlock::setUp()
{
    std::unique_lock<std::mutex> lck(m);
    u = true;
}

void Deadlock::setDown()
{
    std::unique_lock<std::mutex> lck(m);
    d = true;
}

Intersection_V1::Intersection_V1()
{
    tl = std::thread (handler_l,this);
    tr = std::thread (handler_r,this);
    tu = std::thread (handler_u,this);
    td = std::thread (handler_d,this);

}

Intersection_V1::~Intersection_V1()
{
    tl.join();
    tr.join();
    tu.join();
    td.join();
}

/**
threads that handle the directions
if nothing is in the queue the threads sleep
on condition variables
else they try to perform a cross
**/

void Intersection_V1::handler_l()
{
    std::cout << " Left Handler start" << std::endl;
   while (1){

        std::unique_lock<std::mutex> lck(m_l);
        while (l.empty())
            cv_l.wait(lck);

        lck.unlock();

        std::unique_ptr<Entity> e = l.pop();

        if (e->getRealType() == TypeOfEntity::Pedestrian_) {
            std::unique_lock<std::mutex> lck_cross(cross_m);
            e->crossing();
            lck_cross.unlock();
        }
        else {
            if (!d.empty()) {
                 if (!dl.isDeadlock()) {
                      dl.setLeft();
                      std::this_thread::sleep_for (std::chrono::seconds(1));
                 }
                 else {
                    std::unique_lock<std::mutex> lck_cross(cross_m);
                    e->crossing();
                    lck_cross.unlock();
                    dl.reset();
                 }
            }
            else {
                std::unique_lock<std::mutex> lck_cross(cross_m);
                e->crossing();
                lck_cross.unlock();
            }
        }
    }
}

void Intersection_V1::handler_r()
{
    std::cout << " Right Handler start" << std::endl;

   while (1){

        std::unique_lock<std::mutex> lck(m_r);
        while (r.empty())
            cv_r.wait(lck);

        lck.unlock();


        std::unique_ptr<Entity> e = r.pop();

        if (e->getRealType() == TypeOfEntity::Pedestrian_) {
            std::unique_lock<std::mutex> lck_cross(cross_m);
            e->crossing();
            lck_cross.unlock();
        }
        else {
            if (!u.empty()) {
                if (!dl.isDeadlock()) {
                      dl.setRight();
                      std::this_thread::sleep_for (std::chrono::seconds(1));
                 }
                 else {
                    std::unique_lock<std::mutex> lck_cross(cross_m);
                    e->crossing();
                    lck_cross.unlock();
                    dl.reset();
                 }
            }
            else {
                std::unique_lock<std::mutex> lck_cross(cross_m);
                e->crossing();
                lck_cross.unlock();
            }
        }

}
}

void Intersection_V1::handler_u()
{
    std::cout << " Up Handler start" << std::endl;
    while (1){

        std::unique_lock<std::mutex> lck(m_u);
        while (u.empty())
            cv_u.wait(lck);

        lck.unlock();

        std::unique_ptr<Entity> e = u.pop();

        if (e->getRealType() == TypeOfEntity::Pedestrian_) {
            std::unique_lock<std::mutex> lck_cross(cross_m);
            e->crossing();
            lck_cross.unlock();
        }
        else {
            if (!l.empty()) {
                 if (!dl.isDeadlock()) {
                      dl.setUp();
                      std::this_thread::sleep_for (std::chrono::seconds(1));
                 }
                 else {
                    std::unique_lock<std::mutex> lck_cross(cross_m);
                    e->crossing();
                    lck_cross.unlock();
                    dl.reset();
                 }
            }
            else {
                std::unique_lock<std::mutex> lck_cross(cross_m);
                e->crossing();
                lck_cross.unlock();
            }
        }
}
}

void Intersection_V1::handler_d()
{
    std::cout << " Down Handler start" << std::endl;
   while (1){
        std::unique_lock<std::mutex> lck(m_d);
        while (d.empty())
            cv_d.wait(lck);

        lck.unlock();


        std::unique_ptr<Entity> e = d.pop();

        if (e->getRealType() == TypeOfEntity::Pedestrian_) {
            std::unique_lock<std::mutex> lck_cross(cross_m);
            e->crossing();
            lck_cross.unlock();
        }
        else {
            if (!r.empty()) {
                 if (!dl.isDeadlock()) {
                      dl.setDown();
                      std::this_thread::sleep_for (std::chrono::seconds(1));
                 }
                 else {
                    std::unique_lock<std::mutex> lck_cross(cross_m);
                    e->crossing();
                    lck_cross.unlock();
                    dl.reset();
                 }
            }
            else {
                std::unique_lock<std::mutex> lck_cross(cross_m);
                e->crossing();
                lck_cross.unlock();
            }
        }
}

}

/**
Push into queues of correct objects based
on a random generation
**/
void Intersection_V1::approachingIntersection (int s ,int e)
{
   std::cout << "Pedestrian arrive in  " << s << std::endl;

    switch (s) {

        case 1: {
             // left
             c++;
             std::unique_ptr<Entity> p;
             if (c >=5) {
               c = 0;
                p.reset(new OldPedestrian(s,e));
             }else {
                 p.reset(new Pedestrian(s,e));
             }
            l.push_front(std::move(p));
            cv_l.notify_one();
            break;
        }

        case 2: {
            c++;
             std::unique_ptr<Entity> p;
             if (c >=5) {
               c = 0;
                p.reset(new OldPedestrian(s,e));
             }else {
                 p.reset(new Pedestrian(s,e));
             }
            r.push_front(std::move(p));
            cv_r.notify_one();
             break;
        }

        case 3: {
            c++;
             std::unique_ptr<Entity> p;
             if (c >=5) {
               c = 0;
                p.reset(new OldPedestrian(s,e));
             }else {
                 p.reset(new Pedestrian(s,e));
             }
           u.push_front(std::move(p));
           cv_u.notify_one();
            break;
        }

        case 4: {
            c++;
             std::unique_ptr<Entity> p;
             if (c >=5) {
               c = 0;
                p.reset(new OldPedestrian(s,e));
             }else {
                 p.reset(new Pedestrian(s,e));
             }
            d.push_front(std::move(p));
            cv_d.notify_one();
             break;
        }

    }
}


void Intersection_V1::approachingIntersection(VehicleType vt, int s ,int e)
{
   std::cout << "Vehicle arrive in  " << s << std::endl;

switch (vt) {
    case VehicleType::Car : {
        switch (s) {

            case 1: {
                 // left
                 std::unique_ptr<Entity> p (new Car(s,e));
                l.push_back(std::move(p));
                cv_l.notify_one();
                 break;
            }

            case 2: {
                std::unique_ptr<Entity> p (new Car(s,e));
                r.push_back(std::move(p));
                cv_r.notify_one();
                 break;
            }

            case 3: {
                std::unique_ptr<Entity> p (new Car(s,e));
               u.push_back(std::move(p));
               cv_u.notify_one();
                break;
            }

            case 4: {
                std::unique_ptr<Entity> p (new Car(s,e));
                d.push_back(std::move(p));
                cv_d.notify_one();
                 break;
            }

        }

        break;
    }

    case VehicleType::Motorcycle : {
        switch (s) {

            case 1: {
                 // left
                 std::unique_ptr<Entity> p (new Motorcycle(s,e));
                l.push_back(std::move(p));
                cv_l.notify_one();
                break;
            }

            case 2: {
                std::unique_ptr<Entity> p (new Motorcycle(s,e));
                r.push_back(std::move(p));
                cv_r.notify_one();
                break;
            }

            case 3: {
                std::unique_ptr<Entity> p (new Motorcycle(s,e));
               u.push_back(std::move(p));
               cv_u.notify_one();
               break;
            }

            case 4: {
                std::unique_ptr<Entity> p (new Motorcycle(s,e));
                d.push_back(std::move(p));
                cv_d.notify_one();
                break;
            }

        }
        break;
    }

    case VehicleType::Truck : {

    switch (s) {

        case 1: {
             // left
             std::unique_ptr<Entity> p (new Truck(s,e));
            l.push_back(std::move(p));
            cv_l.notify_one();
              break;
        }

        case 2: {
            std::unique_ptr<Entity> p (new Truck(s,e));
            r.push_back(std::move(p));
            cv_r.notify_one();
              break;
        }

        case 3: {
            std::unique_ptr<Entity> p (new Truck(s,e));
           u.push_back(std::move(p));
           cv_u.notify_one();
             break;
        }

        case 4: {
            std::unique_ptr<Entity> p (new Truck(s,e));
            d.push_back(std::move(p));
            cv_d.notify_one();
              break;
        }

    }

    break;
    }


    case VehicleType::Emergency  : {

        switch (s) {

            case 1: {
                 // left
                 std::unique_ptr<Entity> p (new Ambulance(s,e));
                l.push_back(std::move(p));
                cv_l.notify_one();
                break;
            }

            case 2: {
                std::unique_ptr<Entity> p (new Ambulance(s,e));
                r.push_back(std::move(p));
                cv_r.notify_one();
                break;
            }

            case 3: {
                std::unique_ptr<Entity> p (new Ambulance(s,e));
               u.push_back(std::move(p));
               cv_u.notify_one();
               break;
            }

            case 4: {
                std::unique_ptr<Entity> p (new Ambulance(s,e));
                d.push_back(std::move(p));
                cv_d.notify_one();
                break;
            }

        }
        break;
    }

        case VehicleType::Bicycle  : {

        switch (s) {

            case 1: {
                 // left
                 std::unique_ptr<Entity> p (new Bicycle(s,e));
                l.push_back(std::move(p));
                cv_l.notify_one();
                break;
            }

            case 2: {
                std::unique_ptr<Entity> p (new Bicycle(s,e));
                r.push_back(std::move(p));
                cv_r.notify_one();
                break;
            }

            case 3: {
                std::unique_ptr<Entity> p (new Bicycle(s,e));
               u.push_back(std::move(p));
               cv_u.notify_one();
               break;
            }

            case 4: {
                std::unique_ptr<Entity> p (new Bicycle(s,e));
                d.push_back(std::move(p));
                cv_d.notify_one();
                break;
            }

        }
        break;
    }

}
}
