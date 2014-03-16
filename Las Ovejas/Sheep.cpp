#include "Sheep.h"
#include "CocoaTree.h"
#include "Pot.h"

extern HGE * hge;
extern std::vector <Entity *>		entities;
extern std::vector <CocoaTree *>	trees;
extern std::vector <hgeRectExt>		tempBoundingBoxes;
extern std::vector <Sheep *>		sheep;
extern std::vector <Pot *>			pots;

extern HEFFECT attackBaa; //HEFFECT == DWORD

Sheep::Sheep(Texture * tex, int x, int y, int w, int h, int team, bool specialdemo) : Entity(*tex, x, y, w, h)
{
	hge->System_Log("SHEEP CREATED!\n");
	m_VelocityX = 500.0f;
	m_VelocityY = 300.0f;
	m_Sprite->SetZ(0.1f);
	//m_DirectionX = RIGHT;
	m_DirectionY = UP;
	m_State = 0;
	m_HarvestSpeed = 500.0f;
	m_CurrentHarvest = 0.0f;
	m_MaximumHarvest = 50000.0f;
	m_ChargeTime = 0.0f;
	m_Team = team;
	m_Special = specialdemo;

	if (team == 1)
	{
		m_FlipX = true;
		m_Sprite->SetFlip(m_FlipX, m_FlipY, true);
		m_DirectionX = LEFT;
	}
	else
		m_DirectionX = RIGHT;

	m_Sprite->SetBlendMode(BLEND_COLORMUL | BLEND_ALPHABLEND | BLEND_NOZWRITE);
	m_Sprite->SetColor(TEAM_COLORS[m_Team]);
	m_KeyCount = KeyCount();
}

Sheep::~Sheep(void)
{
	hge->System_Log("SHEEP DELETED!\n");
}

void Sheep::Update(const float & dt)
{
	if (m_State == NORMAL)
		return;
	else if (m_State == CHARGING)
	{
		//std::cout << "charge time: " << m_ChargeTime << '\n';
		if (m_ChargeTime <= 0)
		{
			Stun(1.0f);
			return;
		}
		if (Move(dt, m_VelocityX * 3 * (m_DirectionX == LEFT? - 1 : 1)))
		{
			hgeRectExt attackBox;
			float size = 1;
			attackBox = CreateTargetBox((m_W * m_ScaleX + size)/2, 0, size, m_H * m_ScaleY, true);

			for (unsigned int i = 0; i < sheep.size(); ++i)
			{
				if (sheep[i]->GetRect().Intersect(&attackBox))
				{
					if (sheep[i] == this) continue;
					sheep[i]->Hit(m_DirectionX, 0.5f);
				}
			}
			if (DEBUG)
			{
				attackBox.SetLifespan(2);
				tempBoundingBoxes.push_back(attackBox);
			}
		}
		m_ChargeTime -= dt;
	}
	else if (m_State == HIT)
	{
		if (m_Hit.Time <= 0)
		{
			Stun(1.0f);
			return;
		}
		Move(dt, m_VelocityX/2 * (m_Hit.Direction == LEFT ? -1 : 1));
		m_Hit.Time -= dt;
	}
	else if (m_State == STUNNED)
	{
		if (m_StunTime <= 0)
		{
			m_State = NORMAL;
			m_StunTime = 0;
			return;
		}
		else
			m_StunTime -= dt;
	}
		
}

void Sheep::Render()
{
	if (DEBUG == true)
		RenderBoundingBox();

	m_Sprite->RenderEx(m_X, m_Y, m_Rotation, m_ScaleX, m_ScaleY);

	if (!m_Special)
		RenderBar(m_CurrentHarvest, m_MaximumHarvest);
}

void Sheep::Up(const float & dt)
{
	if (m_State != NORMAL) return;

	Move(dt, 0, -m_VelocityY);

	m_DirectionY = UP;
}

void Sheep::Down(const float & dt)
{
	if (m_State != 0) return;

	Move(dt, 0, m_VelocityY);

	m_DirectionY = DOWN;
}

void Sheep::Left(const float & dt, bool forcemove)
{
	if (m_State != 0) return;

	m_FlipX = true;
	m_Sprite->SetFlip(m_FlipX, m_FlipY, true);
	m_DirectionX = LEFT;

	Move(dt, -m_VelocityX, 0.0f, forcemove);
}

void Sheep::Right(const float & dt, bool forcemove)
{
	if (m_State != 0) return;

	m_FlipX = false;
	m_Sprite->SetFlip(m_FlipX, m_FlipY, true);
	m_DirectionX = RIGHT;

	Move(dt, m_VelocityX, 0.0f, forcemove);

}

void Sheep::Harvest()
{
	//if (m_State != 0) return;

	hgeRectExt targetRange = CreateTargetBox(0, 0, 10 + m_W * m_ScaleX, 10 + m_H * m_ScaleY);

	if (m_CurrentHarvest >= m_MaximumHarvest)
		return;


	CocoaTree * target = NULL;
	for (unsigned int i = 0; i < trees.size(); ++i)
	{
		if (targetRange.Intersect(&(trees[i]->GetRect()) ) )
		{
			target = trees[i];
			break;
		}
	}
	
	if (target != NULL)
	{
		//std::cout << "Harvesting!\n";
		//m_CurrentHarvest += target->BeHarvested((m_HarvestSpeed > m_MaximumHarvest-m_CurrentHarvest ? m_MaximumHarvest-m_CurrentHarvest : m_HarvestSpeed), this);
		target->BeHarvested((m_HarvestSpeed > m_MaximumHarvest-m_CurrentHarvest ? m_MaximumHarvest-m_CurrentHarvest : m_HarvestSpeed), this);
	}

	if (DEBUG)
	{
		targetRange.SetLifespan(2);
		tempBoundingBoxes.push_back(targetRange);
	}
}

hgeRectExt Sheep::CreateTargetBox(float xoffset, float yoffset, const float & sizex, const float& sizey, const bool usedirection)
{

	//std::cout << m_DirectionX << '\n';
	hgeRectExt targetBox;
	float hx, hy;
	m_Sprite->GetHotSpot(&hx, &hy);

	float tempX, tempY, tempW, tempH;
	m_Sprite->GetTextureRect(&tempX, &tempY, &tempW, &tempH);


	if (usedirection)
	{
		if (m_DirectionX == LEFT)
			xoffset *= -1;
	}

	m_Sprite->SetTextureRect(tempX, tempY, sizex, sizey);

	m_Sprite->GetBoundingBoxEx(m_X - (sizex - tempW)/2 + xoffset, m_Y - (sizey - tempH)/2 + yoffset, m_Rotation, m_ScaleX, m_ScaleY, &targetBox);

	m_Sprite->SetTextureRect(tempX, tempY, tempW, tempH);

	return targetBox;
}

void Sheep::Attack()
{
	if (m_State != NORMAL) return;
	m_State = CHARGING;
	m_ChargeTime = 0.1f;
	hge->Effect_Play(attackBaa);
	//std::cout << "Effect: " << attackBaa << '\n';
}

void Sheep::Hit(const bool directionx, const float time)
{
	m_Hit.Direction = directionx;
	//m_Hit.Magnitude = magnitude;
	m_Hit.Time = time;
	m_State = HIT;
	m_CurrentHarvest = 0.0f;
}

void Sheep::Stun(const float & t)
{
	m_State = STUNNED;
	m_StunTime = t;
}

bool Sheep::Move(float dt, float magnitudex, float magnitudey, bool forcemove)
{
	bool collide = false;

	if (forcemove)
	{
		m_X += magnitudex * dt;
		m_Y += magnitudey * dt;
		return  false;
	}

	for (int i = 0; i < PHYS_ITER; ++i)
	{
		if (magnitudex != 0)
		{
			float tempX = m_X;
			m_X = m_X + dt * (magnitudex/PHYS_ITER);
			for (unsigned int i = 0; i < entities.size(); ++i)
			{
				if (entities[i] == this)
					continue;
				if (entities[i]->GetRect().Intersect(&GetRect()))
				{
					m_X = tempX;
					collide = true;
					magnitudex = 0;
					break;
				}
			}
		}
		if (magnitudey != 0)
		{
			float tempY = m_Y;
			m_Y = m_Y + dt * (magnitudey/PHYS_ITER);
			for (unsigned int i = 0; i < entities.size(); ++i)
			{
				if (entities[i] == this)
					continue;
				if (entities[i]->GetRect().Intersect(&GetRect()))
				{
					m_Y = tempY;
					collide = true;
					magnitudey = 0;
					break;
				}
			}
		}
	}
	return collide;
}
	
void Sheep::GiveCocoa()
{
	//if (m_State != 0) return;

	if (m_CurrentHarvest <= 0) return;

	hgeRectExt targetRange = CreateTargetBox(0, 0, 10 + m_W * m_ScaleX, 10 + m_H * m_ScaleY);


	Pot * target = NULL;

	for (unsigned int i = 0; i < pots.size(); ++i)
	{
		if (targetRange.Intersect(&(pots[i]->GetRect()) ) )
		{
			target = pots[i];
			break;
		}
	}
	
	if (target != NULL)
	{
		float amount = m_HarvestSpeed > m_CurrentHarvest ? m_CurrentHarvest : m_HarvestSpeed;
		m_CurrentHarvest += target->ReceiveCocoa(amount, m_Team) - amount;
	}

	if (DEBUG)
	{
		targetRange.SetLifespan(2);
		tempBoundingBoxes.push_back(targetRange);
	}
}

void Sheep::AttackPot()
{
	for (unsigned int i = 0; i < pots.size(); ++i)
	{
		if (pots[i]->GetTeam() == m_Team)
			continue;
		else if (pots[i]->GetAttackRect().Intersect(&GetRect()))
			pots[i]->GetAttacked(this);
	}
}

void Sheep::DefendPot()
{
	//if (m_State != 0) return;

	for (unsigned int i = 0; i < pots.size(); ++i)
	{
		if (pots[i]->GetTeam() != m_Team)
			continue;
		else if (pots[i]->GetDefendRect().Intersect(&GetRect()))
			pots[i]->GetDefended(this);
	}
}

void Sheep::PressKeyA()
{
	std::cout << "Key A Pressed\n";

	if (m_State == ATTACKING_POT || m_State == DEFENDING_POT)
		m_KeyCount.KeyA++;
}

void Sheep::HoldKeyA()
{
	if (m_State == NORMAL)
	{
		Harvest();
		GiveCocoa();
	}
}

void Sheep::PressKeyB()
{
	std::cout << "Key B pressed\n";

	if (m_State == ATTACKING_POT || m_State == DEFENDING_POT)
		m_KeyCount.KeyB++;
	else if (m_State == NORMAL)
		Attack();
}

void Sheep::PressKeyC()
{
	std::cout << "Key C pressed\n";
	if (m_State == ATTACKING_POT || m_State == DEFENDING_POT)
		m_KeyCount.KeyC = m_KeyCount.KeyC + 1;
	else if (m_State == NORMAL)
	{
		AttackPot();
		DefendPot();
	}
}

KeyCount Sheep::GetKeyCount()
{
	KeyCount temp = m_KeyCount;
	std::cout << "Original A: " << m_KeyCount.KeyA << "\t B: " << m_KeyCount.KeyB << "\t C: " << m_KeyCount.KeyC << '\n';
	std::cout << "Temp     A: " << temp.KeyA << "\t B: " << temp.KeyB << "\t C: " << temp.KeyC << '\n';
	m_KeyCount = KeyCount();
	return temp;
}