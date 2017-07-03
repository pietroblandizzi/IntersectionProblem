#ifndef PEDESTRIAN_H
#define PEDESTRIAN_H

#include "Entity.h"


/**
Pedestrian interface
**/

class Pedestrian : public Entity
{
    public:
        Pedestrian (int s, int e)
    {
        startPos = s;
        endPos = e;
        m_real_type = TypeOfEntity::Pedestrian_;
    }

         ~Pedestrian() {}

        void crossing()  {
        std::cout << " Pedestrian is  crossing" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(10));
    }


};

class OldPedestrian : public Pedestrian
{
    public:
        OldPedestrian (int s, int e) : Pedestrian(s,e)
    {

        m_real_type = TypeOfEntity::Pedestrian_;
    }

         ~OldPedestrian() {}

        void crossing()  {
        std::cout << "Really old Pedestrian is  crossing" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(20));
    }


};


#endif // PEDESTRIAN_H
