#include "Intersection_V1.h"
#include <chrono>
#include <iostream>


/**
Set All the possible paths and the index
of the mutex associated.
In order to avoid starvation and deadlock
the mutex have to be locked from the small
index
**/

void Paths::setPaths()
{
    // left to right // right left
    std::pair<int,int>  p(1,2); std::vector<int> v = {0,1};
    paths[p] = v;
    std::pair<int,int>  p1(1,3); std::vector<int> v1 = {0,1,2};
    paths[p1] = v1;
    std::pair<int,int>  p2(1,4); std::vector<int> v2 = {0};
    paths[p2] = v2;
    std::pair<int,int>  p3(2,1); std::vector<int> v3 = {2,3};
    paths[p3] = v3;
    std::pair<int,int>  p4(2,3); std::vector<int> v4 = {2};
    paths[p4] = v4;
    std::pair<int,int>  p5(2,4); std::vector<int> v5 = {0,2,3};
    paths[p5] = v5;

    std::pair<int,int>  p6(3,1); std::vector<int> v6 = {3};
    paths[p6] = v6;
    std::pair<int,int>  p7(3,2); std::vector<int> v7 = {0,1,3};
    paths[p7] = v7;
    std::pair<int,int>  p8(3,4); std::vector<int> v8 = {3,0};
    paths[p8] = v8;

    std::pair<int,int>  p9(4,1); std::vector<int> v9 = {1,2,3};
    paths[p9] = v9;
    std::pair<int,int>  p10(4,2); std::vector<int> v10 = {1};
    paths[p10] = v10;

    std::pair<int,int>  p11(4,3); std::vector<int> v11 = {1,2};
    paths[p11] = v11;
}


/** Return the path from start pos and end pos**/
void Paths::getPath (const std::pair<int,int>& p, std::vector<int>& v)
{
    v.resize(paths[p].size());
    v= paths[p];


}


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


Intersection_V1::Intersection_V1() : cross_m(4)
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
try to lock all the path
if one resource is locked
it release all the previous
**/
bool Intersection_V1::try_lock (std::vector<int>& index)
{
    size_t i = 0;
    bool flag = true;
    for (i = 0; i< index.size(); i++) {
        if (!cross_m[index[i]].try_lock()){
            flag = false;
            break;
        }
    }
    if (flag == false) {
        for (size_t j = 0; j < i ; j++) {
            cross_m[index[j]].unlock ();
        }
    }

    return flag;
}

/**
unlock all the locked resources
**/
void Intersection_V1::unlock (std::vector<int>& index)
{
    for (size_t j = 0; j <  index.size(); j++) {
        cross_m[index[j]].unlock ();
    }
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
        std::pair<int,int> p(e->getStarPos(),e->getEndPos());
        std::vector<int> index;
        paths.getPath(p,index);

        if (e->getRealType() == TypeOfEntity::Pedestrian_) {
            // we have to lock all the path or unlock all!!
            if (try_lock(index)) {
                 e->crossing();
                 unlock(index);
            }
        }
        //Vehicle
        else {
            if (!d.empty()) {
                if (!dl.isDeadlock()) {
                    dl.setLeft();
                    std::this_thread::sleep_for (std::chrono::seconds(1));
                 }
                 else {

                    // we have to lock all the path or unlock all!!
                    if (try_lock(index)) {
                        e->crossing();
                        unlock(index);
                        dl.reset();
                    }
                 }
            }
            else {
                    if (try_lock(index)) {
                        e->crossing();
                        unlock(index);
                    }
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
        std::pair<int,int> p(e->getStarPos(),e->getEndPos());
        std::vector<int> index;
        paths.getPath(p,index);

        if (e->getRealType() == TypeOfEntity::Pedestrian_) {
            if (try_lock(index)) {
                e->crossing();
                unlock(index);
            }
        }
        else {
            if (!u.empty()) {
                if (!dl.isDeadlock()) {
                      dl.setRight();
                      std::this_thread::sleep_for (std::chrono::seconds(1));
                 }
                 else {
                    if (try_lock(index)) {
                        e->crossing();
                        unlock(index);
                        dl.reset();
                    }

                 }
            }
            else {
                if (try_lock(index)) {
                    e->crossing();
                    unlock(index);
                }
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
        std::pair<int,int> p(e->getStarPos(),e->getEndPos());
        std::vector<int> index;
        paths.getPath(p,index);

        if (e->getRealType() == TypeOfEntity::Pedestrian_) {
            if (try_lock(index)) {
                e->crossing();
                unlock(index);
            }
        }
        else {
            if (!l.empty()) {
                 if (!dl.isDeadlock()) {
                      dl.setUp();
                      std::this_thread::sleep_for (std::chrono::seconds(1));
                 }
                 else {
                    if (try_lock(index)) {
                        e->crossing();
                        unlock(index);
                        dl.reset();
                    }
                 }
            }
            else {
                if (try_lock(index)) {
                    e->crossing();
                    unlock(index);
                }
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
        std::pair<int,int> p(e->getStarPos(),e->getEndPos());
        std::vector<int> index;
        paths.getPath(p,index);

        if (e->getRealType() == TypeOfEntity::Pedestrian_) {
            if (try_lock(index)) {
                e->crossing();
                unlock(index);
            }
        }
        else {
            if (!r.empty()) {
                 if (!dl.isDeadlock()) {
                      dl.setDown();
                      std::this_thread::sleep_for (std::chrono::seconds(1));
                 }
                 else {
                    if (try_lock(index)) {
                        e->crossing();
                        unlock(index);
                        dl.reset();
                    }
                 }
            }
            else {
                if (try_lock(index)) {
                    e->crossing();
                    unlock(index);
                }
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
                std::unique_ptr<Entity> p  (new Car(s,e)) ;
                l.push_back(std::move(p));
                cv_l.notify_one();
                 break;
            }

            case 2: {
                std::unique_ptr<Entity> p  (new Car(s,e)) ;
                r.push_back(std::move(p));
                cv_r.notify_one();
                 break;
            }

            case 3: {
                std::unique_ptr<Entity> p  (new Car(s,e)) ;
               u.push_back(std::move(p));
               cv_u.notify_one();
                break;
            }

            case 4: {
                std::unique_ptr<Entity> p  (new Car(s,e)) ;
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
                 std::unique_ptr<Entity> p  (new Motorcycle(s,e)) ;
                l.push_back(std::move(p));
                cv_l.notify_one();
                break;
            }

            case 2: {
                std::unique_ptr<Entity> p  (new Motorcycle(s,e)) ;
                r.push_back(std::move(p));
                cv_r.notify_one();
                break;
            }

            case 3: {
                std::unique_ptr<Entity> p  (new Motorcycle(s,e)) ;
               u.push_back(std::move(p));
               cv_u.notify_one();
               break;
            }

            case 4: {
                std::unique_ptr<Entity> p  (new Motorcycle(s,e)) ;
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
             std::unique_ptr<Entity> p  (new Truck(s,e)) ;
            l.push_back(std::move(p));
            cv_l.notify_one();
              break;
        }

        case 2: {
            std::unique_ptr<Entity> p  (new Truck(s,e)) ;
            r.push_back(std::move(p));
            cv_r.notify_one();
              break;
        }

        case 3: {
            std::unique_ptr<Entity> p  (new Truck(s,e)) ;
           u.push_back(std::move(p));
           cv_u.notify_one();
             break;
        }

        case 4: {
            std::unique_ptr<Entity> p  (new Truck(s,e)) ;
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
                 std::unique_ptr<Entity> p  (new Ambulance(s,e)) ;
                l.push_back(std::move(p));
                cv_l.notify_one();
                break;
            }

            case 2: {
                std::unique_ptr<Entity> p  (new Ambulance(s,e)) ;
                r.push_back(std::move(p));
                cv_r.notify_one();
                break;
            }

            case 3: {
                std::unique_ptr<Entity> p  (new Ambulance(s,e)) ;
               u.push_back(std::move(p));
               cv_u.notify_one();
               break;
            }

            case 4: {
                std::unique_ptr<Entity> p  (new Ambulance(s,e)) ;
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
                 std::unique_ptr<Entity> p  (new Bicycle(s,e)) ;
                l.push_back(std::move(p));
                cv_l.notify_one();
                break;
            }

            case 2: {
                std::unique_ptr<Entity> p  (new Bicycle(s,e)) ;
                r.push_back(std::move(p));
                cv_r.notify_one();
                break;
            }

            case 3: {
                std::unique_ptr<Entity> p  (new Bicycle(s,e)) ;
               u.push_back(std::move(p));
               cv_u.notify_one();
               break;
            }

            case 4: {
                std::unique_ptr<Entity> p  (new Bicycle(s,e)) ;
                d.push_back(std::move(p));
                cv_d.notify_one();
                break;
            }

        }
        break;
    }

}
}
