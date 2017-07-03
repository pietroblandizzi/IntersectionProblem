#ifndef ENTITY_H
#define ENTITY_H

/**
Types of any possible
Entity that can cross the intersection
**/


enum class TypeOfEntity
{
    Pedestrian_,
    Vehicle_
};

class Entity
{
    public:
        Entity() {}
        virtual ~Entity() {}

        void virtual crossing () = 0;
        int  getStarPos () {return startPos;}
        int getEndPos () {return endPos;}
        TypeOfEntity getRealType () {return m_real_type;}

    protected:
       TypeOfEntity m_real_type;
       int startPos;
       int endPos;
};

#endif // ENTITY_H
