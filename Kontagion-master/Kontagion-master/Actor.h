#ifndef ACTOR_H_
#define ACTOR_H_
#include "GraphObject.h"
#include "GameConstants.h"
class StudentWorld;

const int SOCRATES_MAX_HITPOINTS = 100;
const int CLOCKWISE = -5;
const int COUNTERCLOCKWISE = 5;
const int MAX_SPRAY_CHARGES = 20;
const int FLAME_MAX_DISTANCE_TRAVELED = 32;
const int SPRAY_MAX_DISTANCE_TRAVELED = 112;
const int FLAME_DAMAGE = 5;
const int SPRAY_DAMAGE = 2;
const int REGULAR_SALMONELLA_DAMAGE = 1;
const int AGGRESSIVE_SALMONELLA_DAMAGE = 2;
const int E_COLI_DAMAGE = 4;
const int AGGRESSIVE_SALMONELLA_RADIUS = 72;
const int SALMONELLA_MOVEMENT = 3;
const int ECOLI_MOVEMENT = 2;
const int NUM_FLAMES_CREATED = 16;

class Actor : public GraphObject               
{
public:
	Actor(double x, double y, int imageID, StudentWorld* world, int dir = right, int depth = 0);   
	virtual ~Actor();
	virtual void doSomething() = 0;
	bool isAlive() const;
	void setDead();
	StudentWorld* getWorld() const;
	virtual bool hasLifetime() const;
	virtual bool canBlockMovement() const;
	virtual bool canBeEaten() const;
	virtual bool eatsFood() const;
	virtual bool preventsLevelCompleting() const;

private:
	StudentWorld* m_world;
	bool m_alive;
};

class LifetimeActor : public Actor
{
public:
	LifetimeActor(int lifetime, double x, double y, int ID, StudentWorld* world, int dir = right, int depth = 1);
	virtual ~LifetimeActor();
	void doSomething();
	virtual bool hasLifetime() const;

protected:
	void checkLifetime();
	void decreaseLifetime();
	virtual void playSound() = 0;
	virtual void affectScore() = 0;
	virtual void affectSocrates() = 0;

private:
	int m_lifetime;
};

class HitPointsActor : public Actor
{
public:
	HitPointsActor(int hitpoints, double x, double y, int ID, StudentWorld* world, int dir);
	virtual ~HitPointsActor();
	int getHitPoints() const;
	virtual void adjustHitPoints(int value);  

protected:
	virtual void playSound() = 0;
	virtual void playDeathSound() = 0;
	virtual void deadStuff();

private:
	int m_hitPoints;
};

class Weapon : public Actor
{
public:
	Weapon(double x, double y, int ID, StudentWorld* world, int dir, int travelDistance, int amountDamage);
	virtual ~Weapon();
	void doSomething();

protected:
	int getDamage() const;
	
private:
	int m_pixelsMoved;
	int m_maxTravelDistance;
	int m_amountDamage;
};

class Bacteria : public HitPointsActor
{
public:
	Bacteria(int hitpoints, double x, double y, int ID, StudentWorld* world);
	virtual ~Bacteria();
	virtual void doSomething();	
	virtual bool eatsFood() const;
	virtual bool preventsLevelCompleting() const;

protected:
	bool checkSocratesDistance(const int& dist = SPRITE_WIDTH);
	void getCoordinateFoward(double& coord, const double& oldCoord, const int& comparer);
	void divide();
	virtual void checkMove();
	void moveTowardsFood();
	bool move(const int& moveDistance, const int& dir, bool lazy);
	virtual void findNewMove();
	virtual void playSound();
	virtual void playDeathSound();
	virtual void deadStuff();
	void turnIntoFood();
	void damageOrDivideOrEat();
	virtual int getAmountDamage() const = 0;
	virtual void addNewBacteria(const double& x, const double& y) = 0;
	bool checkInsideCircle(const double& x, const double& y) const;

private:
	int m_movementPlanDist;
	int m_foodEaten;
};

class RegularSalmonella : public Bacteria
{
public:
	RegularSalmonella(double x, double y, StudentWorld* world);

private:
	virtual int getAmountDamage() const;
	virtual void addNewBacteria(const double& x, const double& y);
};

class AggressiveSalmonella : public Bacteria
{
public:
	AggressiveSalmonella(double x, double y, StudentWorld* world);
	virtual void doSomething();

private:
	virtual int getAmountDamage() const;
	virtual void addNewBacteria(const double& x, const double& y);
};

class Ecoli : public Bacteria
{
public:
	Ecoli(double x, double y, StudentWorld* world);

private:
	virtual int getAmountDamage() const;
	virtual void addNewBacteria(const double& x, const double& y);
	virtual void checkMove();
};

class Socrates : public HitPointsActor
{
public:
	Socrates(double x, double y, StudentWorld* world);
	virtual void doSomething();
	void resetHitPoints();
	void addFlames();
	int getFlames() const;
	int getSpray() const;

protected:
	int getPosAngle() const;     

private:
	void sprayNoise();
	void flameNoise();
	virtual void playSound();
	virtual void playDeathSound();
	void move(int dir);   
	int m_sprayCharges;
	int m_flameCharges;
};

class Dirt : public Actor
{
public:
	Dirt(double x, double y, StudentWorld* world);
	virtual void doSomething();
	virtual bool canBlockMovement() const;

private:
};

class Food : public Actor
{
public:
	Food(double x, double y, StudentWorld* world);
	virtual void doSomething();
	virtual bool canBeEaten() const;

private:
};

class Goodie : public LifetimeActor
{
public:
	Goodie(int lifetime, double x, double y, int ID, StudentWorld* world);
	virtual ~Goodie();

protected:
	void playSound();
	virtual void affectScore() = 0;
	virtual void affectSocrates() = 0;

private:
};

class RestoreHealthGoodie : public Goodie
{
public:
	RestoreHealthGoodie(int lifetime, double x, double y, StudentWorld* world);

private:
	void affectScore();
	void affectSocrates();
};

class FlamethrowerGoodie : public Goodie
{
public:
	FlamethrowerGoodie(int lifetime, double x, double y, StudentWorld* world);

private:
	void affectScore();
	void affectSocrates();
};

class ExtraLifeGoodie : public Goodie
{
public:
	ExtraLifeGoodie(int lifetime, double x, double y, StudentWorld* world);

private:
	void affectScore();
	void affectSocrates();
};

class Fungus : public LifetimeActor
{
public:
	Fungus(int lifetime, double x, double y, StudentWorld* world);

private:
	void playSound();   
	void affectScore();
	void affectSocrates();
};

class Flame : public Weapon
{
public:
	Flame(double x, double y, StudentWorld* world, int dir);

private:
};

class Spray : public Weapon
{
public:
	Spray(double x, double y, StudentWorld* world, int dir);

private:
};

class Pit : public Actor
{
public:
	Pit(double x, double y, StudentWorld* world);
	virtual void doSomething();
	virtual bool preventsLevelCompleting() const;

private:
	int m_numBacteria;
	int m_numRS;
	int m_numAS;
	int m_numEcoli;
};

#endif // ACTOR_H_