#ifndef VEHICLE_H
#define VEHICLE_H

#include <iostream>
#include <string>
#include <memory>
#include "Entity.h"

using std::string;


/**
Vehicle interface
and different implementations
**/

enum class VehicleType
{
    Bicycle,
    Motorcycle,
    Car,
    Truck,
    Emergency
};


class Vehicle : public Entity
{
    public:
        Vehicle() { }
        virtual ~Vehicle() {}

        virtual int getNumberOfWheels () = 0;

    protected:
        VehicleType vt;
        //other attributes
};

class Bicycle : public Vehicle
{

public:
    Bicycle (int s, int e)
    {
       startPos = s;
        endPos = e;
        vt = VehicleType::Bicycle;
        m_real_type = TypeOfEntity::Vehicle_;
        n_of_wheels = 2;
    }

        int  getNumberOfWheels () {return n_of_wheels;}

    void crossing()  {
        std::cout << " Bicycle crossing" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(6));
    }

private:
    int n_of_wheels;
};


class Motorcycle : public Vehicle
{

public:
    Motorcycle (int s, int e)
    {
       startPos = s;
        endPos = e;
        vt = VehicleType::Motorcycle;
        m_real_type = TypeOfEntity::Vehicle_;
        n_of_wheels = 2;
    }

        int  getNumberOfWheels () {return n_of_wheels;}

    void crossing()  {
        std::cout << " Motorcycle crossing" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }

private:
    int n_of_wheels;
};



class Car : public Vehicle
{

public:
    Car (int s, int e) {
       startPos = s;
        endPos = e;
        vt = VehicleType::Motorcycle;
        m_real_type = TypeOfEntity::Vehicle_;
        n_of_wheels = 4;}

        int  getNumberOfWheels () {return n_of_wheels;}

        void crossing()  {
        std::cout << " Car crossing" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }

private:
    int n_of_wheels;
};



class Truck : public Vehicle
{

public:
    Truck (int s, int e)  {
       startPos = s;
        endPos = e;
        vt = VehicleType::Motorcycle;
        m_real_type = TypeOfEntity::Vehicle_;
         n_of_wheels = 4;}

        int  getNumberOfWheels () {return n_of_wheels;}

        void crossing()  {
        std::cout << " Truck crossing" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(6));
    }
private:
    int n_of_wheels;
};


class Ambulance : public Vehicle
{

public:
    Ambulance (int s, int e)  {
       startPos = s;
        endPos = e;
        vt = VehicleType::Motorcycle;
        m_real_type = TypeOfEntity::Vehicle_;
        n_of_wheels = 4;}

        int  getNumberOfWheels () {return n_of_wheels;}

        void crossing()  {
        std::cout << " Ambulance crossing" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }

private:
    int n_of_wheels;
};




#endif // VEHICLE_H
