#pragma once

/*
	Sheep class
	Entity type
	William Goddard 2009

	TODO:	Convert movement + collision into a function (Bad Will for letting it get this way)
			Organize data
*/

#include "Entity.h"
#include "hgeRectExt.h"

#include <vector>

#define RIGHT true
#define LEFT false
#define UP true
#define DOWN false


class Sheep :
	public Entity
{
private:
	enum state { NORMAL, CHARGING, HIT, STUNNED, ATTACKING_POT, DEFENDING_POT };
	int m_Team;
	bool m_DirectionX;
	bool m_DirectionY;
	unsigned int m_State;
	float m_VelocityX;
	float m_VelocityY;
	float m_HarvestSpeed;
	float m_CurrentHarvest;
	float m_MaximumHarvest;
	float m_ChargeTime;
	float m_StunTime;
	bool m_Special;
	struct { bool Direction; float Time; } m_Hit;
	KeyCount m_KeyCount;
protected:
	hgeRectExt CreateTargetBox(float xoffset, float yoffset, const float & sizex, const float & sizey, const bool usedirection = false);
	bool Move(float dt, float MagnitudeX, float MagnitudeY = 0.0f, bool forcemove = false);
public:
	Sheep(Texture * tex, int x, int y, int w, int h, int team = 0, bool specialdemo = false);
	~Sheep(void);
	virtual void Update(const float & dt);
	virtual void Render();
	void Up(const float & dt);
	void Down(const float & dt);
	void Left(const float & dt, bool forcemove = false);
	void Right(const float & dt, bool forcemove = false);
	void Harvest();
	void ReceiveCocoa(const float & amount) { m_CurrentHarvest += amount; }
	void GiveCocoa();
	void Attack();
	void Hit(const bool directionx, const float magnitude);
	void Stun(const float & t);
	void AttackPot();
	void DefendPot();
	void AttackingPot() { m_State = ATTACKING_POT; }
	void DefendingPot() { m_State = DEFENDING_POT; }
	void StopAttacking() { m_State = NORMAL; }
	KeyCount GetKeyCount();
	void PressKeyA();
	void HoldKeyA();
	void PressKeyB();
	void PressKeyC();
};
