#include "Pot.h"

#include <vector>
#include "hgeRectExt.h"
extern std::vector <hgeRectExt> tempBoundingBoxes;
extern std::vector <StaticGraphic *> buttons;
extern HEFFECT prepare;
extern HEFFECT humil;
extern HEFFECT blip;

Pot::Pot(Texture * tex, Texture * padtex, int x, int y, int w, int h, int team, float scale) : Entity(*tex, x, y, w, h), m_PadTex(*padtex)
{
	m_Team = team;
	m_CurrentLoad = m_CurrentTimeCook = 0;
	m_MaximumTimeCook = 30.0f;
	m_MaximumLoad = 200000;
	m_ScaleX = scale;
	m_ScaleY = scale;
	m_PadTex.AddRef();
	//m_PadSprite = new hgeSprite(m_PadTex.GetTex(), 0, 0, m_PadTex.GetWidth(), m_PadTex.GetHeight());
	
	float padsizex = 60 * m_ScaleX;
	float padsizey = 60 * m_ScaleY;

	if (team)
	{
		if (team) m_FlipX = true;
		m_Sprite->SetFlip(m_FlipX, m_FlipY, true);

		m_PadD = new StaticGraphic(padtex, m_X + (m_W * m_ScaleX)/2 + 25, m_Y, padsizex, padsizey, 0.8f, true);
		m_PadD->GetSprite().SetFlip(true, false, true);
		m_PadA = new StaticGraphic(padtex, m_X - (m_W * m_ScaleX)/2 - 25, m_Y, padsizex, padsizey, 0.8f, true);
		m_PadA->GetSprite().SetFlip(true, false, true);
	}
	else
	{
		m_PadA = new StaticGraphic(padtex, m_X + (m_W * m_ScaleX)/2 + 25, m_Y, padsizex, padsizey, 0.8f, true);
		m_PadD = new StaticGraphic(padtex, m_X - (m_W * m_ScaleX)/2 - 25, m_Y, padsizex, padsizey, 0.8f, true);
	}

	//m_BeingAttacked = m_BeingDefended = false;
	m_TimeAttacked = 0.0f;
	m_TimeToTip = 3.0f;
	m_TimeToBattle = 10.0f;
	m_Attacker = m_Defender = NULL;

	//m_AttackerKeys = m_Defenderkeys = KeyCount();
	m_AttackerKeys = m_DefenderKeys = 0;
	m_KeySteps = 0;


}

Pot::~Pot()
{
	m_PadTex.DelRef();
	delete m_PadA;
	delete m_PadD;
}

void Pot::Render()
{
	if (DEBUG)
	{
		RenderBoundingBox();
		//m_PadA->RenderBoundingBox();
		//m_PadD->RenderBoundingBox();
	}

	if (m_Attacker && m_Defender)
	{
		buttons[m_CurrentKey]->GetSprite().Render(m_X, m_Y-120);
	}

	m_Sprite->RenderEx(m_X, m_Y, m_Rotation, m_ScaleX, m_ScaleY);

	RenderBar(m_CurrentLoad, m_MaximumLoad, 0, -20, 50, 0xFFAAAA00);
	RenderBar(m_CurrentTimeCook, m_MaximumTimeCook, 0, (10 + m_H * m_ScaleY));

	//m_PadSprite->RenderEx(m_X + (m_W + 10)*m_ScaleX, m_Y, m_Rotation, m_ScaleX, m_ScaleY);
	m_PadA->Render();
	m_PadD->Render();

}

void Pot::Update(const float &dt)
{
	if (m_Attacker != NULL && m_Defender == NULL)
	{
		if (m_TimeAttacked > m_BlipCount)
		{
			m_BlipCount++;
			hge->Effect_Play(blip);
		}
		m_TimeAttacked += dt;
		//std::cout << "Time attacked: " << m_TimeAttacked << '\n';
		if (m_TimeAttacked > m_TimeToTip)
		{
			m_Attacker->StopAttacking();
			//Tip Pot
			m_CurrentLoad = 0;
			m_CurrentTimeCook = 0;
			m_Attacker = NULL;
			m_TimeAttacked = 0;
		}
	}
	if (m_Attacker && m_Defender)
	{
		m_TimeAttacked += dt;

		if (m_KeySteps == 0)
		{
			//Flush key counts before first key
			m_Attacker->GetKeyCount();
			m_Defender->GetKeyCount();
			m_AttackerKeys = 0;
			m_DefenderKeys = 0;
			m_KeySteps++;
			m_CurrentKey = hge->Random_Int(0,2);
			std::cout << "Press key " << char(m_CurrentKey+65) << '\n';
		}

		if (m_TimeAttacked / 2  > m_KeySteps)
		{
			m_KeySteps++;
			CollectKeys();
			m_CurrentKey = hge->Random_Int(0,2);
			std::cout << "Press key " << char(m_CurrentKey+65) << '\n';
		}

		//std::cout << "Battle time = " << m_TimeAttacked << "!\n";
		if (m_TimeAttacked >= m_TimeToBattle)
		{
			//CollectKeys();
			std::cout << "Attacker had " << m_AttackerKeys << " valid keys\n";
			std::cout << "Defender had " << m_DefenderKeys << " valid keys\n";
			if (m_AttackerKeys > m_DefenderKeys)
			{
				m_CurrentLoad = 0.0f;
				m_CurrentTimeCook = 0.0f;
				std::cout << "Attacker wins\n";
				m_Attacker->StopAttacking();
				m_Defender->Stun(3.0f);
			}
			else if (m_AttackerKeys == m_DefenderKeys)
			{
				std::cout << "Draw\n";
				m_Attacker->Stun(2.0f);
				m_Defender->Stun(2.0f);
			}
			else if (m_AttackerKeys < m_DefenderKeys)
			{
				hge->Effect_Play(humil);
				std::cout << "Defender won\n";
				m_Attacker->Stun(3.0f);
				m_Defender->StopAttacking();
			}
			m_Attacker = NULL;
			m_Defender = NULL;
			m_TimeAttacked = 0.0f;
		}
	}
}

float Pot::ReceiveCocoa(float amount, int team)
{
	if (team != m_Team) return amount;

	if (m_CurrentLoad >= m_MaximumLoad)
		return amount;
	else if (m_CurrentLoad + amount > m_MaximumLoad)
	{
		float temp = m_CurrentLoad;
		m_CurrentLoad = m_MaximumLoad;
		return amount - (m_MaximumLoad - temp);
	}
	else
	{
		m_CurrentLoad += amount;
		return 0;
	}
}

hgeRect Pot::GetAttackRect()
{

	return ResizeBox(m_PadA->GetRect(), m_Team % 2);

}

hgeRect Pot::GetDefendRect()
{
	return ResizeBox(m_PadD->GetRect(), !(m_Team % 2));
}

hgeRect Pot::ResizeBox(hgeRect rect, bool flip)
{

	if (!flip)
	{
		//Makes the box smaller, and closer to the pot so the sheep has to be close and somewhat centered
		rect.x2 = rect.x1 + (rect.x2 - rect.x1)/2;
		rect.y2 = rect.y1 + (rect.y2 - rect.y1)/2;
		float temp = (rect.y2-rect.y1)/2;
		rect.y1 += temp;
		rect.y2 += temp;
	}
	else
	{
		rect.x1 = rect.x2 + (rect.x1 - rect.x2)/2;
		rect.y1 = rect.y2 + (rect.y1 - rect.y2)/2;
		float temp = (rect.y1-rect.y2)/2;
		rect.y2 += temp;
		rect.y1 += temp;
	}


	if (DEBUG)
	{
		hgeRectExt rectex(rect);

		rectex.SetLifespan(3);
		tempBoundingBoxes.push_back(rectex);
	}
	
	return rect;
}

void Pot::GetAttacked(Sheep * attacker)
{
	if (m_Attacker == NULL)
	{
		if (m_CurrentLoad > 0)
		{
			m_BlipCount = 0;
			m_Attacker = attacker;
			m_Attacker->AttackingPot();
		}
	}
}

void Pot::GetDefended(Sheep * defender)
{
	if (m_Attacker == NULL) return;

	if (m_Defender == NULL)
	{
		m_Defender = defender;
		m_Defender->DefendingPot();
		m_TimeAttacked = 0.0f;
		m_KeySteps = 0;
		hge->Effect_Play(prepare);
	}
}

void Pot::CollectKeys()
{
	switch (m_CurrentKey)
	{
	case 0:
		m_AttackerKeys += m_Attacker->GetKeyCount().KeyA;
		m_DefenderKeys += m_Defender->GetKeyCount().KeyA;
		break;
	case 1:
		m_AttackerKeys += m_Attacker->GetKeyCount().KeyB;
		m_DefenderKeys += m_Defender->GetKeyCount().KeyB;
		break;
	case 2:
		m_AttackerKeys += m_Attacker->GetKeyCount().KeyC;
		m_DefenderKeys += m_Defender->GetKeyCount().KeyC;
		break;
	}
}