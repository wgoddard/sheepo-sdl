#include "CocoaTree.h"

CocoaTree::CocoaTree(Texture * tex, int x, int y, int w, int h) : Entity(*tex, x, y, w, h)
{
	m_CurrentCocoa = 0;
	m_MaximumCocoa = 100000;
	m_GrowthCocoa = 10000;
}

CocoaTree::~CocoaTree(void)
{
}

void CocoaTree::Update(const float & dt)
{
	m_CurrentCocoa += m_GrowthCocoa * dt;

	if (m_CurrentCocoa > m_MaximumCocoa)
		m_CurrentCocoa = m_MaximumCocoa;

	float totalRequested = 0;

	for (unsigned int i = 0; i < m_Requests.size(); ++i)
		totalRequested += m_Requests[i]->amount;

	if (totalRequested < m_CurrentCocoa)
	{
		for (unsigned int i = 0; i < m_Requests.size(); ++i)
		{
			m_Requests[i]->target->ReceiveCocoa(m_Requests[i]->amount);
			m_CurrentCocoa -= m_Requests[i]->amount;
		}
	}
	else
	{
		float average = m_CurrentCocoa / m_Requests.size();
		std::vector<Request *>::iterator i;

		for (i = m_Requests.begin(); i != m_Requests.end(); ++i)
		{
			if ( (*i)->amount > average )
			{
				(*i)->target->ReceiveCocoa(average);
				m_CurrentCocoa -= average;
			}
			else
			{
				(*i)->target->ReceiveCocoa((*i)->amount);
				m_CurrentCocoa -= (*i)->amount;
			}
		}

	}

	while (m_Requests.size() > 0)
	{
		delete m_Requests[m_Requests.size()-1];
		m_Requests.pop_back();
	}


	//std::cout << "Current: " << m_CurrentCocoa << '\n';
	//std::cout << "Current/Max: " << m_CurrentCocoa/m_MaximumCocoa << '\n';
}

void CocoaTree::Render()
{
	m_Sprite->RenderEx(m_X, m_Y, m_Rotation, m_ScaleX, m_ScaleY);

	RenderBar(m_CurrentCocoa, m_MaximumCocoa);

	if (DEBUG)
		RenderBoundingBox();
}

void CocoaTree::BeHarvested(const float & requested, Sheep * target)
{
	Request * request = new Request;
	request->target = target;
	request->amount = requested;
	m_Requests.push_back(request);
}