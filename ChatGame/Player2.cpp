#include "stdafx.h"
#include "Player2.h"


void Player2::init(std::shared_ptr<Waypoint> _wp, sf::Texture& _texture, sf::Font& _font)
{
	this->m_nextWaypoint = _wp;
	this->m_lastWaypoint = _wp;
	this->m_playerpos = _wp->getPos();

	this->m_myPlayernumber.setFont(_font);
	this->m_myPlayernumber.setColor(sf::Color::Black);
	this->m_myPlayernumber.setCharacterSize(GFX_TEXT_PLAYERNUMBER_FONT_SIZE);

	this->m_mySprite.setTexture(_texture);
	this->m_mySprite.setTextureRect(sf::IntRect(0, 0, 1, 1));

	this->m_timeTillNextIdlePoint = sf::milliseconds(500);
	this->m_modus = modus::movingIdle;
	this->setClass(Class::classless);

	this->m_isKing = false;
}

void Player2::setId(int _newId)
{
	if (_newId != m_id) 
	{ 
		m_id = _newId; 
		m_myPlayernumber.setString(std::to_string(m_id)); 
	} 
}

std::string Player2::toString()
{
	return "[" + m_playername + "] "
		+ "lvl:" + std::to_string(m_lvl) + " - "
		+ "xp:" + std::to_string(m_xp);
}


void Player2::setClass(Class _newClass)
{
	m_class = _newClass;

	switch (m_class)
	{
		case classless:
			tilesetposOrigin.x = GFX_CLASS_NONE_X;
			tilesetposOrigin.y = GFX_CLASS_NONE_Y;
			break;
		case warrior:
			tilesetposOrigin.x = GFX_CLASS_WARRIOR_X;
			tilesetposOrigin.y = GFX_CLASS_WARRIOR_Y;
			break;
		default:
			tilesetposOrigin = sf::Vector2i(0, 0);
	}
}

void Player2::setJob(Job _newJob)
{
	m_myJob = _newJob;

	tilesetposOffset = sf::Vector2i(0, 0);
	//Jobs are always "in the bot-orientation animation"
	switch (m_myJob)
	{
	case jobless:

		break;
	case str:
		tilesetposOffset.x += 2 + TILESIZE_CHAR_X * 3;
		tilesetposOffset.y -= TILESIZE_CHAR_Y * 2;
		break;
	case dex:
		tilesetposOffset.x += 2 + TILESIZE_CHAR_X * 3;
		tilesetposOffset.y -= TILESIZE_CHAR_Y;
		break;
	case mat:
		tilesetposOffset.x += 2 + TILESIZE_CHAR_X * 3;
		tilesetposOffset.y = 0;
		break;
	case bty:
		tilesetposOffset.x += 2 + TILESIZE_CHAR_X * 3;
		tilesetposOffset.y += TILESIZE_CHAR_Y;
		break;
	case repairing:
		tilesetposOffset.x += 4 + TILESIZE_CHAR_X * 6;
		tilesetposOffset.y -= TILESIZE_CHAR_Y * 2;
		break;
	case attacking:
		tilesetposOffset.x += 4 + TILESIZE_CHAR_X * 6;
		tilesetposOffset.y = 0;
		break;
		/*
		case wood:
		tilesetpos.x += 6 + TILESIZE_CHAR_X * 9;
		tilesetpos.y -= TILESIZE_CHAR_Y * 2;
		break;
		case stone:
		tilesetpos.x += 6 + TILESIZE_CHAR_X * 9;
		tilesetpos.y -= TILESIZE_CHAR_Y;
		break;
		case iron:
		tilesetpos.x += 6 + TILESIZE_CHAR_X * 9;
		//tilesetpos.y;
		break;
		case gold:
		tilesetpos.x += 6 + TILESIZE_CHAR_X * 9;
		tilesetpos.y += TILESIZE_CHAR_Y;
		break;
		//*/
	}
}



void Player2::update()
{
	_timeSinceLastUpdate = m_UpdateClk.restart();
	//Are we NOT in movingPath-Mode and have we a new Waypoint to go?
	if ((m_modus != modus::movingPath) && (!m_wayToGo.empty()))
	{	//-> get next WP and save the old one
		m_lastWaypoint = m_nextWaypoint;
		m_nextWaypoint = m_wayToGo.front();
		m_wayToGo.pop_front();

		m_modus = modus::movingPath;
		setJob(Job::jobless);
		m_isMoving = true;
	}

	sf::Vector2f dif;
	switch (m_modus)
	{
	case modus::movingPath:

		//has arrived at point?
		if (Helper::hasReached(m_playerpos, m_nextWaypoint->getPos(), m_movingspeed))
		{	//We switch to idle Mode (it gets another Waypoint in the next update)
			m_modus = modus::movingIdle;
			m_nextIdlePoint = m_playerpos;
			m_isMoving = false;
			//this->m_orientation = orientation::bot;
			m_timeTillNextIdlePoint = sf::seconds(1.0f);

			m_lastWaypoint = m_nextWaypoint;

			break;
		}


		//Movement
		dif = m_nextWaypoint->getPos() - m_playerpos;
		move(getDirection(dif), m_movingspeed);
		break;
	case modus::idle:

		break;
	case modus::train:
		//go to random point in area and play animation when reached
		if (!m_hasReachedJobPoint)
		{
			//is there?
			if (Helper::hasReached(m_playerpos, m_nextIdlePoint, m_movingspeed))
			{	//nahe endpunkt
				m_hasReachedJobPoint = true;
				m_orientation = orientation::bot;
				if (m_lastWaypoint->getName() == "str")	{ setJob(Job::str); }
				else if (m_lastWaypoint->getName() == "dex")	{ setJob(Job::dex); }
				else if (m_lastWaypoint->getName() == "mat")	{ setJob(Job::mat); }
				else if (m_lastWaypoint->getName() == "bty")	{ setJob(Job::bty); }
			}
			else
			{
				move(getDirection(m_nextIdlePoint - m_playerpos), m_movingspeed);
			}
		}

		break;
	case modus::work:
		//go to random point in area and play animation when reached
		if (!m_hasReachedJobPoint)
		{
			//is there?
			if (Helper::hasReached(m_playerpos, m_nextIdlePoint, m_movingspeed))
			{	//nahe endpunkt
				m_hasReachedJobPoint = true;
				m_orientation = orientation::bot;
				if (m_lastWaypoint->getName() == "gold")	{ setJob(Job::gold); }
				else if (m_lastWaypoint->getName() == "iron")	{ setJob(Job::iron); }
				else if (m_lastWaypoint->getName() == "wood")	{ setJob(Job::wood); }
				else if (m_lastWaypoint->getName() == "stone")	{ setJob(Job::stone); }
			}
			else
			{
				move(getDirection(m_nextIdlePoint - m_playerpos), m_movingspeed);
			}

			//Start working
			m_jobstate = 0;
		}


		//
		switch (m_jobstate)
		{
		case 0:
			//inside mine
			m_jobTime += _timeSinceLastUpdate;
			if (m_jobTime >= m_jobUpdateTime)
			{
				m_jobvalue++;

				m_jobTime = sf::Time::Zero;
			}

			if (m_jobvalue >= 5)
			{
				m_jobstate = 1;
				if (m_myJob == Job::gold)
					m_nextTargetPoint = sf::Vector2f(66, 472);
				else if (m_myJob == Job::iron)
					m_nextTargetPoint = sf::Vector2f(254, 455);
				else if (m_myJob == Job::stone)
					m_nextTargetPoint = sf::Vector2f(1058, 485);
				else if (m_myJob == Job::wood)
					m_nextTargetPoint = sf::Vector2f(1143, 351);

				m_nextTargetPoint += sf::Vector2f(-TILESIZE_CHAR_X/2.0f, -TILESIZE_CHAR_Y);
				m_hasReachedTarget = false;
			}
			break;
		case 1:
			//moving to stash
			if (!m_hasReachedTarget)
			{
				if (Helper::hasReached(m_playerpos, m_nextTargetPoint, m_movingspeed))
				{
					//std::cout << "at stash" << std::endl;
					m_hasReachedTarget = true;
					m_jobFinished = true;
				}
				else
				{
					move(getDirection(m_nextTargetPoint - m_playerpos), m_movingspeed);
				}
			}
			else
			{
				//at stash 
				if (!m_jobFinished)
				{	//jobending got handled
					m_jobvalue = 0;
					m_hasReachedJobPoint = false;
				}
				
			}

			break;
		case 2:
			//nothing

			break;
		default:
			std::cout << "Invalid Jobstate - Mining/chopping" << std::endl;
		}

		break;
	case modus::repair:
		if (!m_hasReachedJobPoint)
		{
			//is there?
			if (Helper::hasReached(m_playerpos, m_nextIdlePoint, m_movingspeed))
			{	//nahe endpunkt
				m_hasReachedJobPoint = true;
				m_orientation = orientation::bot;
				if (!wpString.empty())
				{
					if (wpString[1] == "Wall" || wpString[1] == "Gate")	{ setJob(Job::repairing); }
				}
				else
				{
					std::cout << "Error while looking which repair node!" << std::endl;
				}

			}
			else
			{	//moving to random point
				move(getDirection(m_nextIdlePoint - m_playerpos), m_movingspeed);
			}

			//do stuff at point
		}
		else
		{
			//std::cout << "repairing..." << std::endl;
		}
		break;
	case modus::movingIdle:

		//Last Waypoint (== The WP i'm at) is an area?
		if (this->m_lastWaypoint->getType() == Waypoint::wptype::area)
		{
			if (m_lastWaypoint->getName() == "start")
			{
				//im at the startpoint - no action needed
			}
			else if (m_lastWaypoint->getName() == "str" ||
				m_lastWaypoint->getName() == "dex" ||
				m_lastWaypoint->getName() == "mat" ||
				m_lastWaypoint->getName() == "bty")
			{
				m_modus = modus::train;
				m_nextIdlePoint = m_lastWaypoint->randomPointInArea();
				m_isMoving = true;
				m_hasReachedJobPoint = false;
			}
			else if (	m_lastWaypoint->getName() == "gold" ||
						m_lastWaypoint->getName() == "iron" ||
						m_lastWaypoint->getName() == "wood" ||
						m_lastWaypoint->getName() == "stone")
			{
				m_modus = modus::work;
				m_nextIdlePoint = m_lastWaypoint->randomPointInArea();
				m_isMoving = true;
				m_hasReachedJobPoint = false;
			} 
			else
			{
				//Split the name to look for repair and fight Nodes
				wpString = Helper::split(m_lastWaypoint->getName(), '-');
				if (!wpString.empty()) {
					if (wpString[0] == "repair") 
					{
						m_modus = modus::repair;
						m_nextIdlePoint = m_lastWaypoint->randomPointInArea();
						m_isMoving = true;
						m_hasReachedJobPoint = false;
					}
					else if (wpString[0] == "fight")
					{
						m_modus = modus::attack;
						m_nextIdlePoint = m_lastWaypoint->randomPointInArea();
						m_isMoving = true;
						m_hasReachedJobPoint = false;
					}
				}
			}

			//Enough time passed for new Point to idle to
			if (m_timeTillNextIdlePoint <= sf::Time::Zero)
			{
				m_nextIdlePoint = m_lastWaypoint->randomPointInArea();
				m_isMoving = true;
				if (LOG_IDLEPOINT) { std::cout << "nextp: " << m_nextIdlePoint.x << "," << m_nextIdlePoint.y << " @ " << m_playerpos.x << "," << m_playerpos.y << std::endl; }
				m_timeTillNextIdlePoint = sf::milliseconds(rand() % 6000 + 1500);
			}

			if (Helper::hasReached(m_playerpos, m_nextIdlePoint, m_movingspeed) )
			{	//nahe endpunkt
				m_isMoving = false;
				m_orientation = orientation::bot;
			}
			else
			{
				//ToDo: Smooth the movement, so we dont go until we get 45° movement only
				move(getDirection(m_nextIdlePoint - m_playerpos), m_movingspeed);

			}
			
			m_timeTillNextIdlePoint -= _timeSinceLastUpdate;
		}
		else
		{
			m_modus = modus::idle;
		}

		break;
	case modus::attack:
		if (!m_hasReachedJobPoint)
		{
			//is there?
			if (Helper::hasReached(m_playerpos, m_nextIdlePoint, m_movingspeed))
			{	//nahe endpunkt
				m_hasReachedJobPoint = true;
				m_orientation = orientation::bot;
				if (!wpString.empty())
				{
					if (wpString[1] == "Wall" || wpString[1] == "Gate")	{ setJob(Job::attacking); }
				}
				else
				{
					std::cout << "Error while looking which fight node!" << std::endl;
				}

			}
			else
			{	//moving to random point
				move(getDirection(m_nextIdlePoint - m_playerpos), m_movingspeed);
			}

			//do stuff at point
		}
		else
		{
			//std::cout << "attacking..." << std::endl;
		}
		break;
		break;
	default:
		std::cout << "Player " << getPlayername() << " is not in a valid modus!" << std::endl;
		break;
	}



	

	if (m_timeTillNextAnimState <= sf::Time::Zero) {
		m_timeTillNextAnimState = sf::milliseconds(GFX_ANIMATION_TIME_BETWEEN_STATES);
		if (m_isMoving)
		{
			nextAnimFrame();
		}
	}
	m_timeTillNextAnimState -= _timeSinceLastUpdate;
	//std::cout << m_timeTillNextAnimState.asMicroseconds() << std::endl;

	if (m_animstateChanged || m_orientationChanged)
	{
		m_mySprite.setTextureRect(sf::IntRect(
			tilesetposOrigin.x + tilesetposOffset.x + TILESIZE_CHAR_X*m_animstate,
			tilesetposOrigin.y + tilesetposOffset.y + TILESIZE_CHAR_Y*m_orientation,
			TILESIZE_CHAR_X, TILESIZE_CHAR_Y));
		m_animstateChanged = false;
		m_orientationChanged = false;
	}

	//playerpos.x + TILESIZE_CHAR_X / 2 - 3, playerpos.y - GFX_TEXT_PLAYERNUMBER_FONT_SIZE - 2
	m_myPlayernumber.setPosition(m_playerpos.x + TILESIZE_CHAR_X / 2 - m_myPlayernumber.getLocalBounds().width/2, m_playerpos.y-9);
	m_mySprite.setPosition(m_playerpos);
}

void Player2::move(orientation _direction, float _length)
{
	sf::Vector2f newMovement(0.f, 0.f);

	switch (_direction) {
	case orientation::bot:
		newMovement.y += _length;
		if (!(m_orientation == orientation::bot))
			setOrientation(orientation::bot);
		break;
	case orientation::left:
		newMovement.x -= _length;
		if (!(m_orientation == orientation::left))
			setOrientation(orientation::left);
		break;
	case orientation::right:
		newMovement.x += _length;
		if (!(m_orientation == orientation::right))
			setOrientation(orientation::right);
		break;
	case orientation::top:
		newMovement.y -= _length;
		if (!(m_orientation == orientation::top))
			setOrientation(orientation::top);
		break;
	default:
		std::cout << "Not a valid direction!" << std::endl;
	}
	m_playerpos += newMovement;
}