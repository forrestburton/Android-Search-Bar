#include "Actor.h"
#include "GraphObject.h"
#include "GameConstants.h"
#include "StudentWorld.h"
#include <cmath>
using namespace std;

Actor::Actor(double x, double y, int imageID, StudentWorld* world, int dir, int depth)       
	:GraphObject(imageID, x, y, dir, depth)
{
	m_world = world;
	m_alive = true;         
}

Actor::~Actor()
{}

bool Actor::isAlive() const  
{
	return m_alive;
}

void Actor::setDead()
{
	m_alive = false;
}

StudentWorld* Actor::getWorld() const 
{
	return m_world;
}

bool Actor::hasLifetime() const
{
	return false;
}

bool Actor::canBlockMovement() const
{
	return false;
}

bool Actor::canBeEaten() const
{
	return false;
}

bool Actor::eatsFood() const
{
	return false;
}

bool Actor::preventsLevelCompleting() const
{
	return false;
}

LifetimeActor::LifetimeActor(int lifetime, double x, double y, int ID, StudentWorld* world, int dir, int depth)
	:Actor(x, y, ID, world, dir, depth), m_lifetime(lifetime)
{}

LifetimeActor::~LifetimeActor()
{}

void LifetimeActor::doSomething()
{
	if (isAlive())
	{
		if (getWorld()->checkSocratesOverlap(getX(), getY()))
		{
			affectScore();
			setDead();
			playSound();
			affectSocrates();
			return;
		}
		decreaseLifetime();
		checkLifetime();
	}
}

void LifetimeActor::checkLifetime()
{
	if (m_lifetime <= 0)
		setDead();
}

void LifetimeActor::decreaseLifetime()
{
	m_lifetime--;
}

bool LifetimeActor::hasLifetime() const
{
	return true;
}

HitPointsActor::HitPointsActor(int hitpoints, double x, double y, int ID, StudentWorld* world, int dir)
	:Actor(x, y, ID, world, right, dir), m_hitPoints(hitpoints)
{}
HitPointsActor::~HitPointsActor()
{}

int HitPointsActor::getHitPoints() const
{
	return m_hitPoints;
}

void HitPointsActor::adjustHitPoints(int value)
{
	m_hitPoints += value;
	if (m_hitPoints <= 0)
	{
		playDeathSound();
		deadStuff();
		setDead();
		return;
	}
	else if (value < 0)
		playSound();
}

void HitPointsActor::deadStuff()
{}

Goodie::Goodie(int lifetime, double x, double y, int ID, StudentWorld* world)
	:LifetimeActor(lifetime, x, y, ID, world)
{}
Goodie::~Goodie()
{}

void Goodie::playSound()
{
	getWorld()->playSound(SOUND_GOT_GOODIE);
}

Weapon::Weapon(double x, double y, int ID, StudentWorld* world, int dir, int travelDistance, int amountDamage)
	:Actor(x, y, ID, world, dir, 1), m_pixelsMoved(0), m_maxTravelDistance(travelDistance), m_amountDamage(amountDamage)
{}

Weapon::~Weapon()
{}

void Weapon::doSomething()
{
	if (isAlive())
	{
		if (getWorld()->checkWeaponOverlap(getX(), getY(), getDamage()))
		{
			setDead();
			return;
		}
		moveAngle(getDirection(), SPRITE_WIDTH);
		m_pixelsMoved += SPRITE_WIDTH;
		if (m_pixelsMoved == m_maxTravelDistance)
			setDead();
	}
}

int Weapon::getDamage() const
{
	return m_amountDamage;
}

Bacteria::Bacteria(int hitpoints, double x, double y, int ID, StudentWorld* world)
	:HitPointsActor(hitpoints, x, y, ID, world, up), m_movementPlanDist(0), m_foodEaten(0)
{}

Bacteria::~Bacteria()
{}

void Bacteria::doSomething()
{
	if (isAlive())    
	{
		damageOrDivideOrEat();
		checkMove();	
	}
}

bool Bacteria::checkSocratesDistance(const int& dist)
{
	return getWorld()->checkSocratesOverlap(getX(), getY(), dist);
}

void Bacteria::getCoordinateFoward(double& coord, const double& oldCoord, const int& comparer)
{
	if (oldCoord < comparer)
		coord = oldCoord + SPRITE_WIDTH / 2;
	else if (oldCoord > comparer)
		coord = oldCoord - SPRITE_WIDTH / 2;
	else
		coord = oldCoord;
}

void Bacteria::divide()
{
	double newX;
	double oldX = getX();
	getCoordinateFoward(newX, oldX, VIEW_WIDTH / 2);

	double newY;
	double oldY = getY();
	getCoordinateFoward(newY, oldY, VIEW_HEIGHT / 2);

	addNewBacteria(newX, newY);
	getWorld()->incrNeededToComplete();
	m_foodEaten = 0;
}

void Bacteria::checkMove()
{
	if (m_movementPlanDist > 0)
	{
		m_movementPlanDist--;
		move(SALMONELLA_MOVEMENT, getDirection(), false);
		return;
	}
	else
		moveTowardsFood();
}

void Bacteria::moveTowardsFood()
{
	int angle;
	if (getWorld()->getClosestFoodPosAngle(getX(), getY(), angle))    //if food is found within a radius of 128 pixels, move toward it
		move(SALMONELLA_MOVEMENT, angle, false);
	else
		findNewMove();
}	

bool Bacteria::move(const int& moveDistance, const int& dir, bool lazy)
{
	double newX = getX();           
	double newY = getY();
	getPositionInThisDirection(dir, moveDistance, newX, newY);

	if (!getWorld()->checkDirtMovementOverlap(newX, newY) && checkInsideCircle(newX, newY))
	{
		setDirection(dir);  
		moveAngle(getDirection(), moveDistance);
		return true;
	}
	else if (lazy == false)
	{
		findNewMove();
		return false;
	}
	return false;
}

void Bacteria::findNewMove()
{
	int angle;
	getWorld()->getRandomAngle(angle);
	setDirection(angle);
	m_movementPlanDist = 10;
}

void Bacteria::playSound()
{
	getWorld()->playSound(SOUND_SALMONELLA_HURT);
}

void Bacteria::playDeathSound()
{
	getWorld()->playSound(SOUND_SALMONELLA_DIE);
}

void Bacteria::deadStuff()
{
	getWorld()->increaseScore(100);
	turnIntoFood();
}

void Bacteria::turnIntoFood()
{
	int randNum = randInt(1, 2);
	if (randNum == 1)
	{
		Food* f = new Food(getX(), getY(), getWorld());
		getWorld()->addActor(f);
	}
}

void Bacteria::damageOrDivideOrEat()
{
	if (checkSocratesDistance())
		getWorld()->hurtSocrates(getAmountDamage());
	else if (m_foodEaten == 3)
		divide();
	else if (getWorld()->checkFoodOverlap(getX(), getY()))
		m_foodEaten++;
}

bool Bacteria::eatsFood() const
{
	return true;
}

bool Bacteria::preventsLevelCompleting() const
{
	return true;
}

bool Bacteria::checkInsideCircle(const double& x, const double& y) const
{
	double val = sqrt(pow((x - VIEW_WIDTH / 2), 2) + pow((y - VIEW_HEIGHT / 2), 2));
	if (val < VIEW_RADIUS)   //check if coord is inside circle
		return true;
	return false;
}

RegularSalmonella::RegularSalmonella(double x, double y, StudentWorld* world)
	:Bacteria(4, x, y, IID_SALMONELLA, world)
{}

int RegularSalmonella::getAmountDamage() const
{
	return REGULAR_SALMONELLA_DAMAGE;
}

void RegularSalmonella::addNewBacteria(const double& x, const double& y)   
{
	RegularSalmonella* as = new RegularSalmonella(x, y, getWorld());
	getWorld()->addActor(as);
}

AggressiveSalmonella::AggressiveSalmonella(double x, double y, StudentWorld* world)
	:Bacteria(10, x, y, IID_SALMONELLA, world)
{}

void AggressiveSalmonella::doSomething()
{
	if (isAlive())
	{
		if (checkSocratesDistance(AGGRESSIVE_SALMONELLA_RADIUS))
		{
			double x = getX();
			double y = getY();
			int angle = getWorld()->getBacteriaDirectionForSocrates(x, y);
			move(SALMONELLA_MOVEMENT, angle, true);
			damageOrDivideOrEat();
			return;
		}
		else
			Bacteria::doSomething();
	}
}

int AggressiveSalmonella::getAmountDamage() const
{
	return AGGRESSIVE_SALMONELLA_DAMAGE;
}

void AggressiveSalmonella::addNewBacteria(const double& x, const double& y)   
{
	AggressiveSalmonella* as = new AggressiveSalmonella(x, y, getWorld());
	getWorld()->addActor(as);
}

Ecoli::Ecoli(double x, double y, StudentWorld* world)
	:Bacteria(5, x, y, IID_ECOLI, world)
{}

int Ecoli::getAmountDamage() const
{
	return E_COLI_DAMAGE;
}

void Ecoli::addNewBacteria(const double& x, const double& y)
{
	Ecoli* e = new Ecoli(x, y, getWorld());
	getWorld()->addActor(e);
}

void Ecoli::checkMove()
{
	double x = getX();
	double y = getY();
	int angle = getWorld()->getBacteriaDirectionForSocrates(x, y);
	for (int i = 0; i < 10; i++)
	{
		if (move(ECOLI_MOVEMENT, angle, true))
			break;
		else
			angle += 10;
	}
}

Socrates::Socrates(double x, double y, StudentWorld* world)
	:HitPointsActor(SOCRATES_MAX_HITPOINTS, x, y, IID_PLAYER, world, right), m_sprayCharges(20), m_flameCharges(5)
{}

void Socrates::doSomething()
{
	int ch;
	if (getWorld()->getKey(ch))
	{
		switch(ch)
		{
		case KEY_PRESS_SPACE:
		{
			if (m_sprayCharges > 0)
			{
				double currX = getX();
				double currY = getY();
				getPositionInThisDirection(getDirection(), SPRITE_WIDTH, currX, currY);
				Spray* s = new Spray(currX, currY, getWorld(), getDirection());   
				getWorld()->addActor(s);
				m_sprayCharges--;
				sprayNoise();
			}
			break;
		}
		case KEY_PRESS_ENTER:
		{
			if (m_flameCharges > 0)
			{
				double currX;
				double currY;
				int dir = getDirection();
				for (int i = 0; i < NUM_FLAMES_CREATED; i++)
				{
					currX = getX();
					currY = getY();
					getPositionInThisDirection(dir, SPRITE_WIDTH, currX, currY);
					Flame* f = new Flame(currX, currY, getWorld(), dir);
					getWorld()->addActor(f);
					dir += 22;
				}
				m_flameCharges--;
				flameNoise();
			}
			break;
		}
		case KEY_PRESS_LEFT:    //move counterclockwise 5 degrees
		{
			move(COUNTERCLOCKWISE);
			break;
		}
		case KEY_PRESS_RIGHT:      //move clockwise 5 degrees
		{
			move(CLOCKWISE);
			break;
		}
		}
	}
	else if (m_sprayCharges < MAX_SPRAY_CHARGES)
		m_sprayCharges++;
}

void Socrates::move(int dir)  //moves socrates
{
	int angle = getPosAngle() + dir;
	double x;
	double y;
	getWorld()->getLocationOnCircle(angle, x, y);
	moveTo(x, y);
	setDirection(getDirection() + dir);
}

void Socrates::resetHitPoints()   //reset Socrates hitpoints to 100
{
	int newHitPoints = SOCRATES_MAX_HITPOINTS - getHitPoints();
	adjustHitPoints(newHitPoints);
}

void Socrates::addFlames()
{
	m_flameCharges += 5;
}

int Socrates::getFlames() const
{
	return m_flameCharges;
}

int Socrates::getSpray() const
{
	return m_sprayCharges;
}

void Socrates::sprayNoise()
{
	getWorld()->playSound(SOUND_PLAYER_SPRAY);
}

void Socrates::flameNoise()
{
	getWorld()->playSound(SOUND_PLAYER_FIRE);
}

void Socrates::playSound()
{
	getWorld()->playSound(SOUND_PLAYER_HURT);
}

void Socrates::playDeathSound()
{
	getWorld()->playSound(SOUND_PLAYER_DIE);
}

int Socrates::getPosAngle() const
{
	int pos;
	pos = getDirection() - 180;
	if (pos < 0)
		pos += 360;
	return pos;
}

Dirt::Dirt(double x, double y, StudentWorld* world)
	:Actor(x, y, IID_DIRT, world, right, 1)    //ID of dirt, dir of right(0), can block movement
{}

void Dirt::doSomething()
{}

bool Dirt::canBlockMovement() const
{
	return true;
}

Food::Food(double x, double y, StudentWorld* world)  // ID of food, dir of up(90), cannot block movement
	:Actor(x, y, IID_FOOD, world, up, 1)
{}

void Food::doSomething()
{}

bool Food::canBeEaten() const
{
	return true;
}

RestoreHealthGoodie::RestoreHealthGoodie(int lifetime, double x, double y, StudentWorld* world)
	:Goodie(lifetime, x, y, IID_RESTORE_HEALTH_GOODIE, world)
{}

void RestoreHealthGoodie::affectScore()
{
	getWorld()->increaseScore(250);
}

void RestoreHealthGoodie::affectSocrates()
{
	getWorld()->restoreSocrates();
}

FlamethrowerGoodie::FlamethrowerGoodie(int lifetime, double x, double y, StudentWorld* world)
	:Goodie(lifetime, x, y, IID_FLAME_THROWER_GOODIE, world)
{}

void FlamethrowerGoodie::affectScore()
{
	getWorld()->increaseScore(300);
}

void FlamethrowerGoodie::affectSocrates()
{
	getWorld()->addFlamethrowers();
}

ExtraLifeGoodie::ExtraLifeGoodie(int lifetime, double x, double y, StudentWorld* world)
	:Goodie(lifetime, x, y, IID_EXTRA_LIFE_GOODIE, world)
{}

void ExtraLifeGoodie::affectScore()
{
	getWorld()->increaseScore(500);
}

void ExtraLifeGoodie::affectSocrates()
{
	getWorld()->incLives();
}

Fungus::Fungus(int lifetime, double x, double y, StudentWorld* world)
	:LifetimeActor(lifetime, x, y, IID_FUNGUS, world)
{}

void Fungus::playSound()
{}

void Fungus::affectScore()
{
	getWorld()->increaseScore(-50);
}

void Fungus::affectSocrates()
{
	getWorld()->hurtSocrates(20);
}

Flame::Flame(double x, double y, StudentWorld* world, int dir)
	:Weapon(x, y, IID_FLAME, world, dir, FLAME_MAX_DISTANCE_TRAVELED, FLAME_DAMAGE)
{}

Spray::Spray(double x, double y, StudentWorld* world, int dir)
	:Weapon(x, y, IID_SPRAY, world, dir, SPRAY_MAX_DISTANCE_TRAVELED, SPRAY_DAMAGE)
{}

Pit::Pit(double x, double y, StudentWorld* world)
	:Actor(x, y, IID_PIT, world, right, 1), m_numBacteria(10), m_numRS(5), m_numAS(3), m_numEcoli(2)
{}

void Pit::doSomething()
{
	if (m_numBacteria == 0)
	{
		setDead();
		return;
	}
	else
	{
		int randNum = randInt(1, 50);       //1 in 50 chance a bacteria will spawn
		if (randNum == 1)
		{
			bool hasInserted = false;
			while (!hasInserted)
			{
				int randNum2 = randInt(1, 3);
				switch (randNum2)
				{
				case 1:             
				{
					if (m_numRS > 0)
					{
						RegularSalmonella* es = new RegularSalmonella(getX(), getY(), getWorld());
						getWorld()->addActor(es);
						m_numRS--;
						hasInserted = true;
					}
					break;
				}
				case 2:
					if (m_numAS > 0)
					{
						AggressiveSalmonella* as = new AggressiveSalmonella(getX(), getY(), getWorld());
						getWorld()->addActor(as);
						m_numAS--;
						hasInserted = true;
					}
					break;
				case 3:
					if (m_numEcoli > 0)
					{
						Ecoli* e = new Ecoli(getX(), getY(), getWorld());
						getWorld()->addActor(e);
						m_numEcoli--;
						hasInserted = true;
					}
					break;
				}
			}
			getWorld()->playSound(SOUND_BACTERIUM_BORN);
			m_numBacteria--;
		}
	}
}

bool Pit::preventsLevelCompleting() const
{
	return true;
}