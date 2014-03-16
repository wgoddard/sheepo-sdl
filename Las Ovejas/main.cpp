/*
*
*   Sheep Game - September 2009
*   William Goddard
*
*/

#include <iostream>
#include <vector>

#include "hge.h"
#include "hgesprite.h"
#include "hgefont.h"
#include "hgeparticle.h"

#include "XInput.h"

#include "hgeRectExt.h"

#include "global.h"
#include "Texture.h"
#include "Sheep.h"
#include "CocoaTree.h"
#include "Box.h"
#include "StaticGraphic.h"
#include "Pot.h"

// Pointer to the HGE interface.
HGE *hge=0;

hgeFont * fnt;
HEFFECT attackBaa;
HEFFECT prepare;
HEFFECT humil;
HEFFECT blip;
HEFFECT welcome;
HEFFECT battle;
Texture * sheepTexture;
Texture * bar;

//All sheep players
std::vector <Sheep *> sheep;

//std::vector <Texture *> textures;
std::vector <Entity *> entities;

std::vector <StaticGraphic *> graphics;

std::vector <CocoaTree *> trees;

std::vector <hgeRectExt> tempBoundingBoxes;

std::vector <Pot *> pots;

std::vector <StaticGraphic *> buttons;


bool Intro();
bool FrameFunc();
bool RenderFunc();
bool TitleFrame();
bool TitleRender();
bool MenuFrame();
bool MenuRender();
bool CreditsFrame();
bool CreditsRender();
bool PauseFrame();
bool PauseRender();


float creditsPosition = S_HEIGHT;
bool CreditsFrame()
{
	creditsPosition -= 0.5f;
	if (creditsPosition < -800)
		creditsPosition = S_HEIGHT;
	if (hge->Input_GetKey())
		return true;
	return false;
}

bool CreditsRender()
{
	hge->Gfx_BeginScene();
		hge->Gfx_Clear(0);
		fnt->printf(S_WIDTH/2, creditsPosition, HGETEXT_CENTER, "%s%s%s%s%s",
			"Credits (Alphabetical)\n\n\n",
			"*Design*\n----\nKrystal\nNathan\nPaul\nWilliam\nYing", "\n\n\n*Programming*\n----\nWilliam Goddard",
			"\n\n\n*Art Work*\n----\nWilliam Goddard", "\n\n\n*Tools*\n----\nPaul Fox");
	hge->Gfx_EndScene();
	return false;
}


int pauseOption = 0;
Sheep * pauseSheep = NULL;
bool pauseDirection = true;
bool PauseFrame()
{

	if (pauseSheep->GetRect().x2 > S_WIDTH-10)
		pauseDirection = false;
	else if (pauseSheep->GetRect().x1 < 10)
		pauseDirection = true;
	float dt = hge->Timer_GetDelta();
	if (pauseDirection)
		pauseSheep->Right(dt/2, true);
	else
		pauseSheep->Left(dt/2, true);

	if (hge->Input_KeyDown(HGEK_ESCAPE))
	{
		delete pauseSheep;
		pauseSheep = NULL;
		hge->System_SetState(HGE_FRAMEFUNC, FrameFunc);
		hge->System_SetState(HGE_RENDERFUNC, RenderFunc);
		return false;
	}

	if (hge->Input_KeyDown(HGEK_DOWN))
		pauseOption++;
	if (hge->Input_KeyDown(HGEK_UP))
		pauseOption--;

	if (pauseOption < 0) pauseOption = 1;
	if (pauseOption > 1) pauseOption = 0;

	if (hge->Input_KeyDown(HGEK_ENTER))
	{
		delete pauseSheep;
		pauseSheep = NULL;
		switch(pauseOption)
		{
		case 0:
			hge->System_SetState(HGE_FRAMEFUNC, FrameFunc);
			hge->System_SetState(HGE_RENDERFUNC, RenderFunc);
			return false;
			break;
		case 1:
			/////////////////
			hge->System_SetState(HGE_FRAMEFUNC, CreditsFrame);
			hge->System_SetState(HGE_RENDERFUNC, CreditsRender);
			return false;
			break;
		}
	}

	return false;
}

bool PauseRender()
{
	hge->Gfx_BeginScene();
		hge->Gfx_Clear(0);
		pauseSheep->Render();
		fnt->printf(S_WIDTH/2, 200, HGETEXT_LEFT, "Resume");
		fnt->printf(S_WIDTH/2, 300, HGETEXT_LEFT, "Exit");
		int selection = 0;
		switch(pauseOption)
		{
		case 0:
			selection = 200;
			break;
		case 1:
			selection = 300;
			break;
		default:
			break;
		}
		fnt->printf(S_WIDTH/2 - 50, selection, HGETEXT_LEFT, "*");
	hge->Gfx_EndScene();
	return false;
}

bool TitleFrame()
{
	if (hge->Input_GetKeyState(HGEK_ENTER))
	{
		hge->System_SetState(HGE_FRAMEFUNC, MenuFrame);
		hge->System_SetState(HGE_RENDERFUNC, MenuRender);
		delete graphics[graphics.size()-1];
		graphics.pop_back();
		fnt->SetColor(0xFFFFFFFF);
	}
	return false;
}

bool TitleRender()
{
	static BYTE alpha = 0;
	static bool up = true;
	if (up)
		alpha++;
	else
		alpha--;

	if (alpha >= 255) up = false;
	else if (alpha <= 0) up = true;

	hge->Gfx_BeginScene();
		hge->Gfx_Clear(0);
		//static StaticGraphic titlegraphic(&title, 0, 0, 1024, 768);
		//titlegraphic.Render();
		graphics[graphics.size()-1]->Render();
		fnt->SetColor((alpha << 24) + 0x000000);
		fnt->printf(400, 700, HGETEXT_LEFT, "Press START to continue.");
	hge->Gfx_EndScene();
	return false;
}

bool MenuFrame()
{
	PLUGGED_IN = 0;
	for (unsigned int i = 0; i < MAX_CONTROLLERS; ++i )
	{
		DWORD dwResult;   
		XINPUT_STATE state;  
		ZeroMemory( &state, sizeof(XINPUT_STATE) );
		dwResult = XInputGetState( i, &state );
		if (dwResult = ERROR_SUCCESS)
			PLUGGED_IN++;
	}

	if (hge->Input_KeyDown(HGEK_RIGHT) || hge->Input_KeyDown(HGEK_LEFT))
	{
		NUMBER_PLAYERS = NUMBER_PLAYERS > 2 ? 2 : 4;
	}
	if (hge->Input_KeyDown(HGEK_ENTER))
	{
		hge->System_SetState(HGE_FRAMEFUNC, Intro);
		hge->System_SetState(HGE_RENDERFUNC, RenderFunc);

		//////*Sheep entities*/////////
		sheep.push_back(new Sheep(sheepTexture, S_WIDTH/2 - 65, 400, 65, 36));
		sheep.push_back(new Sheep(sheepTexture, S_WIDTH/2 + 65, 400, 65, 36, 1));
		//sheep.push_back(new Sheep(&sheepTexture, 100, 100, 65, 36, 1));

		if (NUMBER_PLAYERS == 4)
		{
			sheep.push_back(new Sheep(sheepTexture, S_WIDTH/2 - 65, 440, 65, 36));
			sheep.push_back(new Sheep(sheepTexture, S_WIDTH/2 + 65, 440, 65, 36, 1));
		}

		for (unsigned int i = 0; i < sheep.size(); ++i)
				entities.push_back(sheep[i]);
		///////////////////////
	}
	return false;
}

bool MenuRender()
{
	hge->Gfx_BeginScene();
		hge->Gfx_Clear(0);
		fnt->printf(S_WIDTH/2, 300, HGETEXT_CENTER, "%d players selected.", NUMBER_PLAYERS);
		fnt->printf(S_WIDTH/2, 500, HGETEXT_CENTER, "%d controllers plugged in.", PLUGGED_IN);
		if (PLUGGED_IN < 4 && NUMBER_PLAYERS == 4)
			fnt->printf(S_WIDTH/2, 600, HGETEXT_CENTER, "Cannot begin with 4 players until 4 controllers are inserted");
	hge->Gfx_EndScene();
	return false;
}

bool Intro()
{
	/////////////////////
	///Skip this shit
	hge->System_SetState(HGE_FRAMEFUNC, FrameFunc);
	return false;
	////////////////

	static bool played = false;
	if (!played)
	{
		hge->Effect_Play(welcome);
		played = true;
	}
	static float countdown = 0;
	countdown += hge->Timer_GetDelta();
	if (countdown > 5)
	{
		hge->System_SetState(HGE_FRAMEFUNC, FrameFunc);
		hge->Effect_PlayEx(battle, 50, 0, 1, true);
	}
	return false;
}
bool FrameFunc()
{
	float dt=hge->Timer_GetDelta();

	PLUGGED_IN = 0;
	for (unsigned int i = 0; i < NUMBER_PLAYERS; ++i )
	{
		DWORD dwResult;   
		XINPUT_STATE state;  
		ZeroMemory( &state, sizeof(XINPUT_STATE) );
		dwResult = XInputGetState( i, &state );
		if (dwResult == ERROR_SUCCESS)
			PLUGGED_IN++;

		if (state.Gamepad.sThumbLX > 10276)
			sheep[i]->Right(dt);
		if (state.Gamepad.sThumbLX < -10276)
			sheep[i]->Left(dt);
		if (state.Gamepad.sThumbLY > 10000)
			sheep[i]->Up(dt);
		if (state.Gamepad.sThumbLY < -10000)
			sheep[i]->Down(dt);
		if (state.Gamepad.bRightTrigger > 100)
			sheep[i]->Attack();
		if (state.Gamepad.wButtons & XINPUT_GAMEPAD_A)
		{
			sheep[i]->HoldKeyA();
		}
		//if (state.Gamepad.wButtons & XINPUT_GAMEPAD_B)
		//{
		//	sheep[i]->AttackPot();
		//	sheep[i]->DefendPot();
		//}
		//DWORD XInputGetKeystroke(
		//DWORD dwUserIndex,
		//DWORD dwReserved,
		//PXINPUT_KEYSTROKE pKeystroke
		//);
	}


	if (hge->Input_KeyDown(HGEK_P))
	{
		sheep[0]->GetKeyCount();
	}


	if (hge->Input_KeyDown(HGEK_Z))
	{
		DEBUG = !DEBUG;
		hge->System_SetState(HGE_HIDEMOUSE, !DEBUG);
	}

	if (DEBUG)
	{
		//std::cout << "Boxes size: " << tempBoundingBoxes.size() << '\n';
		for (unsigned int i = 0; i < tempBoundingBoxes.size(); ++i)
		{
			if (tempBoundingBoxes[i].Update(dt))
			{
				tempBoundingBoxes.erase(tempBoundingBoxes.begin() + i);
				--i;
			}
		}

		static DWORD col = 255 << 24;
		static Entity * target = sheep[0];
		
		if (hge->Input_KeyDown(HGEK_LBUTTON))
		{
			float mx, my;
			hge->Input_GetMousePos(&mx, &my);

			for (unsigned int i = 0; i < entities.size(); ++i)
			{
				if (entities[i]->GetRect().TestPoint(mx,my))
				{
					target = entities[i];
					break;
				}
			}
		}
		if (hge->Input_GetKeyState(HGEK_X))
		{
			DWORD r = ((col & 0x00FF0000) >> 16);
			r++;
			if (r > 255) r = 0;
			col = (255 << 24) + (r << 16) + (col & 0x0000FFFF);
			target->GetSprite().SetColor(col);

		}
		if (hge->Input_GetKeyState(HGEK_C))
		{
			DWORD g = ((col & 0x0000FF00) >> 8);
			g++;
			if (g > 255) g = 0;
			col = (255 << 24) + (g << 8) + (col & 0x00FF00FF);
			target->GetSprite().SetColor(col);
		}
		if (hge->Input_GetKeyState(HGEK_V))
		{
			DWORD b = ((col & 0x000000FF) );
			b++;
			if (b > 255) b = 0;
			col = (255 << 24) + (col & 0x00FFFF00) + b;
			target->GetSprite().SetColor(col);
		}
		if (hge->Input_KeyDown(HGEK_B))
		{
			char hex[9];
			_itoa_s(col, hex, 9, 16);
			std::cout << "\nTarget Color: " << hex << '\n';
			_itoa_s(TEAM_COLORS[0], hex, 9, 16);
			std::cout << "Team 1 color: " << hex;
			_itoa_s(TEAM_COLORS[1], hex, 9, 16);
			std::cout << "\tTeam 2 color: " << hex << '\n';
		}
	}

	if (hge->Input_GetKeyState(HGEK_W))
		sheep[0]->Up(dt);
	else if (hge->Input_GetKeyState(HGEK_S))
		sheep[0]->Down(dt);
	if (hge->Input_GetKeyState(HGEK_A))
		sheep[0]->Left(dt);
	else if (hge->Input_GetKeyState(HGEK_D))
		sheep[0]->Right(dt);
	if (hge->Input_GetKeyState(HGEK_SPACE))
	{
		sheep[0]->HoldKeyA();
	}
	if (hge->Input_KeyDown(HGEK_SPACE))
		sheep[0]->PressKeyA();
	if (hge->Input_KeyDown(HGEK_Q))
		sheep[0]->PressKeyB();
	if (hge->Input_KeyDown(HGEK_E))
	{
		sheep[0]->PressKeyC();
	}


	if (hge->Input_GetKeyState(HGEK_UP))
		sheep[1]->Up(dt);
	else if (hge->Input_GetKeyState(HGEK_DOWN))
		sheep[1]->Down(dt);
	if (hge->Input_GetKeyState(HGEK_LEFT))
		sheep[1]->Left(dt);
	else if (hge->Input_GetKeyState(HGEK_RIGHT))
		sheep[1]->Right(dt);
	if (hge->Input_GetKeyState(HGEK_NUMPAD0))
		sheep[1]->HoldKeyA();
	if (hge->Input_KeyDown(HGEK_NUMPAD0))
		sheep[1]->PressKeyA();
	if (hge->Input_KeyDown(HGEK_NUMPAD1))
		sheep[1]->PressKeyB();
	if (hge->Input_KeyDown(HGEK_NUMPAD2))
	{
		sheep[1]->PressKeyC();
	}

	for (unsigned int i = 0; i < entities.size(); ++i)
		entities[i]->Update(dt);


	if (hge->Input_KeyDown(HGEK_ESCAPE))
	{
		//return true;
		////////////////////
		if (pauseSheep == NULL)
			pauseSheep = new Sheep(sheepTexture, 100, S_HEIGHT/2, 65, 36, 1, true);
		//////////////////////////
		hge->System_SetState(HGE_FRAMEFUNC, PauseFrame);
		hge->System_SetState(HGE_RENDERFUNC, PauseRender);
		return false;
	}

	return false;
}


bool RenderFunc()
{
	hge->Gfx_BeginScene();
		hge->Gfx_Clear(0);
		for (unsigned int i = 0; i < graphics.size(); ++i)
			graphics[i]->Render();

		for (unsigned int i = 0; i < entities.size(); ++i)
			entities[i]->Render();

		if (PLUGGED_IN < NUMBER_PLAYERS)
			fnt->printf(S_WIDTH/2, 100, HGETEXT_CENTER, "Please enable %d controllers.", NUMBER_PLAYERS-PLUGGED_IN); 

		if (DEBUG)
		{
			fnt->printf(5, 5, HGETEXT_LEFT, "dt:%.3f\nFPS:%d", hge->Timer_GetDelta(), hge->Timer_GetFPS());
			for (unsigned int i = 0; i < tempBoundingBoxes.size(); ++i)
				tempBoundingBoxes[i].Render();
		}

		//hge->Gfx_RenderLine(0,500,500,501);

	hge->Gfx_EndScene();

	return false;
}


//int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
int main (int argc, char * argv[])
{
	hge = hgeCreate(HGE_VERSION);

	hge->System_SetState(HGE_LOGFILE, LOGFILE.c_str());
	hge->System_SetState(HGE_FRAMEFUNC, TitleFrame);
	hge->System_SetState(HGE_RENDERFUNC, TitleRender);
	hge->System_SetState(HGE_TITLE, NAME.c_str());
	hge->System_SetState(HGE_FPS, 100);
	hge->System_SetState(HGE_WINDOWED, S_WINDOWED);
	hge->System_SetState(HGE_SCREENWIDTH, S_WIDTH);
	hge->System_SetState(HGE_SCREENHEIGHT, S_HEIGHT);
	hge->System_SetState(HGE_SCREENBPP, S_BPP);
	hge->System_SetState(HGE_ZBUFFER, true);
	hge->System_SetState(HGE_SHOWSPLASH, false);
	if (DEBUG)
		hge->System_SetState(HGE_HIDEMOUSE, !DEBUG);


	if(hge->System_Initiate()) 
	{

		DWORD dwResult;    
		for (DWORD i=0; i< MAX_CONTROLLERS; i++ )
		{
			XINPUT_STATE state;
			ZeroMemory( &state, sizeof(XINPUT_STATE) );

			// Simply get the state of the controller from XInput.
			dwResult = XInputGetState( i, &state );

			if( dwResult == ERROR_SUCCESS )
			{ 
				std::cout << "Controller " << i << " connected\n";
			}
			else
			{
				std::cout << "Controller " << i << " not conntected\n";	// Controller is not connected 
			}
		}




		// Load a font
		fnt=new hgeFont("font1.fnt");
		fnt->SetZ(0.0f);

		attackBaa = hge->Effect_Load("attack.mp3");
		prepare = hge->Effect_Load("prepare.mp3");
		humil = hge->Effect_Load("humiliation.mp3");
		blip = hge->Effect_Load("blip.wav");
		welcome = hge->Effect_Load("welcome.mp3");
		battle = hge->Effect_Load("battle.mp3");


		sheepTexture = new Texture("sheep.png");
		Texture tree("tree.png");
		Texture brick("brick.png");
		//Texture grass("grass.png");
		Texture grass("bg.png");
		Texture pyramid("pyramid2.png");
		Texture pot("pot.png");
		Texture pad("pad.png", 79, 79);
		Texture title("sheepo.png");
		bar = new Texture("BAR.png");

		Texture buttonA("A.png", 118, 118);
		Texture buttonB("B.png", 118, 118);
		Texture buttonX("X.png", 118, 118);
		Texture buttonY("Y.png", 118, 118);

		buttons.push_back(new StaticGraphic(&buttonA, 0, 0, 118, 118, 0.9, true));
		buttons.push_back(new StaticGraphic(&buttonB, 0, 0, 118, 118, 0.9, true));
		buttons.push_back(new StaticGraphic(&buttonX, 0, 0, 118, 118, 0.9, true));
		buttons.push_back(new StaticGraphic(&buttonY, 0, 0, 118, 118, 0.9, true));


		///////*Tree Entities*////////
		trees.push_back(new CocoaTree(&tree, S_WIDTH/2, 500, 43*2, 54));
		entities.push_back(trees[0]);
		/////////////

		////////*Cooking Pots*////////////
		pots.push_back(new Pot(&pot, &pad, 130, 300, 108, 98, 0, .75));
		pots.push_back(new Pot(&pot, &pad, S_WIDTH-130, 300, 108, 98, 1, .75));
		entities.push_back(pots[0]);
		entities.push_back(pots[1]);
		/////////////////////

		///////Boxes don't need to be isolated///
		entities.push_back(new Box(&brick, 0, 0, 20, S_HEIGHT));
		entities.push_back(new Box(&brick, 0, 0, S_WIDTH, 20));
		entities.push_back(new Box(&brick, S_WIDTH-20, 0, 20, S_HEIGHT));
		entities.push_back(new Box(&brick, 0, S_HEIGHT-20, S_WIDTH, 20));
		///////

		entities.push_back(new Box(&pyramid, 50, S_HEIGHT/2, 143, 108));
		entities.push_back(new Box(&pyramid, S_WIDTH-193, S_HEIGHT/2, 143, 108));

		entities[entities.size() -1]->GetSprite().SetFlip(true, false, true);

		//////*Graphics - No Collision, just for show*//////////
		graphics.push_back(new StaticGraphic(&grass, 0, 0, S_WIDTH, S_HEIGHT));
		graphics.push_back(new StaticGraphic(&title, 0, 0, S_WIDTH, S_HEIGHT));
		//////////

		// Let's rock now!
		hge->System_Start();

		delete fnt;
		hge->Effect_Free(attackBaa);
		hge->Effect_Free(prepare);
		hge->Effect_Free(humil);
		hge->Effect_Free(blip);
		hge->Effect_Free(welcome);
		hge->Effect_Free(battle);

		delete sheepTexture;
		delete bar;

		//Clean up objects (switch in transition level)
		for (unsigned int i = 0; i < entities.size(); ++i)
			delete (entities[i]);
		//entities.clear();
		for (unsigned int i = 0; i < graphics.size(); ++i)
			delete (graphics[i]);
		//graphics.clear();

	}

	// Clean up and shutdown
	hge->System_Shutdown();
	hge->Release();
	return 0;
}
