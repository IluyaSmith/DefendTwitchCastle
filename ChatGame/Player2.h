#pragma once

#include <string>
#include <memory>

#include <SFML\Graphics.hpp>
#include "Waypoint.h"
#include "Helper.h"


class Player2
{
public:
	//Never Create a Player through Constructors! use PlayerIO::addPlayer
	Player2(){};
	Player2(std::string _pname, float _xp, float _lvl, float _str, float _dex, float _mat, float _bty) :
		m_playername(_pname),
		m_xp(_xp),
		m_lvl(_lvl),
		m_str(_str),
		m_dex(_dex),
		m_mat(_mat),
		m_bty(_bty) {};
	~Player2(){};

	enum modus { movingIdle, movingPath, work, train, repair, attack, idle, invalid };
	Player2::modus getModePlayerIsIn() { return  m_modus; }

	void init(std::shared_ptr<Waypoint> _wp, sf::Texture& _texture, sf::Font& _font);

	void setId(int _newId);
	int getId() { return m_id; }

	std::string getPlayername() { return m_playername; }

	//Gamestats
	float getXp() { return this->m_xp; }
	void setXp(float _newXp) { this->m_xp = _newXp; }
	void addXp(float _difXp) { this->m_xp += _difXp; }

	float getLvl() { return m_lvl; }
	void setLvl(float _newLvl) { this->m_lvl = _newLvl; }

	float getStr() { return m_str; }
	void setStr(float _newStr) { m_str = _newStr; }
	void addStr(float _difStr) { m_str += _difStr; }
	
	float getDex() { return m_dex; }
	void setDex(float _newDex) { m_dex = _newDex; }
	void addDex(float _difDex) { m_dex += _difDex; }

	float getMat() { return m_mat; }
	void setMat(float _newMath) { m_mat = _newMath; }
	void addMat(float _difMath) { m_mat += _difMath; }

	float getBty() { return m_bty; }
	void setBty(float _newBeauty) { m_bty = _newBeauty; }
	void addBty(float _difBeauty) { m_bty += _difBeauty; }


	float getMS() { return m_movingspeed; }
	void setMS(float _newMS) { m_movingspeed = _newMS; }

	enum Class { classless, warrior, engineer, cheerleader };
	Class getClass() { return m_class; }
	void setClass(Class _newClass);

	bool isKing() { return m_isKing; }
	void setKing(bool _newisKing) { m_isKing = _newisKing; }
	bool hasVoted = false;
	int m_votes = 0;
	int m_voteid = 0;

	enum Job { jobless, str, dex, mat, bty, gold, iron, wood, stone, repairing, attacking};
	Job getJob() { return m_myJob; }

	sf::Vector2f getPos() { return m_playerpos; }

	std::string toString();

	//drawing
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
	void setOrientation(orientation _newOri) { m_orientation = _newOri; m_orientationChanged = true; }

	sf::Text*		getNumber() { return &this->m_myPlayernumber; }
	sf::Sprite		getSprite() { return this->m_mySprite; }
	bool			m_isInside = false;


	//moving
	std::shared_ptr<Waypoint> getNextWaypoint() { return this->m_nextWaypoint; }
	std::shared_ptr<Waypoint> getActualWaypoint() { return this->m_lastWaypoint; }

	void setWayToGo(std::list<std::shared_ptr<Waypoint>>& _wayToGo) { this->m_wayToGo = _wayToGo; }


	void update();

	//Jobstuff
	bool hasJobFinished() { return m_jobFinished; }
	int takeJobEarnings() { m_jobFinished = false; return m_jobvalue; }

	//SaveLoadStuff
	void setIsSaved(bool _isS) { m_isSaved = _isS; }
	bool isSaved() { return m_isSaved; }

	
private:
	int m_id = -1;
	std::string m_playername = "generic not initialized Player";
	
	//stats
	float m_xp = -1;
	float m_lvl = -1;
	
	float m_str = -1;
	float m_dex = -1;
	float m_mat = -1;
	float m_bty = -1;

	float m_movingspeed = PLAYER_SVAL_MS;
	Class m_class = Class::classless;

	bool m_isKing = false;

	//draw/move Stuff
	sf::Clock		m_UpdateClk;
	sf::Time		_timeSinceLastUpdate;
	sf::Vector2f	m_playerpos;
	orientation		m_orientation = orientation::bot;

	Player2::modus	m_modus = modus::invalid;
	sf::Time		m_timeTillNextIdlePoint = sf::Time(sf::seconds(1.0f));
	bool			m_isMoving = false;

	void		nextAnimFrame() { m_animstate = (m_animstate + 1) % 3; m_animstateChanged = true; }
	int			m_animstate = 1;
	sf::Time	m_timeTillNextAnimState;// = sf::Time(sf::milliseconds(GFX_ANIMATION_TIME_BETWEEN_STATES));

	//Jobstuff
	Job				m_myJob;
	void			setJob(Job _newJob);
	int				m_jobstate = 2;
	int				m_jobvalue = 0;
	bool			m_jobFinished = false;
	bool			m_hasReachedTarget = false;
	sf::Vector2f	m_nextTargetPoint = sf::Vector2f(0, 0);
	sf::Time		m_jobTime = sf::Time::Zero;
	sf::Time		m_jobUpdateTime = sf::seconds(1.0f);

	sf::Vector2f	m_nextIdlePoint = { 20.0f, 20.0f };
	bool			m_hasReachedJobPoint = true;

	std::vector<std::string> wpString;

	std::shared_ptr<Waypoint>				m_nextWaypoint;
	std::shared_ptr<Waypoint>				m_lastWaypoint;
	std::list<std::shared_ptr<Waypoint>>	m_wayToGo;

	void move(orientation _direction, float _length);

	sf::Sprite m_mySprite;
	sf::Text m_myPlayernumber;
	sf::Vector2i tilesetposOrigin;
	sf::Vector2i tilesetposOffset;

	//optimization stuff
	bool m_animstateChanged = false;
	bool m_orientationChanged = false;

	//SaveLoad Stuff
	bool m_isSaved = false; //does it exist in the file yet
};

