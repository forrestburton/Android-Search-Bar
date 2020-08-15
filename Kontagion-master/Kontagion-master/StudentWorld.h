#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include <string>
#include <list>
class Actor;
class Socrates;

const double PI = 3.14159265;        

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
	virtual ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    bool checkSocratesOverlap(const double& x, const double& y, const int& dist = SPRITE_WIDTH);   
    void restoreSocrates();
    void hurtSocrates(const int& amount);
    void addFlamethrowers();   
    void getLocationOnCircle(const int& angle, double& x, double& y);
    bool checkWeaponOverlap(const double& x, const double& y, const int& amountDamage);
    bool checkFoodOverlap(const double& x, const double& y, const int& radius = SPRITE_WIDTH);  
    bool checkDirtMovementOverlap(const double& x, const double& y);
    void getRandomAngle(int& angle);
    bool getClosestFoodPosAngle(const double& x, const double& y, int& angle);
    int getBacteriaDirection(const double& x1, const double& y1, const double& x2, const double& y2);
    int getBacteriaDirectionForSocrates(const double& x, const double& y);
    void incrNeededToComplete();
    void addActor(Actor* a);

private:
    bool checkOverlap(Actor* s, const double& x2, const double& y2, const int& overlap = SPRITE_WIDTH) const; //returns true if overlap
    void getRandomLocation(double& randX, double& randY) const;
    void getOpenLocation(const int& numToCheck, double& randX, double& randY);
    void insertInCircle(const int& m_numToInsert, const char& ch);
    int getChanceToInsert(const int& chance);
    void insertEdgeCircle(const int& chance, const char& ch);

    //member variables
    int m_numActors;
    int m_numDeadForLevel;   //current number of dead actors that prevent level from completing
    int m_numDeadNeededForLevel;  //number of needed dead actors that prevent level from completing
    std::list<Actor*> m_actors;
    Socrates* m_socrates;
};

#endif // STUDENTWORLD_H_