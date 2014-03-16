#pragma once
#include "Entity.h"
#include "StaticGraphic.h"
#include "Sheep.h"

class Pot :
	public Entity
{
private:
	Texture & m_PadTex;
	//hgeSprite * m_PadSprite;
	StaticGraphic * m_PadD;
	StaticGraphic * m_PadA;
	int m_State;
	float m_CurrentLoad;
	float m_MaximumLoad;
	float m_CurrentTimeCook;
	float m_MaximumTimeCook;
	int m_Team;
	float m_TimeAttacked;
	float m_TimeToTip;
	float m_TimeToBattle;
	//float m_TimeDefended;
	//bool m_BeingAttacked;
	//bool m_BeingDefended;
	Sheep * m_Attacker;
	Sheep * m_Defender;
	int m_AttackerKeys;
	int m_DefenderKeys;
	int m_KeySteps;
	int m_CurrentKey;
	int m_BlipCount;
protected:
	hgeRect ResizeBox(hgeRect rect, bool flip);
public:
	Pot(Texture * tex, Texture * padtex, int x, int y, int w, int h, int team, float scale = 1);
	virtual ~Pot();
	virtual void Render();
	virtual void Update(const float & dt);
	float ReceiveCocoa(float amount, int team);
	hgeRect GetAttackRect();
	hgeRect GetDefendRect();
	int GetTeam() const {return m_Team;}
	void GetAttacked(Sheep * attacker); 
	void GetDefended(Sheep * defender);
	void CollectKeys();
};
