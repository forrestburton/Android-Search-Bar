#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>   
#include <cmath>
#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <math.h>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

StudentWorld::StudentWorld(string assetPath)
	: GameWorld(assetPath), m_numActors(0), m_numDeadForLevel(0), m_numDeadNeededForLevel(0), m_socrates(nullptr)
{}

StudentWorld::~StudentWorld()
{
	this->cleanUp();
}

int StudentWorld::init()    
{
	m_numDeadNeededForLevel = getLevel() * 10 + getLevel(); 
	m_numDeadForLevel = 0;
	m_socrates = new Socrates(0, VIEW_HEIGHT / 2, this);   

	int numPits = getLevel();
	insertInCircle(numPits, 'p');

	int numFood = min(5 * getLevel(), 25); 
	insertInCircle(numFood, 'f');

	int numDirt = max(180 - 20 * getLevel(), 20);  
	insertInCircle(numDirt, 'd');

    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
	//each alive actor does something
	for (list<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); it++)
	{
		if ((*it)->isAlive())  
			(*it)->doSomething();

		if (!m_socrates->isAlive())   
		{
			decLives();
			return GWSTATUS_PLAYER_DIED;
		}

		if (m_numDeadForLevel == m_numDeadNeededForLevel)   
		{
			playSound(SOUND_FINISHED_LEVEL);
			return GWSTATUS_FINISHED_LEVEL;
		}
	}
	m_socrates->doSomething();      

	//remove dead actors
	for (list<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); )
	{
		if ((*it)->isAlive() == false)
		{
			if ((*it)->preventsLevelCompleting())
				m_numDeadForLevel++;
			Actor* dead = (*it);
			it = m_actors.erase(it);
			delete dead;
			m_numActors--;
		}
		else
			it++;
	}

	//potentially add new actors to the game
	insertEdgeCircle(200, 'f');  //fungus
	insertEdgeCircle(250, 'g');  //goodie

	//update game status line
	int score = getScore();
	int level = getLevel();
	int lives = getLives();
	int health = m_socrates->getHitPoints();
	int sprays = m_socrates->getSpray();
	int flames = m_socrates->getFlames();
	ostringstream oss; 
	oss.fill('0');
	if (score >= 0)
		oss << "Score: " << setw(6) << score << "  Level: " << level << "  Lives: " << lives << "  Health: " << health << "  Sprays: " << sprays << "  Flames: " << flames;
	else
	{
		int negScore = 0 - score;
		oss << "Score: -" << setw(5) << negScore << "  Level: " << level << "  Lives: " << lives << "  Health: " << health << "  Sprays: " << sprays << "  Flames: " << flames;
	}
	string s = oss.str();  
	setGameStatText(s);

	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()                  
{
	delete m_socrates;
	m_socrates = nullptr;
	while(!m_actors.empty()) 
	{
		list<Actor*>::iterator it = m_actors.begin();
		Actor* dead = (*it);
		m_actors.erase(it);   
		delete dead;
		m_numActors--;
	}
}

bool StudentWorld::checkSocratesOverlap(const double& x, const double& y, const int& dist)
{
	return checkOverlap(m_socrates, x, y, dist);
}

void StudentWorld::restoreSocrates()     
{
	m_socrates->resetHitPoints();
}

void StudentWorld::hurtSocrates(const int& amount)
{
	m_socrates->adjustHitPoints(0 - amount);
}

void StudentWorld::addFlamethrowers()
{
	m_socrates->addFlames();   
}

void StudentWorld::getLocationOnCircle(const int& angle, double& x, double& y)
{
	x = 128 + (128 * cos(angle * PI / 180));
	y = 128 + (128 * sin(angle * PI / 180));
}

bool StudentWorld::checkWeaponOverlap(const double& x, const double& y, const int& amountDamage)   
{
	for (list<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); it++)
	{
		if (checkOverlap((*it), x, y))
		{
			if ((*it)->hasLifetime() || (*it)->canBlockMovement())  
			{
				(*it)->setDead();
				return true;
			}
			else if ((*it)->eatsFood())  //bacteria
			{
				static_cast<Bacteria*>(*it)->adjustHitPoints(0 - amountDamage);
				return true;
			}
		}
	}
	return false;
}

bool StudentWorld::checkFoodOverlap(const double& x, const double& y, const int& radius)  
{
	for (list<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); it++)
	{
		if (checkOverlap((*it), x, y, radius))
		{
			if ((*it)->canBeEaten())  //pizza
			{
				(*it)->setDead();
				return true;
			}
		}
	}
	return false;
}

bool StudentWorld::checkDirtMovementOverlap(const double& x, const double& y)    
{
	for (list<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); it++)
	{
		if (checkOverlap((*it), x, y, SPRITE_WIDTH / 2))
		{
			if ((*it)->canBlockMovement())  
				return true;
		}
	}
	return false;
}

void StudentWorld::getRandomLocation(double& randX, double& randY) const   //generates a random location within a radius of 120 pixels from center
{
	randX = randInt(-120, 120);  
	int y = ((int)sqrt(120.0 * 120.0 - randX * randX));   
	randY = randInt(0 - y, y);
	
	randX += VIEW_WIDTH / 2;          
	randY += VIEW_HEIGHT / 2;  
}
                                               
void StudentWorld::getOpenLocation(const int& numToCheck, double& randX, double& randY)
{
	bool b = true;
	list<Actor*>::const_iterator it = m_actors.begin();
	
	int i = 0;                   
	while (b)
	{
		getRandomLocation(randX, randY);   
		for (; i < numToCheck && it != m_actors.end(); i++)  
		{
			if (checkOverlap((*it), randX, randY))   
				break;
			it++;
		}
		if (i == numToCheck)  
			b = false;
	}
}


void StudentWorld::insertInCircle(const int& numToInsert, const char& ch) 
{
	int currentInserted = 0;
	int numToCheck = m_numActors;
	while (currentInserted < numToInsert)  
	{
		double randX;
		double randY;
		getOpenLocation(numToCheck, randX, randY);   //generates a non-overlapping random location within a radius of 120 pixels from center 

		if (ch == 'p' || ch == 'f')
		{
			if (ch == 'p')
				m_actors.push_back(new Pit(randX, randY, this));
			else if (ch == 'f') //food
				m_actors.push_back(new Food(randX, randY, this)); 
			numToCheck++;  
		}
		else if (ch == 'd') //dirt
			m_actors.push_back(new Dirt(randX, randY, this)); 
		
		currentInserted++;  
	}
	m_numActors += numToInsert;
}

int StudentWorld::getChanceToInsert(const int& chance)
{
	int randomNum = max(510 - getLevel() * 10, chance);                         
	return randInt(0, randomNum - 1);     // [0 , randomNum)
}

void StudentWorld::getRandomAngle(int& angle)
{
	angle = randInt(0, 359);
}

bool StudentWorld:: getClosestFoodPosAngle(const double& x, const double& y, int& angle) 
{
	Food* temp = nullptr;
	for (list<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); it++)    //find closest pizza to bacteria
	{
		if (checkOverlap((*it), x, y, 128))
		{
			if ((*it)->canBeEaten())  //if pizza
			{
				if (temp == nullptr)
					temp = static_cast<Food*>(*it);
				else
				{
					double tempDist = sqrt(pow((temp->getX() - x), 2) + pow((temp->getY() - y), 2));
					double currDist = sqrt(pow(((*it)->getX() - x), 2) + pow(((*it)->getY() - y), 2));
					if (currDist < tempDist)
						temp = static_cast<Food*>(*it);
				}
			}
		}
	}
	if (temp != nullptr)   //pizza found
	{
		double foodX = temp->getX();  //return its position angle
		double foodY = temp->getY();
		angle = getBacteriaDirection(x, y, foodX, foodY);
		return true;
	}
	return false;
}


void StudentWorld::insertEdgeCircle(const int& chance, const char& ch)
{
	int randNum = getChanceToInsert(chance);
	if (randNum == 0)
	{
		double randX;
		double randY;
		int randAngle;
		int lifetime;
		getRandomAngle(randAngle);
		getLocationOnCircle(randAngle, randX, randY);
		lifetime = max(randInt(0, 300 - 10 * getLevel() - 1), 50);
		m_numActors++;

		switch (ch)
		{
		case 'f':
		{
			m_actors.push_back(new Fungus(lifetime, randX, randY, this));
				break;
		}
		case 'g':
		{
			int whichGoodie = randInt(1, 10);
			switch (whichGoodie)
			{
			case 1:     //10% chance extra life goodie
			{
				m_actors.push_back(new ExtraLifeGoodie(lifetime, randX, randY, this));
				break;
			}
			case 2:
			case 3:
			case 4:    //30% chance flamethrower goodie
			{
				m_actors.push_back(new FlamethrowerGoodie(lifetime, randX, randY, this));
				break;
			}
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
			case 10:
			{
				m_actors.push_back(new RestoreHealthGoodie(lifetime, randX, randY, this));
				break;
			}
			}
		}
		}
	}
}

int StudentWorld::getBacteriaDirection(const double& x1, const double& y1, const double& x2, const double& y2)  //bacteria x1, x2
{
	double changeY = y2 - y1;
	double changeX = x2 - x1;
	int angle = ((int)(atan2((changeY), (changeX)) * 180 / PI));
	return angle;
}

int StudentWorld::getBacteriaDirectionForSocrates(const double& x, const double& y)
{
	double socX = m_socrates->getX();
	double socY = m_socrates->getY();
	int angle = getBacteriaDirection(x, y, socX, socY);
	return angle;
}

void StudentWorld::incrNeededToComplete()
{
	m_numDeadNeededForLevel++;
}

void StudentWorld::addActor(Actor* a)
{
	m_actors.push_back(a);
	m_numActors++;
}

bool StudentWorld::checkOverlap(Actor* s, const double& x2, const double& y2, const int& overlap) const
{
	double x1 = s->getX();
	double y1 = s->getY();

	if (sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2)) <= overlap)    //overlap if <= 8 pixels apart
		return true;
	return false;
}