#include "stdafx.h"
#include "Enemy.h"


void Enemy::init(std::shared_ptr<Waypoint> _wp, std::list< std::shared_ptr<Waypoint>> &_movePath, std::vector< std::shared_ptr<DefenseBuilding> > &_buildingList, sf::Texture& _texture, sf::Texture& _textHpBar, sf::Texture& _textHpFill)
{
	this->m_nextWaypoint = _wp;
	this->m_lastWaypoint = _wp;
	this->m_enemypos = _wp->randomPointInArea();
	this->m_wayToGo = _movePath;
	this->m_buildings = _buildingList;

	this->m_mySprite.setTexture(_texture);
	this->m_mySprite.setTextureRect(sf::IntRect(0, 0, 1, 1));

	m_myHpBar.setTexture(_textHpBar);
	m_myHpFill.setTexture(_textHpFill);

	m_myHpBar.setScale(sf::Vector2f(.32f, .32f));
	m_myHpFill.setScale(sf::Vector2f(.32f, .32f));

	switch (m_enemyType)
	{
	case classless:
		tilesetpos.x = GFX_CLASS_WARRIOR_X;
		tilesetpos.y = GFX_CLASS_WARRIOR_Y;
		break;
	case boss:
		tilesetpos.x = GFX_CLASS_WARRIOR_X;
		tilesetpos.y = GFX_CLASS_WARRIOR_Y;
		break;
	default:
		tilesetpos = sf::Vector2i(0, 0);
	}

	this->m_modus = modus::spawn;
	
	this->m_myLane = _wp->getLane();
}

std::string Enemy::toString()
{
	return "[ Enemy ] id: " + std::to_string(m_id);
}

void Enemy::damageIt(float _dmg)
{
	m_hp -= _dmg;
	if (m_hp < 0)
	{
		m_hp = 0;
		m_isDone = true;
	}
	m_myHpFill.setTextureRect(sf::IntRect(0,0,(int)(50 * (m_hp/m_maxHp)) ,12));
}

void Enemy::update(sf::Time _timeSinceLastUpdate)
{
	sf::FloatRect buildingbounds = m_buildings[m_myLane]->getSprite().getLocalBounds();
	sf::FloatRect myRect = m_mySprite.getLocalBounds();

	if ((buildingbounds.height + m_buildings[m_myLane]->getSprite().getPosition().y) > (myRect.height + m_mySprite.getPosition().y))
	{
		m_isInside = true;
	}
	else
	{
		m_isInside = false;
	}

	if (m_modus == modus::spawn)
	{
		m_modus = modus::moveToObj;
		m_isMoving = true;
		m_wayToGo.pop_front();
		m_nextPosToMoveTo = m_wayToGo.front()->randomPointInArea();
		m_lastWaypoint = m_wayToGo.front();
		m_wayToGo.pop_front();
		m_nextWaypoint = m_wayToGo.front();
	}


	sf::Vector2f dif;
	switch (m_modus)
	{
	case modus::moveToObj:

		//has arrived at point?
		if (Helper::hasReached(m_enemypos, m_nextPosToMoveTo, m_movingspeed))
		{	//We switch to idle Mode (it gets another Waypoint in the next update)
			m_modus = modus::attack;
			m_isMoving = false;

			break;
		}

		//Movement
		dif = m_nextPosToMoveTo - m_enemypos;
		move(getDirection(dif), m_movingspeed);
		break;

	case modus::attack:
		m_orientation = orientation::bot;
		
		//every sec
		//m_timeTillNextAttack -= _timeSinceLastUpdate;
		//if (m_timeTillNextAttack <= sf::Time::Zero)
		//{
		//	m_timeTillNextAttack = sf::milliseconds(500);
			//m_buildings[m_myLane]->damageIt(1.0f);
			if (m_buildings[m_myLane]->isDown())
			{	//Gate/Wall is down
				m_modus = modus::raid;
				m_nextWaypoint = m_wayToGo.front();
				m_wayToGo.pop_front();
				m_isMoving = true;
			}
	//	}

		break;
	case modus::raid:
		//raid the town
		if (Helper::hasReached(m_enemypos, m_nextWaypoint->getPos(), m_movingspeed))
		{	//We switch to idle Mode (it gets another Waypoint in the next update)
			m_modus = modus::retreat;
			m_nextWaypoint = m_wayToGo.front();
			m_wayToGo.pop_front();
			m_isMoving = true;
			break;
		}
		dif = m_nextWaypoint->getPos() - m_enemypos;
		move(getDirection(dif), m_movingspeed);

		break;
	case modus::retreat:
		//flee from town
		if (Helper::hasReached(m_enemypos, m_nextWaypoint->getPos(), m_movingspeed))
		{	//Wants to be destroyed
			m_isDone = true;
		}

		dif = m_nextWaypoint->getPos() - m_enemypos;
		move(getDirection(dif), m_movingspeed);
		break;
	default:
		std::cout << "Enemy " << m_id << " is not in a valid modus!" << std::endl;
		break;
	}

	if (m_timeTillNextAnimState <= sf::Time::Zero) {
		m_timeTillNextAnimState = sf::milliseconds(GFX_ANIMATION_TIME_BETWEEN_STATES);
		if (m_isMoving)
		{
			m_animstate = (m_animstate + 1) % 3;

			m_mySprite.setTextureRect(sf::IntRect(
				tilesetpos.x + TILESIZE_CHAR_X*m_animstate,
				tilesetpos.y + TILESIZE_CHAR_Y*m_orientation,
				TILESIZE_CHAR_X, TILESIZE_CHAR_Y));
		}
	}
	m_timeTillNextAnimState -= _timeSinceLastUpdate;

	m_myHpBar.setPosition(m_enemypos + sf::Vector2f(0, 16));
	m_myHpFill.setPosition(m_enemypos+sf::Vector2f(0,16));
	m_mySprite.setPosition(m_enemypos);
}

void Enemy::move(orientation _direction, float _length)
{
	sf::Vector2f newMovement(0.f, 0.f);

	switch (_direction) {
	case orientation::bot:
		newMovement.y += _length;
		m_orientation = orientation::bot;
		break;
	case orientation::left:
		newMovement.x -= _length;
		m_orientation = orientation::left;
		break;
	case orientation::right:
		newMovement.x += _length;
		m_orientation = orientation::right;
		break;
	case orientation::top:
		newMovement.y -= _length;
		m_orientation = orientation::top;
		break;
	default:
		std::cout << "Not a valid direction!" << std::endl;
	}
	m_enemypos += newMovement;
}