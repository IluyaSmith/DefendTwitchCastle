#include "stdafx.h"
#include "DefenseBuilding.h"


void DefenseBuilding::init(DefenseBuilding::type _type, sf::FloatRect _dim, sf::Texture& _textObj, sf::Texture& _textHpBar, sf::Texture& _textHpFill, sf::Font& _font)
{
	m_type = _type;
	m_dimension = _dim;

	m_mySprite.setTexture(_textObj);
	m_mySprite.setTextureRect(sf::IntRect(0, 0, 56, 48));
	m_mySprite.setPosition(sf::Vector2f(m_dimension.left, m_dimension.top));
	m_myHpBar.setTexture(_textHpBar);
	//					612						56 / 2									50 / 2
	float hp_x = m_dimension.left + m_mySprite.getLocalBounds().width / 2 - m_myHpBar.getLocalBounds().width / 2;
	m_myHpBar.setPosition(sf::Vector2f(hp_x, m_dimension.top - 10));
	m_myHpFill.setTexture(_textHpFill);
	m_myHpFill.setPosition(sf::Vector2f(hp_x , m_dimension.top - 10));

	m_myHpText.setFont(_font);
	m_myHpText.setCharacterSize(10);
	m_myHpText.setColor(sf::Color::Black);
	LevelUp();
	m_myHpText.setPosition(m_dimension.left + +m_mySprite.getLocalBounds().width / 2 - m_myHpText.getLocalBounds().width / 2, m_dimension.top - 10);

}

void DefenseBuilding::damageIt(float _dmg)
{
	m_hp -= _dmg; 
	if (m_hp < 0)
	{
		m_hp = 0;
		m_isDown = true;
		m_isRepairable = false;
	}
	else
	{
		updateHpText();
	}
}

void DefenseBuilding::repairIt(float _repairAmount)
{
	if (m_isRepairable)
	{
		if (m_hp < m_maxHp)
		{
			m_hp += _repairAmount;

			if (m_hp > m_maxHp)
				m_hp = m_maxHp;
		}
	}
	
	if (m_hp > 0)
	{
		m_isDown = false;
	}

	updateHpText();
}

int DefenseBuilding::getHpPercent()
{
	return (int)(100*(m_hp / m_maxHp));
}

void DefenseBuilding::LevelUp()
{	//=248 + (C56*35)^1.25
	m_level++;
	m_maxHp = (float)(248 + pow((m_level * 35), 1.25f));
	updateHpText();
}

cost DefenseBuilding::getLevelUpCost()
{
	cost lvlcost;
	lvlcost.gold = m_level * 10.0f;
	lvlcost.iron = m_level * 150.0f;
	lvlcost.stone = m_level * 25.0f;
	return lvlcost;
}

void DefenseBuilding::setMaxHp(float _newMaxHp)
{
	m_maxHp = _newMaxHp;
	updateHpText();
}

void DefenseBuilding::update(sf::Time _timeSinceLastUpdate)
{
	//random for testing
	/*
	int fill = rand() % 100;
	m_myHpFill.setTextureRect(sf::IntRect(0,0,(56*(fill/100.f)),48));
	//*/
	m_myHpFill.setTextureRect(sf::IntRect(0, 0, 1 + (int)(49 * (m_hp / m_maxHp)), 12));

	if (m_isDown)
	{
		//setTexture to down state
		m_mySprite.setTextureRect(sf::IntRect(58,0,56,48));
	}
	else
	{
		m_mySprite.setTextureRect(sf::IntRect(0, 0, 56, 48));
	}
}

void DefenseBuilding::updateHpText()
{
	m_myHpText.setString(std::to_string((int)m_hp) + "|" + std::to_string((int)m_maxHp));
	m_myHpText.setPosition(m_dimension.left + +m_mySprite.getLocalBounds().width / 2 - m_myHpText.getLocalBounds().width / 2, m_dimension.top - 10);
}