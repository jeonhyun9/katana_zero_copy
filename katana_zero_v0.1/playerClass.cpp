#include "stdafx.h"
#include "playerClass.h"

HRESULT playerClass::init()
{
	/*플레이어 변수 초기화*/
	//플레이어의 시작 좌표를 싱글톤에서 받아온다.
	player.x = DATA->getStartLoc().x;
	player.y = DATA->getStartLoc().y;

	player_run = IMAGEMANAGER->findImage("player_run");
	player_idle = IMAGEMANAGER->findImage("player_idle");
	player_jump = IMAGEMANAGER->findImage("player_jump");
	player_roll = IMAGEMANAGER->findImage("player_roll");

	player.width = 30;
	player.height = 70;
	player.angle = PI / 2;
	player.speed = 10.0f;
	player.jumpSpeed = 10.0f;
	player.fallSpeed = 4.0f;
	player.rollSpeed = 15.0f;
	player.isRight = true;
	player.state = P_IDLE;

	camera.height = 720;
	camera.width = 1280;

	WALLCRASH_LEFT = false;
	WALLCRASH_RIGHT = false;
	WALLCRASH_BOTTOM = false;
	WALLCRASH_TOP = false;

	ON_FLOOR = false;

	REACHED_CLEAR = false;

	JUMP = false;
	FALL = false;
	JUMP_BELOW = false;
	ROLL = false;

	jumpCnt = 0;
	jump_belowCnt = 0;
	frameCnt = 0;
	rollCnt = 0;
	frameIdx = 0;
	gravity = 0;

	return S_OK;
}

void playerClass::release()
{
	
}

void playerClass::update()
{
	//렉트 생성
	player.rc = RectMakeCenter(player.x - DATA->getBgCol().x, player.y - DATA->getBgCol().y, player.width, player.height);
	camera.rc = RectMakeCenter(player.x - DATA->getBgCol().x, player.y - DATA->getBgCol().y, camera.width, camera.height);

	//픽셀충돌 탐색점
	findleft = (player.x - player.width / 2) - player.speed * 2;
	findright = (player.x + player.width / 2) + player.speed * 2;
	findtop = (player.y - player.height / 2) ;
	findbottom = (player.y + player.height / 2) ;
	findY = player.y + player.height / 2;

	//이미지 위치
	player.renderX = (player.rc.left - player.width / 2) - 5;
	
	/*플레이어 조작*/
	//왼쪽 이동
	if (INPUT->GetKey('A') && WALLCRASH_LEFT == false && JUMP_BELOW == false)
	{
		player.state = P_RUN; //애니메이션
		player.isRight = false;
		player.x -= player.speed;
		if (camera.rc.left < 0 && DATA->getBgCol().x > 0)
		{
			DATA->minusBgX(player.speed);
		}
	}
	if (INPUT->GetKeyUp('A'))
	{
		player.state = P_IDLE;
	}
	//오른쪽 이동
	if (INPUT->GetKey('D') && WALLCRASH_RIGHT == false && JUMP_BELOW == false)
	{
		player.state = P_RUN; //애니메이션
		player.isRight = true;
		player.x += player.speed;
		if (camera.rc.right > WINSIZEX && DATA->getBgCol().x < DATA->getBgCol().width - WINSIZEX)
		{
			DATA->addBgX(player.speed);
		}

	}
	if (INPUT->GetKeyUp('D'))
	{
		player.state = P_IDLE;
	}

	//점프
	if (INPUT->GetKeyDown('W') && JUMP == false && FALL == false)
	{
		JUMP = true;
	}

	//밑점프
	if (INPUT->GetKeyDown('S'))
	{
		JUMP_BELOW = true;
	}

	//구르기
	if (INPUT->GetKeyDown('Q'))
	{
		ROLL = true;
	}

	if (ROLL == true)
	{
		rollCnt++;
		player.state = P_ROLL;
		switch (player.isRight)
		{
		case false:
			player.x -= player.rollSpeed;
			if (camera.rc.left < 0 && DATA->getBgCol().x > 0)
			{
				DATA->minusBgX(player.speed);
			}
			break;
		case true:
			player.x += player.rollSpeed;
			if (camera.rc.right > WINSIZEX && DATA->getBgCol().x < DATA->getBgCol().width - WINSIZEX)
			{
				DATA->addBgX(player.speed);
			}
			break;
		}
	}

	//구르기 로직
	if (rollCnt > 14)
	{
		rollCnt = 0;
		ROLL = false;
		player.state = P_IDLE;
	}
	
	//벽충돌중이 아니고 , 점프 중 아닐 경우 추락
	if (WALLCRASH_BOTTOM == false && JUMP == false && ON_FLOOR ==false)
	{
		FALL = true;
	}

	//점프 로직
	if (JUMP == true)
	{
		player.state = P_JUMP;
		jumpCnt++;
		gravity += 0.01f;
		player.y += -sinf(player.angle) * player.jumpSpeed + gravity;
		if (camera.rc.top < 0 && DATA->getBgCol().y > 0)
		{
			DATA->minusBgY(player.speed);
		}
	}

	//밑점프 로직
	if (JUMP_BELOW == true)
	{
		jump_belowCnt++;
		WALLCRASH_BOTTOM = false;
		FALL = true;
	}

	if (jump_belowCnt > 20)
	{
		jump_belowCnt = 0;
		JUMP_BELOW = false;

	}

	//추락 시점
	if (jumpCnt > 20)
	{
		jumpCnt = 0;
		JUMP = false;
		FALL = true;
	}

	//머리에 벽이 닿을 경우 추락
	if (WALLCRASH_TOP == true)
	{
		jumpCnt = 0;
		JUMP = false;
		FALL = true;
	}

	//추락 로직
	if (FALL == true)
	{
		if (WALLCRASH_BOTTOM == false)
		{
			player.state = P_JUMP;
			gravity += 0.5f;
			player.y += -sinf(player.angle) * player.fallSpeed + gravity;
		}
		else if (WALLCRASH_BOTTOM == true)
		{
			player.state = P_IDLE;
			gravity = 0;
			FALL = false;
		}

		if (camera.rc.bottom > WINSIZEY && DATA->getBgCol().y < DATA->getBgCol().height - WINSIZEY)
		{
			DATA->addBgY(player.speed);
		}
	}

	//점프 중 아닐 경우 바닥 로직
	if (JUMP == false && FALL == false)
	{
		this->pixelCollisionFloor();
	}

	//밑점프 중일 때 잠깐 충돌 끔
	this->pixelCollisionBottom();
	
	//픽셀 충돌 함수 호출
	this->pixelCollisionLeft();
	this->pixelCollisionRight();
	this->pixelCollisionTop();
	this->pixelCollisionClearLocation();

	//플레이어 애니메이션 함수 호출
	this->playerAnimation();
	

}

void playerClass::render()
{
	//Rectangle(getMemDC(), camera.rc);
	Rectangle(getMemDC(), player.rc);

	switch (player.state)
	{
	case P_IDLE:
		player_idle->frameRender(getMemDC(), player.renderX, (player.rc.top));
		break;
	case P_RUN:
		player_run->frameRender(getMemDC(), player.renderX, (player.rc.top + 10));
		break;
	case P_JUMP:
		if (player.isRight == true)
		{
			player_jump->frameRender(getMemDC(), player.renderX, player.rc.top);
		}
		else
		{
			player_jump->frameRender(getMemDC(), player.renderX + 10, player.rc.top);
		}
		
		break;
	case P_ROLL:
		player_roll->frameRender(getMemDC(), player.renderX, (player.rc.top));
		break;
	}

	//TextOut(getMemDC(), player.x - DATA->getBgCol().x, player.y - DATA->getBgCol().y, str2, strlen(str2));
}

void playerClass::playerAnimation()
{
	switch (player.state)
	{
	case P_IDLE:
		if (player.isRight)
		{
			frameCnt++;
			player_idle->setFrameY(0);
			if (frameCnt % 7 == 0)
			{
				frameIdx++;
				if (frameIdx > 11)
				{
					frameIdx = 0;
				}
				player_idle->setFrameX(frameIdx);
			}
		}
		else
		{
			frameCnt++;
			player_idle->setFrameY(1);
			if (frameCnt % 7 == 0)
			{
				frameIdx--;
				if (frameIdx < 0)
				{
					frameIdx = 11;
				}
				player_idle->setFrameX(frameIdx);
			}
		}
		break;
	case P_RUN:
		if (player.isRight)
		{
			frameCnt++;
			player_run->setFrameY(0);
			if (frameCnt % 5 == 0)
			{
				frameIdx++;
				if (frameIdx > 9)
				{
					frameIdx = 0;
				}
				player_run->setFrameX(frameIdx);
			}
		}
		else
		{
			frameCnt++;
			player_run->setFrameY(1);
			if (frameCnt % 5 == 0)
			{
				frameIdx--;
				if (frameIdx < 1)
				{
					frameIdx = 9;
				}
				player_run->setFrameX(frameIdx);
			}
		}
		break;
	case P_JUMP:
		if (player.isRight)
		{
			frameCnt++;
			player_jump->setFrameY(0);
			if (frameCnt % 4 == 0)
			{
				frameIdx++;
				if (frameIdx > 3)
				{
					frameIdx = 0;
				}
				player_jump->setFrameX(frameIdx);
			}
		}
		else
		{
			frameCnt++;
			player_jump->setFrameY(1);
			if (frameCnt % 4 == 0)
			{
				frameIdx--;
				if (frameIdx < 1)
				{
					frameIdx = 4;
				}
				player_jump->setFrameX(frameIdx);
			}
		}
		break;
	case P_ROLL:
		if (player.isRight)
		{
			frameCnt++;
			player_roll->setFrameY(0);
			if (frameCnt % 2 == 0)
			{
				frameIdx++;
				if (frameIdx > 6)
				{
					frameIdx = 0;
				}
				player_roll->setFrameX(frameIdx);
			}
		}
		else
		{
			frameCnt++;
			player_roll->setFrameY(1);
			if (frameCnt % 2 == 0)
			{
				frameIdx--;
				if (frameIdx < 1)
				{
					frameIdx = 7;
				}
				player_roll->setFrameX(frameIdx);
			}
		}
		break;
	}
}

void playerClass::pixelCollisionLeft()
{
	//픽셀충돌 왼쪽
	for (int i = player.y - player.height / 2; i < player.y; i++)
	{
		COLORREF color = GetPixel(DATA->getBgCol().img->getMemDC(), findleft, i);
		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);

		if (!(r == 255 && g == 0 && b == 255 || r == 0 && g == 0 && b == 0))
		{
			WALLCRASH_LEFT = true;
			sprintf(str2, "왼충돌");
			break;
		}
		else
		{
			WALLCRASH_LEFT = false;
		}
	}
}

void playerClass::pixelCollisionRight()
{
	//픽셀충돌 오른쪽
	for (int i = player.y - player.height / 2; i < player.y; i++)
	{
		COLORREF color = GetPixel(DATA->getBgCol().img->getMemDC(), findright, i);
		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);

		if (!(r == 255 && g == 0 && b == 255 || r == 0 && g == 0 && b == 0))
		{
			WALLCRASH_RIGHT = true;
			sprintf(str2, "오른충돌");
			break;
		}
		else
		{
			WALLCRASH_RIGHT = false;
		}
	}
}

void playerClass::pixelCollisionTop()
{
	//픽셀충돌 위
	for (int i = player.x - player.width / 2; i < player.x + player.width / 2; i++)
	{
		COLORREF color = GetPixel(DATA->getBgCol().img->getMemDC(), i, findtop);
		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);

		if (!(r == 255 && g == 0 && b == 255 || r == 0 && g == 0 && b == 0))
		{
			WALLCRASH_TOP = true;
			sprintf(str2, "위충돌");
			break;
		}
		else
		{
			WALLCRASH_TOP = false;
		}
	}
}

void playerClass::pixelCollisionBottom()
{
	//픽셀충돌 아래
	for (int i = player.x - player.width / 2; i < player.x + player.width / 2; i++)
	{
		COLORREF color = GetPixel(DATA->getBgCol().img->getMemDC(), i, findbottom);
		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);

		
		if (!(r == 255 && g == 0 && b == 255))
		{
			if (JUMP_BELOW == true && r == 0 && g == 0 && b == 0)break;
			WALLCRASH_BOTTOM = true;
			sprintf(str2, "아래충돌");
			break;
		}
		else
		{
			WALLCRASH_BOTTOM = false;
		}
		
	}
	
}

void playerClass::pixelCollisionFloor()
{
	/*이부분이 픽셀충돌의 핵심*/
	for (int i = findY - 15; i < findY + 15; i++)
	{
		COLORREF color = GetPixel(DATA->getBgCol().img->getMemDC(), player.x, i);
		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);

		if (!(r == 255 && g == 0 && b == 255))
		{
			float tempY = player.y;
			player.y = i - player.height / 2;
			if (player.y > tempY)
			{
				if (camera.rc.bottom > WINSIZEY && DATA->getBgCol().y < 3500 - WINSIZEY)
				{
					DATA->addBgY(player.speed);
				}
			}
			else
			{
				if (camera.rc.top < 0 && DATA->getBgCol().y > 0)
				{
					DATA->minusBgY(player.speed);
				}
			}
			ON_FLOOR = true;
			break;
		}
		else
		{
			ON_FLOOR = false;
		}
	}
}

void playerClass::pixelCollisionClearLocation()
{
	for (int i = findleft - 5; i < findright + 5; i++)
	{
		COLORREF color = GetPixel(DATA->getBgCol().img->getMemDC(), i, player.y);
		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);

		if (r == 0 && g == 0 && b == 255)
		{
			this->toNextStage();
		}
	}
}

void playerClass::toNextStage()
{
	switch (DATA->getCurrentClear())
	{
	case NONE_CLEARED:
		DATA->setClear(STAGE1_CLEARED);
		SCENEMANAGER->loadScene("스테이지2");
		this->saveClearedStage();
		break;
	}
}

void playerClass::saveClearedStage()
{
	HANDLE file;
	DWORD write;

	enum_Cleared tempStage[1];

	tempStage[0] = DATA->getCurrentClear();

	file = CreateFile("save.stage", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL, NULL);
	WriteFile(file,tempStage, sizeof(enum_Cleared) * 1, &write, NULL);
	CloseHandle(file);
}

//찌꺼기
//if (INPUT->GetKey('W') && WALLCRASH_TOP == false)
	//{
	//	player.y -= player.speed;
	//	if (camera.rc.top < 0 && DATA->getBgCol().y > 0)
	//	{
	//	
	//		DATA->minusBgY(player.speed);
	//	}
	//}
	//if (INPUT->GetKey('S') && WALLCRASH_BOTTOM == false)
	//{
	//	player.y += player.speed;
	//	if (camera.rc.bottom > WINSIZEY && DATA->getBgCol().y < 3500 - WINSIZEY )
	//	{
	//		
	//		DATA->addBgY(player.speed);
	//	}
	//}