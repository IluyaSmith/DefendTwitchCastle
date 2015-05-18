#pragma once

#include "defines.h"
#include <SFML\Graphics.hpp>


class DefenseBuilding
{
public:
	DefenseBuilding(){};
	DefenseBuilding(std::string _name, float _hp) :
		m_name(_name),
		m_hp(_hp),
		m_maxHp(_hp) {};
	~DefenseBuilding(){};

	std::string getName() { return m_name; }

	enum type { gate, wall };

	void init(DefenseBuilding::type _type, sf::FloatRect _dim, sf::Texture& _textObj, sf::Texture& _textHpBar, sf::Texture& _textHpFill, sf::Font& _font);

	void damageIt(float _dmg);
	void repairIt(float _repairAmount);

	int getHpPercent();

	void LevelUp();
	cost getLevelUpCost();

	void canBeRepairedAgain() { m_isRepairable = true; }
	bool isDown() { return m_isDown; }

	void update(sf::Time _timeSinceLastUpdate);

	//draw Stuff
	sf::Sprite getSprite() { return m_mySprite; }
	sf::Sprite getHpBar() { return m_myHpBar; }
	sf::Sprite getHPFill() { return m_myHpFill; }
	sf::Text getHpText() { return m_myHpText; }

private:
	std::string m_name = "uninitialized";
	DefenseBuilding::type m_type;

	float m_hp = -1;
	float m_maxHp = -1;

	int m_level = 0;

	bool m_isDown = false;
	bool m_isRepairable = true;

	void updateHpText();

	void setMaxHp(float _newMaxHp);

	//draw Stuff
	sf::FloatRect m_dimension;
	sf::Sprite m_mySprite;
	sf::Sprite m_myHpBar;
	sf::Sprite m_myHpFill;
	sf::Text m_myHpText;
};

