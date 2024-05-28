﻿/* =============================================================
	INTRODUCTION TO GAME PROGRAMMING SE102
	
	SAMPLE 04 - COLLISION

	This sample illustrates how to:

		1/ Implement SweptAABB algorithm between moving objects
		2/ Implement a simple (yet effective) collision frame work, applying on Mario, Brick, Goomba & Coin

	Key functions: 
		CCollision::SweptAABB
		CCollision::SweptAABBEx
		CCollision::Scan
		CCollision::Filter
		CCollision::Process

		CGameObject::GetBoundingBox
		
================================================================ */

#include <windows.h>
#include <d3d10.h>
#include <d3dx10.h>
#include <list>

#include "debug.h"
#include "Game.h"
#include "GameObject.h"
#include "Textures.h"
#include "Animation.h"
#include "Animations.h"
#include "Mushroom.h"
#include "String"
#include "Mario.h"
#include "Box.h"
#include "Brick.h"
#include "Goomba.h"
#include "Coin.h"
#include "Platform.h"

#include "SampleKeyEventHandler.h"

#include "AssetIDs.h"

#define WINDOW_CLASS_NAME L"SampleWindow"
#define MAIN_WINDOW_TITLE L"04 - Collision"
#define WINDOW_ICON_PATH L"mario.ico"

#define BACKGROUND_COLOR D3DXCOLOR(200.0f/255, 200.0f/255, 255.0f/255, 0.0f)

#define SCREEN_WIDTH 420
#define SCREEN_HEIGHT 270


#define TEXTURES_DIR L"textures"
#define TEXTURE_PATH_MARIO TEXTURES_DIR "\\mario_transparent.png"
#define TEXTURE_PATH_MISC TEXTURES_DIR "\\misc_transparent.png"
#define TEXTURE_PATH_ENEMY TEXTURES_DIR "\\enemies_transparent.png"
#define TEXTURE_PATH_BBOX TEXTURES_DIR "\\bbox.png"

CGame *game;
CMario *mario;

list<LPGAMEOBJECT> objects;

CSampleKeyHandler * keyHandler; 

LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

void LoadAssetsMushroom()
{
	CTextures* textures = CTextures::GetInstance();
	CSprites* sprites = CSprites::GetInstance();
	CAnimations* animations = CAnimations::GetInstance();

	LPTEXTURE texMisc = textures->Get(ID_TEX_MISC);

	sprites->Add(ID_SPRITE_MUSHROOM + 1, 300, 188, 316, 204, texMisc);
	sprites->Add(ID_SPRITE_MUSHROOM + 2, 318, 188, 334, 204, texMisc);

	LPANIMATION ani = new CAnimation(300);
	ani->Add(ID_SPRITE_MUSHROOM + 1);
	ani->Add(ID_SPRITE_MUSHROOM + 2);
	animations->Add(ID_ANI_MUSHROOM, ani);
}
void LoadAssetsMario()
{
	CTextures* textures = CTextures::GetInstance();
	CSprites* sprites = CSprites::GetInstance();
	CAnimations* animations = CAnimations::GetInstance();

	LPTEXTURE texMario = textures->Get(ID_TEX_MARIO);

	// IDLE
	sprites->Add(ID_SPRITE_MARIO_BIG_IDLE_RIGHT + 1, 246,154,259,181, texMario);
	sprites->Add(ID_SPRITE_MARIO_BIG_IDLE_LEFT + 1, 186,154,199,181, texMario);

	// WALKING LEFT
	sprites->Add(ID_SPRITE_MARIO_BIG_WALKING_RIGHT + 2, 275,154,290,181, texMario);
	sprites->Add(ID_SPRITE_MARIO_BIG_WALKING_RIGHT + 3, 304,154,321,181, texMario);

	// WALKING RIGHT
	sprites->Add(ID_SPRITE_MARIO_BIG_WALKING_LEFT + 2, 155,154,170,181, texMario);
	sprites->Add(ID_SPRITE_MARIO_BIG_WALKING_LEFT + 3, 125,154,140,181, texMario);

	// RUNNING RIGHT 
	sprites->Add(ID_SPRITE_MARIO_BIG_RUNNING_RIGHT + 1, 334,154,355,181, texMario);
	sprites->Add(ID_SPRITE_MARIO_BIG_RUNNING_RIGHT + 2, 334,154,355,181, texMario);
	sprites->Add(ID_SPRITE_MARIO_BIG_RUNNING_RIGHT + 3, 392,154,413,181, texMario);

	// RUNNING LEFT
	sprites->Add(ID_SPRITE_MARIO_BIG_RUNNING_LEFT + 1, 91,154,112,181, texMario);
	sprites->Add(ID_SPRITE_MARIO_BIG_RUNNING_LEFT + 2, 65,154,86,181, texMario);
	sprites->Add(ID_SPRITE_MARIO_BIG_RUNNING_LEFT + 3, 34,154,55,181, texMario);

	// JUMP WALK RIGHT & LEFT 
	sprites->Add(ID_SPRITE_MARIO_BIG_JUMP_WALK_RIGHT + 1, 395,275,412,302, texMario);
	sprites->Add(ID_SPRITE_MARIO_BIG_JUMP_WALK_LEFT + 1, 35,275,52,302, texMario);

	// JUMP RUN RIGHT & LEFT 
	sprites->Add(ID_SPRITE_MARIO_BIG_JUMP_RUN_RIGHT + 1, 394,195,413,222, texMario);
	sprites->Add(ID_SPRITE_MARIO_BIG_JUMP_RUN_LEFT + 1, 35,195,52,222, texMario);

	// SIT RIGHT/LEFT
	sprites->Add(ID_SPRITE_MARIO_BIG_SIT_RIGHT + 1, 426,239,441,256, texMario);
	sprites->Add(ID_SPRITE_MARIO_BIG_SIT_LEFT + 1, 5,239,20,256, texMario);

	// BRACING RIGHT/LEFT
	sprites->Add(ID_SPRITE_MARIO_BIG_BRACE_RIGHT + 1, 425,154,442,181, texMario);
	sprites->Add(ID_SPRITE_MARIO_BIG_BRACE_LEFT + 1, 5,154,22,181, texMario);

	// MARIO DIE
	sprites->Add(ID_SPRITE_MARIO_DIE + 1, 215, 120, 231, 135, texMario);

	// SMALL MARIO 
	sprites->Add(ID_SPRITE_MARIO_SMALL_IDLE_RIGHT + 1, 247, 0, 259, 15, texMario);			
	sprites->Add(ID_SPRITE_MARIO_SMALL_IDLE_LEFT + 1, 187, 0, 198, 15, texMario);			

	sprites->Add(ID_SPRITE_MARIO_SMALL_WALKING_RIGHT + 2, 275, 0, 291, 15, texMario);			
	sprites->Add(ID_SPRITE_MARIO_SMALL_WALKING_RIGHT + 3, 306, 0, 320, 15, texMario); 

	sprites->Add(ID_SPRITE_MARIO_SMALL_WALKING_LEFT + 2, 155, 0, 170, 15, texMario);
	sprites->Add(ID_SPRITE_MARIO_SMALL_WALKING_LEFT + 3, 125, 0, 139, 15, texMario);

	sprites->Add(ID_SPRITE_MARIO_SMALL_RUNNING_RIGHT + 1, 275, 0, 275 + 15, 15, texMario);
	sprites->Add(ID_SPRITE_MARIO_SMALL_RUNNING_RIGHT + 2, 306, 0, 306 + 15, 15, texMario); 
	sprites->Add(ID_SPRITE_MARIO_SMALL_RUNNING_RIGHT + 3, 335, 0, 335 + 15, 15, texMario);

	sprites->Add(ID_SPRITE_MARIO_SMALL_RUNNING_LEFT + 1, 155, 0, 155 + 15, 15, texMario);
	sprites->Add(ID_SPRITE_MARIO_SMALL_RUNNING_LEFT + 2, 125, 0, 125 + 15, 15, texMario);
	sprites->Add(ID_SPRITE_MARIO_SMALL_RUNNING_LEFT + 3, 95, 0, 95 + 15, 15, texMario);

	sprites->Add(ID_SPRITE_MARIO_SMALL_BRACE_LEFT + 1, 6, 0, 6 + 13, 15, texMario);
	sprites->Add(ID_SPRITE_MARIO_SMALL_BRACE_RIGHT + 1, 426, 0, 426 + 13, 15, texMario);

	sprites->Add(ID_SPRITE_MARIO_SMALL_JUMP_WALK_LEFT + 1, 35, 80, 35 + 15, 80 + 15, texMario);		
	sprites->Add(ID_SPRITE_MARIO_SMALL_JUMP_WALK_RIGHT + 1, 395, 80, 395 + 15, 80 + 15, texMario);

	sprites->Add(ID_SPRITE_MARIO_SMALL_JUMP_RUN_LEFT + 1, 65, 40, 65 + 15, 40 + 15, texMario);
	sprites->Add(ID_SPRITE_MARIO_SMALL_JUMP_RUN_RIGHT + 1, 365, 40, 365 + 15, 40 + 15, texMario);

	LPANIMATION ani;

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_BIG_IDLE_RIGHT + 1);
	animations->Add(ID_ANI_MARIO_IDLE_RIGHT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_BIG_IDLE_LEFT + 1);
	animations->Add(ID_ANI_MARIO_IDLE_LEFT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_BIG_IDLE_RIGHT + 1);
	ani->Add(ID_SPRITE_MARIO_BIG_WALKING_RIGHT + 2);
	ani->Add(ID_SPRITE_MARIO_BIG_WALKING_RIGHT + 3);
	animations->Add(ID_ANI_MARIO_WALKING_RIGHT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_BIG_IDLE_LEFT + 1);
	ani->Add(ID_SPRITE_MARIO_BIG_WALKING_LEFT + 2);
	ani->Add(ID_SPRITE_MARIO_BIG_WALKING_LEFT + 3);
	animations->Add(ID_ANI_MARIO_WALKING_LEFT, ani);

	ani = new CAnimation(50);
	ani->Add(ID_SPRITE_MARIO_BIG_RUNNING_RIGHT + 1);
	ani->Add(ID_SPRITE_MARIO_BIG_RUNNING_RIGHT + 2);
	ani->Add(ID_SPRITE_MARIO_BIG_RUNNING_RIGHT + 3);
	animations->Add(ID_ANI_MARIO_RUNNING_RIGHT, ani);

	// Mario runs faster hence animation speed should be faster
	ani = new CAnimation(50);	
	ani->Add(ID_SPRITE_MARIO_BIG_RUNNING_LEFT + 1);
	ani->Add(ID_SPRITE_MARIO_BIG_RUNNING_LEFT + 2);
	ani->Add(ID_SPRITE_MARIO_BIG_RUNNING_LEFT + 3);
	animations->Add(ID_ANI_MARIO_RUNNING_LEFT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_BIG_JUMP_WALK_RIGHT + 1);
	animations->Add(ID_ANI_MARIO_JUMP_WALK_RIGHT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_BIG_JUMP_WALK_LEFT + 1);
	animations->Add(ID_ANI_MARIO_JUMP_WALK_LEFT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_BIG_JUMP_RUN_RIGHT + 1);
	animations->Add(ID_ANI_MARIO_JUMP_RUN_RIGHT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_BIG_JUMP_RUN_LEFT + 1);
	animations->Add(ID_ANI_MARIO_JUMP_RUN_LEFT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_BIG_SIT_RIGHT + 1);
	animations->Add(ID_ANI_MARIO_SIT_RIGHT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_BIG_SIT_LEFT + 1);
	animations->Add(ID_ANI_MARIO_SIT_LEFT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_BIG_BRACE_RIGHT + 1);
	animations->Add(ID_ANI_MARIO_BRACE_RIGHT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_BIG_BRACE_LEFT + 1);
	animations->Add(ID_ANI_MARIO_BRACE_LEFT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_DIE + 1);
	animations->Add(ID_ANI_MARIO_DIE, ani);

	//
	// SMALL MARIO 
	//
	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_SMALL_IDLE_RIGHT + 1);
	animations->Add(ID_ANI_MARIO_SMALL_IDLE_RIGHT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_SMALL_IDLE_RIGHT + 1);
	ani->Add(ID_SPRITE_MARIO_SMALL_WALKING_RIGHT + 2);
	ani->Add(ID_SPRITE_MARIO_SMALL_WALKING_RIGHT + 3);
	animations->Add(ID_ANI_MARIO_SMALL_WALKING_RIGHT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_SMALL_IDLE_LEFT + 1);
	animations->Add(ID_ANI_MARIO_SMALL_IDLE_LEFT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_SMALL_IDLE_LEFT + 1);
	ani->Add(ID_SPRITE_MARIO_SMALL_WALKING_LEFT + 2);
	ani->Add(ID_SPRITE_MARIO_SMALL_WALKING_LEFT + 3);
	animations->Add(ID_ANI_MARIO_SMALL_WALKING_LEFT, ani);

	ani = new CAnimation(50);
	ani->Add(ID_SPRITE_MARIO_SMALL_RUNNING_RIGHT + 1);
	ani->Add(ID_SPRITE_MARIO_SMALL_RUNNING_RIGHT + 2);
	ani->Add(ID_SPRITE_MARIO_SMALL_RUNNING_RIGHT + 3);
	animations->Add(ID_ANI_MARIO_SMALL_RUNNING_RIGHT, ani);

	ani = new CAnimation(50);
	ani->Add(ID_SPRITE_MARIO_SMALL_RUNNING_LEFT + 1);
	ani->Add(ID_SPRITE_MARIO_SMALL_RUNNING_LEFT + 2);
	ani->Add(ID_SPRITE_MARIO_SMALL_RUNNING_LEFT + 3);
	animations->Add(ID_ANI_MARIO_SMALL_RUNNING_LEFT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_SMALL_BRACE_LEFT + 1);
	animations->Add(ID_ANI_MARIO_SMALL_BRACE_LEFT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_SMALL_BRACE_RIGHT + 1);
	animations->Add(ID_ANI_MARIO_SMALL_BRACE_RIGHT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_SMALL_JUMP_WALK_RIGHT + 1);
	animations->Add(ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_SMALL_JUMP_WALK_LEFT + 1);
	animations->Add(ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_SMALL_JUMP_RUN_LEFT + 1);
	animations->Add(ID_ANI_MARIO_SMALL_JUMP_RUN_LEFT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_SMALL_JUMP_RUN_RIGHT + 1);
	animations->Add(ID_ANI_MARIO_SMALL_JUMP_RUN_RIGHT, ani);
}
void LoadAssetsGoomba()
{
	CTextures* textures = CTextures::GetInstance();
	CSprites* sprites = CSprites::GetInstance();
	CAnimations* animations = CAnimations::GetInstance();

	LPTEXTURE texEnemy = textures->Get(ID_TEX_ENEMY);

	sprites->Add(ID_SPRITE_GOOMBA_WALK + 1, 4, 13, 22, 30, texEnemy);  
	sprites->Add(ID_SPRITE_GOOMBA_WALK + 2, 24, 13, 42, 30, texEnemy); 

	sprites->Add(ID_SPRITE_GOOMBA_DIE + 1, 44, 19, 62, 30, texEnemy);

	LPANIMATION ani = new CAnimation(100);
	ani->Add(ID_SPRITE_GOOMBA_WALK + 1);
	ani->Add(ID_SPRITE_GOOMBA_WALK + 2);
	animations->Add(ID_ANI_GOOMBA_WALKING, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_GOOMBA_DIE + 1);
	animations->Add(ID_ANI_GOOMBA_DIE, ani);
}
void LoadAssetsBrick()
{
	CTextures* textures = CTextures::GetInstance();
	CSprites* sprites = CSprites::GetInstance();
	CAnimations* animations = CAnimations::GetInstance();

	LPTEXTURE texMisc = textures->Get(ID_TEX_MISC);
	sprites->Add(ID_SPRITE_BRICK + 1, 372, 153, 372 + 15, 153 + 15, texMisc);

	LPANIMATION ani = new CAnimation(100);
	ani->Add(ID_SPRITE_BRICK + 1);
	animations->Add(ID_ANI_BRICK, ani);
}
void LoadAssetsCoin()
{
	CTextures* textures = CTextures::GetInstance();
	CSprites* sprites = CSprites::GetInstance();
	CAnimations* animations = CAnimations::GetInstance();

	LPTEXTURE texMisc = textures->Get(ID_TEX_MISC);

	sprites->Add(ID_SPRITE_COIN + 1, 303, 99, 303 + 9, 99 + 15, texMisc);
	sprites->Add(ID_SPRITE_COIN + 2, 321, 99, 321 + 9, 99 + 15, texMisc);
	sprites->Add(ID_SPRITE_COIN + 3, 338, 99, 338 + 9, 99 + 15, texMisc);

	LPANIMATION ani = new CAnimation(300);
	ani->Add(ID_SPRITE_COIN + 1);
	ani->Add(ID_SPRITE_COIN + 2);
	ani->Add(ID_SPRITE_COIN + 3);
	animations->Add(ID_ANI_COIN, ani);
}
void LoadAssetsOther()
{
	CTextures* textures = CTextures::GetInstance();
	CSprites* sprites = CSprites::GetInstance();

	LPTEXTURE texMisc = textures->Get(ID_TEX_MISC);
	sprites->Add(ID_SPRITE_CLOUD_BEGIN, 390, 117, 390 + 15, 117 + 15, texMisc);
	sprites->Add(ID_SPRITE_CLOUD_MIDDLE, 408, 117, 408 + 15, 117 + 15, texMisc);
	sprites->Add(ID_SPRITE_CLOUD_END, 426, 117, 426 + 15, 117 + 15, texMisc);

	sprites->Add(ID_SPRITE_CLOUD_END, 426, 117, 426 + 15, 117 + 15, texMisc);
	sprites->Add(ID_SPRITE_CLOUD_END, 426, 117, 426 + 15, 117 + 15, texMisc);
}
void LoadAssetsBox() {
	CTextures* textures = CTextures::GetInstance();
	CSprites* sprites = CSprites::GetInstance();
	CAnimations* animations = CAnimations::GetInstance();

	LPTEXTURE texMisc = textures->Get(ID_TEX_MISC);
	sprites->Add(ID_SPRITE_BOX + 1, 300, 117, 315, 132, texMisc);
	sprites->Add(ID_SPRITE_BOX + 2, 318, 117, 333, 132, texMisc);
	sprites->Add(ID_SPRITE_BOX + 3, 336, 117, 351, 132, texMisc);
	sprites->Add(ID_SPRITE_BOX + 4, 354, 117, 369, 132, texMisc);

	sprites->Add(ID_SPRITE_BOX_TOUCH + 1, 372, 117, 387, 132, texMisc);

	LPANIMATION ani;
	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_BOX + 1);
	ani->Add(ID_SPRITE_BOX + 2);
	ani->Add(ID_SPRITE_BOX + 3);
	ani->Add(ID_SPRITE_BOX + 4);
	animations->Add(ID_ANI_BOX, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_BOX_TOUCH + 1);
	animations->Add(ID_ANI_BOX_ACTIVE, ani);
}

/*
	Load all game resources
	In this example: load textures, sprites, animations and mario object

	TO-DO: Improve this function by loading texture,sprite,animation,object from file
*/
void LoadResources()
{
	CTextures* textures = CTextures::GetInstance();

	textures->Add(ID_TEX_MARIO, TEXTURE_PATH_MARIO);
	textures->Add(ID_TEX_ENEMY, TEXTURE_PATH_ENEMY);
	textures->Add(ID_TEX_MISC, TEXTURE_PATH_MISC);
	textures->Add(ID_TEX_BBOX, TEXTURE_PATH_BBOX);

	LoadAssetsMario();
	LoadAssetsGoomba();
	LoadAssetsBrick();
	LoadAssetsBox();
	LoadAssetsCoin();
	LoadAssetsOther();
	LoadAssetsMushroom();
}

void ClearScene()
{ 
	list<LPGAMEOBJECT>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		delete (*it);
	}
	objects.clear();
}

#define MARIO_START_X 20.0f
#define MARIO_START_Y 10.0f

#define BRICK_X 0.0f
#define GOOMBA_X 200.0f
#define COIN_X 100.0f

#define BRICK_Y GROUND_Y + 50.0f
#define NUM_BRICKS 70

//================================ TẠO CÁC OBJECT  ==============================
void Reload()
{
	ClearScene();
	// nền gạch dưới chân
	for (int i = 0; i < NUM_BRICKS; i++)
	{
		CBrick* b = new CBrick(i * BRICK_WIDTH * 1.0f, BRICK_Y);
		objects.push_back(b);
		CBrick* b2 = new CBrick(i * BRICK_WIDTH * 1.0f, BRICK_Y + BRICK_WIDTH);
		objects.push_back(b2);
	}

	// đoạn nhô ra khi mario rớt
	for (int i = 1; i < 3; i++)
	{
		CBrick* b = new CBrick(i * BRICK_WIDTH * 1.0f, BRICK_Y - 50.0f);
		objects.push_back(b);
	}

	// gạch trên tầng mây
	for (int i = 7; i < 10; i++)
	{
		CBrick* b = new CBrick(i * BRICK_WIDTH, BRICK_Y - 120.0f);
		objects.push_back(b);

		CBrick* b2 = new CBrick((i+2) * BRICK_WIDTH, BRICK_Y - 170.0f);
		objects.push_back(b2);

		CBrick* b3 = new CBrick((i + 7) * BRICK_WIDTH, BRICK_Y - 120.0f);
		objects.push_back(b3);
	}

	// vị trí các hộp ? chứa coin
	for (int i = 10; i < 20; i++)
	{
		CBox* box = new CBox(BRICK_WIDTH * i, BRICK_Y - 120.0f, (i-5)*2, 1);
		objects.push_back(box);
		i += 6;
	}
	// cột thẳng sát lề trái
	for (int i = 0; i < 10; i++)
	{
		CBrick* b = new CBrick(0, BRICK_Y - i * BRICK_WIDTH);
		objects.push_back(b);
	}

	// Vertical column 2
	for (int i = 1; i < 3; i++)
	{
		CBrick* b = new CBrick(BRICK_X + 400.0f, BRICK_Y - i * BRICK_WIDTH);
		objects.push_back(b);
	}

	// Vertical column 3
	for (int i = 1; i < 4; i++)
	{
		CBrick* b = new CBrick(BRICK_X + 400.0f, BRICK_Y - i * BRICK_WIDTH);
		objects.push_back(b);
	}

	// Vertical column 4
	for (int i = 1; i < 5; i++)
	{
		CBrick* b = new CBrick(BRICK_X + 500.0f, BRICK_Y - i * BRICK_WIDTH);
		objects.push_back(b);
	}

	// Second cloud platform 
	CPlatform* p = new CPlatform(90.0f, GROUND_Y - 20.0f, 16, 15, 16, ID_SPRITE_CLOUD_BEGIN, ID_SPRITE_CLOUD_MIDDLE, ID_SPRITE_CLOUD_END);
	objects.push_back(p);

	mario = new CMario(MARIO_START_X, MARIO_START_Y);
	objects.push_back(mario);

	for (int j = 0; j < 4; j++)
	{
		CGoomba* goomba = new CGoomba(GOOMBA_X + j * 60, GROUND_Y - 120.0f);
		objects.push_back(goomba);
	}
	//=========== MUSHROOM ===========

	CBox* mushroom = new CBox(BRICK_WIDTH *13, GROUND_Y - 120.0f, true);
	objects.push_back(mushroom);

	// COINS 
	for (int i = 0; i < 10; i++)
	{
		CCoin* c = new CCoin(COIN_X + i * (COIN_WIDTH * 2), GROUND_Y - 96.0f);
		objects.push_back(c);
	}
}

bool IsGameObjectDeleted(const LPGAMEOBJECT& o) { return o == NULL; }

void PurgeDeletedObjects()
{
	list<LPGAMEOBJECT>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		LPGAMEOBJECT o = *it;
		if (o->IsDeleted())
		{
			delete o;
			*it = NULL;
		}
	}
	objects.erase(
		std::remove_if(objects.begin(), objects.end(), IsGameObjectDeleted),
		objects.end());
}

/*
	Update world status for this frame
	dt: time period between beginning of last frame and beginning of this frame
*/
void Update(DWORD dt)
{
	vector<LPGAMEOBJECT> coObjects; //danh sách các đối tượng phụ của obj
	list<LPGAMEOBJECT>::iterator i; //được sử dụng để duyệt qua danh sách các đối tượng
	for (i = objects.begin(); i != objects.end(); ++i)
	{
		coObjects.push_back(*i);
	}

	for (i = objects.begin(); i != objects.end(); ++i)
	{
		(*i)->Update(dt,&coObjects);
	}

	PurgeDeletedObjects(); //loại bỏ các đối tượng đã bị xóa

	// Update camera to follow mario
	float cx, cy;
	mario->GetPosition(cx, cy);

	//
	cx -= SCREEN_WIDTH / 2;
	cy = 0;

	if (cx < 0) cx = 0;

	CGame::GetInstance()->SetCamPos(cx, cy);
}

/*
	Render a frame 
*/
void Render()
{
	CGame* g = CGame::GetInstance();

	ID3D10Device* pD3DDevice = g->GetDirect3DDevice();
	IDXGISwapChain* pSwapChain = g->GetSwapChain();
	ID3D10RenderTargetView* pRenderTargetView = g->GetRenderTargetView();
	ID3DX10Sprite* spriteHandler = g->GetSpriteHandler();

	pD3DDevice->ClearRenderTargetView(pRenderTargetView, BACKGROUND_COLOR);

	spriteHandler->Begin(D3DX10_SPRITE_SORT_TEXTURE);

	FLOAT NewBlendFactor[4] = { 0,0,0,0 };
	pD3DDevice->OMSetBlendState(g->GetAlphaBlending(), NewBlendFactor, 0xffffffff);

	list<LPGAMEOBJECT>::iterator i;
	for (i = objects.begin(); i != objects.end(); ++i)
	{
		(*i)->Render();
	}

	spriteHandler->End();
	pSwapChain->Present(0, 0);
}

HWND CreateGameWindow(HINSTANCE hInstance, int nCmdShow, int ScreenWidth, int ScreenHeight)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;

	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = (HICON)LoadImage(hInstance, WINDOW_ICON_PATH, IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = WINDOW_CLASS_NAME;
	wc.hIconSm = NULL;

	RegisterClassEx(&wc);

	HWND hWnd =
		CreateWindow(
			WINDOW_CLASS_NAME,
			MAIN_WINDOW_TITLE,
			WS_OVERLAPPEDWINDOW, // WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			ScreenWidth,
			ScreenHeight,
			NULL,
			NULL,
			hInstance,
			NULL);

	if (!hWnd) 
	{
		OutputDebugString(L"[ERROR] CreateWindow failed");
		DWORD ErrCode = GetLastError();
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return hWnd;
}

int Run()
{
	MSG msg;
	int done = 0;
	//GetTickCount64(): Hàm này trả về số lượng miligiây đã trôi qua từ khi hệ thống bắt đầu
	ULONGLONG frameStart = GetTickCount64();
	//tickPerFrame (thời gian mỗi frame cần phải hiển thị)
	DWORD tickPerFrame = 1000 / MAX_FRAME_RATE;

	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) //kiểm tra sự kiện, nút nhấn
		{
			if (msg.message == WM_QUIT) done = 1;

			TranslateMessage(&msg); //chuyển đổi các thông điệp từ hệ thống về dạng phù hợp để xử lý bàn phím.
			DispatchMessage(&msg); //gửi thông điệp đến cửa sổ điều khiển (window procedure) của cửa sổ được xác định bởi msg.hwnd.
		}

		ULONGLONG now = GetTickCount64();

		DWORD dt = (DWORD)(now - frameStart);

		if (dt >= tickPerFrame)
		{
			frameStart = now;

			game->ProcessKeyboard();			 //xử lý các sự kiện từ bàn phím.
			Update(dt);
			Render();
		}
		else
			Sleep(tickPerFrame - dt);	
	}

	return 1;
}

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow
) {
	HWND hWnd = CreateGameWindow(hInstance, nCmdShow, SCREEN_WIDTH, SCREEN_HEIGHT);

	SetDebugWindow(hWnd);

	game = CGame::GetInstance();
	game->Init(hWnd, hInstance);
	game->setListObject(objects);
	keyHandler = new CSampleKeyHandler();
	game->InitKeyboard(keyHandler);

	LoadResources();
	Reload();

	SetWindowPos(hWnd, 0, 0, 0, SCREEN_WIDTH*2, SCREEN_HEIGHT*2, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);

	Run();

	return 0;
}