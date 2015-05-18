#pragma once

#include <string>
#include <memory>

#include <SFML\Graphics.hpp>
#include "Waypoint.h"
#include "DefenseBuilding.h"
#include "Helper.h"


class Enemy
{
public:
	Enemy(){};
	Enemy(int _id, float _maxHp, float _atk) :
		m_id(_id),
		m_maxHp(_maxHp),
		m_hp(_maxHp),
		m_atk(_atk)
		{};
	~Enemy(){};

	enum modus { spawn, moveToObj, attack, raid, retreat, invalid };
	Enemy::modus getModeEnemyIsIn() { return  m_modus; }

	//enum lane { midGate, leftGate, rightGate };
	Waypoint::lane getLane() { return m_myLane; }

	void init(std::shared_ptr<Waypoint> _wp, std::list< std::shared_ptr<Waypoint>> &_movePath, std::vector< std::shared_ptr<DefenseBuilding> > &_buildingList, sf::Texture& _texture, sf::Texture& _textHpBar, sf::Texture& _textHpFill);

	void setId(int _newId) { m_id = _newId; }
	int getId() { return m_id; }

	void setHp(float _newHp) {m_hp = _newHp;}
	float getHp() { return m_hp; }

	void setAtk(float _newAtk) { m_atk = _newAtk; }
	float getAtk() { return m_atk; }

	void damageIt(float _dmg);

	//Gamestats

	enum EnemyType { classless, boss, type1, type2 };
	EnemyType getEnemyType() { return m_enemyType; }
	void setClass(EnemyType _newClass) { m_enemyType = _newClass; }

	sf::Vector2f getPos() { return m_enemypos; }

	std::string toString();

	enum orientation { top, right, bot, left };
	orientation getDirection(sf::Vector2f _vecWantedMovement)
	{
		//x or y greater
		if (abs(_vecWantedMovement.x) <= abs(_vecWantedMovement.y))
		{	//y determinates depending on +-
			if (_vecWantedMovement.y >= 0) { return orientation::bot; }
			else { return orientation::top; }
		}
		else
		{	//x determinates depending on +-
			if (_vecWantedMovement.x >= 0) { return orientation::right; }
			else { return orientation::left; }
		}
		//return orientation::bot;
	}
	orientation getOrientation() { return m_orientation; }

	std::shared_ptr<Waypoint> getNextWaypoint() { return this->m_nextWaypoint; }
	std::shared_ptr<Waypoint> getActualWaypoint() { return this->m_lastWaypoint; }

	void setWayToGo(std::list<std::shared_ptr<Waypoint>>& _wayToGo) { this->m_wayToGo = _wayToGo; }

	sf::Sprite getSprite() { return this->m_mySprite; }

	sf::Sprite getHpBar() {return m_myHpBar;}
	sf::Sprite getHpFill() { return m_myHpFill; }

	void update(sf::Time _timeSinceLastUpdate);


	//SaveLoadStuff
	bool isDone() { return m_isDone; }
	bool isInside() { return m_isInside; }
private:
	int m_id = -1;
	float m_hp = -1;
	float m_maxHp = -1;
	float m_atk = -1;
	Waypoint::lane m_myLane;

	//stats
	float m_movingspeed = ENEMY_SVAL_MS;
	EnemyType m_enemyType = EnemyType::classless;

	//Attack
	std::vector < std::shared_ptr<DefenseBuilding> > m_buildings;
	sf::Time	m_timeTillNextAttack = sf::Time(sf::milliseconds(500));

	//draw/move Stuff
	Enemy::modus	m_modus = modus::invalid;

	sf::Vector2f	m_enemypos;
	int				m_animstate = 1;
	orientation		m_orientation = orientation::bot;
	sf::Time		m_timeTillNextAnimState = sf::Time(sf::milliseconds(GFX_ANIMATION_TIME_BETWEEN_STATES));
	bool			m_isMoving = false;
	sf::Vector2f	m_nextPosToMoveTo;

	bool			m_isInside = false;
	bool			m_hasReachedJobPoint = true;

	std::shared_ptr<Waypoint>				m_nextWaypoint;
	std::shared_ptr<Waypoint>				m_lastWaypoint;
	std::list<std::shared_ptr<Waypoint>>	m_wayToGo;

	void move(orientation _direction, float _length);

	sf::Sprite m_mySprite;

	sf::Sprite m_myHpBar;
	sf::Sprite m_myHpFill;

	sf::Vector2i tilesetpos;

	//Destr
	bool m_isDone = false;
};

