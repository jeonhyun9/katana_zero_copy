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
	player_fall = IMAGEMANAGER->findImage("player_fall");
	player_roll = IMAGEMANAGER->findImage("player_roll");
	player_attack = IMAGEMANAGER->findImage("player_attack");
	player_slash_basic = IMAGEMANAGER->findImage("player_slash_basic");
	player_hurtfly = IMAGEMANAGER->findImage("player_hurtfly");
	player_hurtground = IMAGEMANAGER->findImage("player_hurtground");
	player_grab = IMAGEMANAGER->findImage("player_grab");
	player_flip = IMAGEMANAGER->findImage("player_flip");
	player_doorbreak = IMAGEMANAGER->findImage("player_doorbreak");
	player_idle_to_run = IMAGEMANAGER->findImage("player_idle_to_run");
	player_run_to_idle = IMAGEMANAGER->findImage("player_run_to_idle");

	player.width = 30;
	player.height = 70;
	player.angle = PI / 2;
	player.speed = 6.5f;
	player.jumpSpeed = 8.0f;
	player.flipSpeed = 8.0f;
	player.fallSpeed = 1.0f;
	player.attSpeed = 4.0f;
	player.rollSpeed = 10.0f;
	player.isRight = true;
	player.touchedTop_Black = false;
	player.doorTouchedLeft = false;
	player.doorTouchedRight = false;
	player.isPick = false;
	player.isBatteryRemain = true;
	
	player.isReflected = false;
	player.regen = false;
	player.WALLGRAB = false;
	player.state = P_IDLE;

	player.speed_old = player.speed;
	player.rollSpeed_old = player.rollSpeed;
	player.attSpeed_old = player.attSpeed;
	player.fallSpeed_old = player.fallSpeed;
	player.jumpSpeed_old = player.jumpSpeed;
	player.flipSpeed_old = player.flipSpeed;

	player.att.height = 58;
	player.att.width = 58;

	camera.height = 720;
	camera.width = 1000;

	WALLCRASH_LEFT = false;
	WALLCRASH_RIGHT = false;
	WALLCRASH_BOTTOM = false;
	WALLCRASH_TOP = false;
	player.wallGrabLeft = false;
	player.wallGrabRight = false;
	player.FLIP = false;

	ON_FLOOR = false;
	HURT = false;
	DEAD = false;
	SLOW = false;
	player.clear = false;
	SLOW_Ratio = 4.0f;
	oldFrameTum = 0;

	REACHED_CLEAR = false;

	JUMP = false;
	FALL = false;
	JUMP_BELOW = false;
	ROLL = false;
	ATTACK = false;

	jumpCnt = frameTum = 0;
	jump_belowCnt = 0;
	hurtCnt = 0;
	frameCnt = 0;
	rollCnt = 0;
	frameIdx = 0;
	gravity = 0;

	ShowCursor(false);
	return S_OK;
}

void playerClass::release()
{
}

void playerClass::update()
{
	DATA->setSlow(SLOW);
	DATA->setSlowRatio(SLOW_Ratio);

	if (DATA->getStop() == true)
	{

	}
	else
	{
		frameCnt++;
		//치트
		if (INPUT->GetKeyDown('1'))
		{
			DEAD = false;
			HURT = false;
			DATA->setDead(false);
			player.state = P_IDLE;
		}

		if (DEAD == true && HURT == false)
		{
			player.state = P_HURTGROUND;
		}


		if (INPUT->GetKey(VK_SHIFT) && DEAD == false && HURT == false)
		{
			if (player.isBatteryRemain)
			{
				SLOW = true;
			}
			else
			{
				SLOW = false;
			}
			
		}
		else
		{
			SLOW = false;
		}

		if (SLOW == true)
		{
			SLOW_Ratio = 20.0f;
			player.speed = player.speed_old / SLOW_Ratio;
			player.attSpeed = player.attSpeed_old / SLOW_Ratio;
			player.jumpSpeed = player.jumpSpeed_old / SLOW_Ratio;
			player.fallSpeed = player.fallSpeed_old / SLOW_Ratio;
			player.rollSpeed = player.rollSpeed_old / SLOW_Ratio;
			player.flipSpeed = player.flipSpeed_old / SLOW_Ratio;
		}
		else
		{
			SLOW_Ratio = 1.0f;
			player.speed = player.speed_old;
			player.attSpeed = player.attSpeed_old;
			player.jumpSpeed = player.jumpSpeed_old;
			player.fallSpeed = player.fallSpeed_old;
			player.rollSpeed = player.rollSpeed_old;
			player.flipSpeed = player.flipSpeed_old;
		}

		//렉트 생성
		player.rc = RectMakeCenter(player.x - DATA->getBgCol().x, player.y - DATA->getBgCol().y, player.width, player.height);
		camera.rc = RectMakeCenter(player.x - DATA->getBgCol().x, player.y - DATA->getBgCol().y, camera.width, camera.height);

		if (player.isRight)
		{
			player.att.rc = RectMakeCenter((player.x + player.att.width / 2) - DATA->getBgCol().x, player.y - DATA->getBgCol().y, player.att.width, player.att.height);
		}
		else
		{
			player.att.rc = RectMakeCenter((player.x - player.att.width / 2) - DATA->getBgCol().x, player.y - DATA->getBgCol().y, player.att.width, player.att.height);
		}

		player.centerX = player.rc.left + player.width / 2;
		player.centerY = player.rc.top + player.height / 2;


		//픽셀충돌 탐색점
		findleft = (player.x - player.width / 2) - player.speed * 2;
		findright = (player.x + player.width / 2) + player.speed * 2;
		findtop = (player.y - player.height / 2);
		if (FALL == true)
		{
			findbottom = (player.y + player.height / 2) + 25;
			findY = player.y + player.height / 2 + 25;
		}
		else
		{
			findbottom = (player.y + player.height / 2);
			findY = player.y + player.height / 2;
		}
		//이미지 위치
		player.renderX = (player.rc.left - player.width / 2) - 5;

		//마우스와 플레이어 공격 각도 구하기
		player.toMouseAngle = getAngle(player.x - DATA->getBgCol().x, player.y - DATA->getBgCol().y, _ptMouse.x, _ptMouse.y);


		/*플레이어 조작*/
		//왼쪽 벽잡기
		if (INPUT->GetKey('A') && player.wallGrabLeft == true && ATTACK == false)
		{
			player.WALLGRAB = true;
		}

		if (player.WALLGRAB == true)
		{
			gravity = 1.0f;
			jumpCnt = 0;
			JUMP = false;
			FALL = true;
		}

		if (INPUT->GetKeyDown('D') && player.WALLGRAB==true)
		{
			player.WALLGRAB = false;
			player.isRight = true;
			player.FLIP = true;
			FALL = false;
			JUMP = true;
			jumpCnt = 0;
			gravity = 1.0f;
		}

		//오른쪽 벽잡기
		if (INPUT->GetKey('D') && player.wallGrabRight == true && ATTACK == false)
		{
			player.WALLGRAB = true;
		}

		if (INPUT->GetKeyDown('A') && player.WALLGRAB == true)
		{
			player.WALLGRAB = false;
			player.isRight = false;
			player.FLIP = true;
			FALL = false;
			JUMP = true;
			jumpCnt = 0;
			gravity = 1.0f;
		}
		
		//왼쪽 이동
		if (INPUT->GetKey('A') && WALLCRASH_LEFT == false && JUMP_BELOW == false && ATTACK == false 
			&& ROLL == false && DEAD == false && HURT == false && player.FLIP == false && 
			player.WALLGRAB == false && player.state != P_DOORBREAK && player.state != P_HURTFLY)
		{
			player.state = P_RUN; //애니메이션
			player.isRight = false;
			if (player.doorTouchedLeft == false)
			{
				player.x -= player.speed;
				if (camera.rc.left < 0 && DATA->getBgCol().x > 0)
				{
					DATA->minusBgX(player.speed);
				}
			}
		}
		if (INPUT->GetKeyUp('A'))
		{
			player.WALLGRAB = false;
			if (player.state != P_DOORBREAK)
			{
				player.state = P_IDLE;
			}

		}
		//오른쪽 이동
		if (INPUT->GetKey('D') && WALLCRASH_RIGHT == false && JUMP_BELOW == false 
			&& ATTACK == false && ROLL == false && DEAD == false && HURT == false && player.FLIP == false 
			&& player.WALLGRAB == false && player.state != P_DOORBREAK && player.state != P_HURTFLY)
		{
			player.state = P_RUN; //애니메이션
			if (player.doorTouchedRight == false)
			{
				player.isRight = true;
				player.x += player.speed;
				if (camera.rc.right > WINSIZEX&& DATA->getBgCol().x < DATA->getBgCol().width - WINSIZEX)
				{
					DATA->addBgX(player.speed);
				}
			}
		}
		if (INPUT->GetKeyUp('D'))
		{
			player.WALLGRAB = false;
			if (player.state != P_DOORBREAK)
			{
				player.state = P_IDLE;
			}

		}

		//공격
		if (INPUT->GetKeyDown(VK_LBUTTON) && ATTACK == false && DEAD == false && HURT == false)
		{
			rollCnt = 0;
			ROLL = false;
			frameCnt = 0;
			player.y -= 1.0f;
			player.isAttack = true;
			if (camera.rc.top <= 0 && DATA->getBgCol().y >= 0)
			{
				DATA->minusBgY(1);
			}
			gravity = 1.0f;
			player.attAngle = player.toMouseAngle;
			if (player.x - DATA->getBgCol().x < _ptMouse.x)
			{
				player.isRight = true;
				frameIdx = 0;
				if (SLOW)frameIdx = 1;
			}
			else
			{
				player.isRight = false;
				frameIdx = 7;
				if (SLOW)frameIdx = 6;
			}
			ATTACK = true;
		}

		//공격 로직
		if (ATTACK == true && DEAD == false && HURT == false)
		{
			//점프 끄기
			jumpCnt = 0;
			JUMP = false;

			player.state = P_ATTACK;
			if (WALLCRASH_TOP == false && WALLCRASH_BOTTOM == false)
			{
				player.y += -sinf(player.attAngle) * (player.attSpeed * 2);
				if (camera.rc.top <= 0 && DATA->getBgCol().y >= 0)
				{
					DATA->addBgY(-sinf(player.attAngle) * player.attSpeed * 2);
				}
				if (camera.rc.bottom >= WINSIZEY && DATA->getBgCol().y <= DATA->getBgCol().height - WINSIZEY)
				{
					DATA->addBgY(-sinf(player.attAngle) * player.attSpeed * 2);
				}
			}

			switch (player.isRight)
			{
			case false:
				if (WALLCRASH_LEFT == false)
				{
					player.x += cosf(player.attAngle) * (player.attSpeed * 2);
					if (camera.rc.left < 0 && DATA->getBgCol().x > 0)
					{
						DATA->addBgX(cosf(player.attAngle) * (player.attSpeed * 2));
					}
				}
				break;
			case true:
				if (WALLCRASH_RIGHT == false)
				{
					player.x += cosf(player.attAngle) * (player.attSpeed * 2);
					if (camera.rc.right > WINSIZEX && DATA->getBgCol().x < DATA->getBgCol().width - WINSIZEX)
					{
						DATA->addBgX(cosf(player.attAngle) * (player.attSpeed * 2));
					}
				}
				break;
			}
		}

		//점프
		if (INPUT->GetKeyDown('W') && JUMP == false && FALL == false && DEAD == false && HURT == false)
		{
			JUMP = true;
		}

		//밑점프
		if (INPUT->GetKeyDown('S') && STAND_ON_BLACK == true && DEAD == false && HURT == false)
		{
			JUMP_BELOW = true;
			player.WALLGRAB = false;
		}

		//구르기
		if (INPUT->GetKeyDown('Q') && DEAD == false && HURT == false && ATTACK == false)
		{
			ROLL = true;
		}

		if (ROLL == true)
		{
			rollCnt++;
			player.state = P_ROLL;
			this->rollOnFloor();
			switch (player.isRight)
			{
			case false:
				if (WALLCRASH_LEFT == false)
				{
					player.x -= player.rollSpeed;
				}
				if (camera.rc.left < 0 && DATA->getBgCol().x > 0)
				{
					DATA->minusBgX(player.rollSpeed);
				}
				break;
			case true:
				if (WALLCRASH_RIGHT == false)
				{
					player.x += player.rollSpeed;
				}
				if (camera.rc.right > WINSIZEX && DATA->getBgCol().x < DATA->getBgCol().width - WINSIZEX)
				{
					DATA->addBgX(player.rollSpeed);
				}
				break;
			}
		}

		//구르기 로직
		if (rollCnt > 14 * SLOW_Ratio)
		{
			rollCnt = 0;
			ROLL = false;
			player.state = P_IDLE;
		}

		//추락 예외처리
		if (WALLCRASH_BOTTOM == false && JUMP == false && ON_FLOOR == false && ATTACK == false && DEAD == false 
			&& player.wallGrabLeft == false && player.wallGrabRight ==false && player.WALLGRAB == false && player.state != P_HURTFLY)
		{
			FALL = true;
		}

		//점프 로직
		if (JUMP == true && HURT == false)
		{
			jumpCnt++;
			gravity += 0.3f;
			//벽점프 로직
			if (player.FLIP == true)
			{
				player.state = P_FLIP;
				if (player.isRight)
				{
					if (WALLCRASH_RIGHT == false)player.x += player.flipSpeed;
					else
					{
						player.FLIP = false;
						if (player.wallGrabRight == true)
						{
							player.WALLGRAB = true;
						}
					}
				}
				else
				{
					if (WALLCRASH_LEFT == false)player.x -= player.flipSpeed;
					else
					{
						player.FLIP = false;
						if (player.wallGrabLeft == true)
						{
							player.WALLGRAB = true;
						}
					}
				}
			}
			else
			{
				player.state = P_JUMP;
			}
			player.y += -sinf(player.angle) * player.jumpSpeed + gravity;
			if (camera.rc.top <= 0 && DATA->getBgCol().y >= 0)
			{
				DATA->addBgY(-sinf(player.angle) * player.jumpSpeed + gravity);
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
			if (WALLCRASH_BOTTOM == true)
			{
				jumpCnt = 0;
				JUMP = false;
				player.state = P_IDLE;
			}
			else
			{
				player.state = P_FALL;
			}
		}

		//머리에 벽이 닿을 경우 추락
		if (WALLCRASH_TOP == true)
		{
			JUMP = false;
			FALL = true;
		}

		//추락 로직
		if (FALL == true && ATTACK == false && HURT == false && player.state != P_HURTFLY)
		{
			if (WALLCRASH_BOTTOM == false)
			{
				if (player.WALLGRAB == true)
				{
					player.state = P_GRAB;
					gravity += 0.3f;
				}
				else 
				{
					player.state = P_FALL;
					gravity += 0.3f;
				}
				
				
				player.y += -sinf(player.angle) * player.fallSpeed + gravity;
				
			}
			else
			{
				player.state = P_IDLE;
				gravity = 0;
				FALL = false;
				player.WALLGRAB = false;
				//this->pixelCollisionFloor();
			}
			if (camera.rc.bottom + (-sinf(player.angle) * player.fallSpeed + gravity) >= WINSIZEY && DATA->getBgCol().y < DATA->getBgCol().height - WINSIZEY)
			{
				DATA->addBgY(-sinf(player.angle) * player.fallSpeed + gravity);
			}
		}

		//점프 펄스면 플립도 펄스
		if (JUMP == false)
		{
			player.FLIP = false;
		}
		
		//피격
		if (HURT == true)
		{
			DEAD = true;
			FALL = false;
			JUMP = false;
			player.FLIP = false;
			player.state = P_HURTFLY;
			DATA->setDead(true);
			if (WALLCRASH_BOTTOM == false)
			{

				gravity += 0.3f;
				player.y += -sinf(player.angle) * player.jumpSpeed + gravity;
				if (player.isRight)
				{
					if (WALLCRASH_LEFT == false)player.x -= player.fallSpeed;
				}
				else
				{
					if (WALLCRASH_RIGHT == false)player.x += player.fallSpeed;
				}
			}
			else
			{
				//this->pixelCollisionFloor();
				gravity = 0;
				HURT = false;
				player.isAttack = false;
			}

		}

		//점프 중 아닐 경우 바닥 로직
		if (JUMP == false && FALL == false && ATTACK == false && HURT == false && ROLL == false)
		{
			this->pixelCollisionFloor();
		}

		this->pixelCollisionBottom();

		//픽셀 충돌 함수 호출
		this->pixelCollisionLeft();
		this->pixelCollisionRight();
		this->pixelCollisionTop();
		this->pixelCollisionClearLocation();

		//플레이어 애니메이션 함수 호출
		this->playerAnimation();

		//카메라 함수 호출
		if (player.state == P_IDLE || player.state == P_RUN || player.state == P_ATTACK)
		{
			this->cameraReset();
		}
		
	}
}

void playerClass::render()
{
	//Rectangle(getMemDC(), camera.rc);
	//Rectangle(getMemDC(), player.rc);

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
	case P_FALL:
		if (player.isRight == true)
		{
			IMAGEMANAGER->findImage("player_fall_shadow")->alphaFrameRender(getMemDC(), player.renderX - 19, player.rc.top - 24, frameIdx - 1, 0, 20);
			IMAGEMANAGER->findImage("player_fall_shadow")->alphaFrameRender(getMemDC(), player.renderX - 19, player.rc.top - 20, frameIdx - 1, 0, 40);
			IMAGEMANAGER->findImage("player_fall_shadow")->alphaFrameRender(getMemDC(), player.renderX - 19, player.rc.top - 16, frameIdx - 1, 0, 60);
			player_fall->frameRender(getMemDC(), player.renderX -15, player.rc.top -12);
		}
		else
		{
			IMAGEMANAGER->findImage("player_fall_shadow")->alphaFrameRender(getMemDC(), player.renderX - 1, player.rc.top -24 , frameIdx - 1, 1, 20);
			IMAGEMANAGER->findImage("player_fall_shadow")->alphaFrameRender(getMemDC(), player.renderX - 1, player.rc.top - 20, frameIdx - 1, 1, 40);
			IMAGEMANAGER->findImage("player_fall_shadow")->alphaFrameRender(getMemDC(), player.renderX - 1, player.rc.top - 16, frameIdx - 1, 1, 60);
			player_fall->frameRender(getMemDC(), player.renderX - 5, player.rc.top - 12);
		}
		break;
	case P_ROLL:
		if (player.isRight)
		{
			IMAGEMANAGER->findImage("player_roll_shadow")->alphaFrameRender(getMemDC(), player.renderX - 25, (player.rc.top) + 10, player_roll->getFrameX() - 4, 0, 60);
			IMAGEMANAGER->findImage("player_roll_shadow")->alphaFrameRender(getMemDC(), player.renderX - 20, (player.rc.top) + 10, player_roll->getFrameX() - 3, 0, 80);
			IMAGEMANAGER->findImage("player_roll_shadow")->alphaFrameRender(getMemDC(), player.renderX - 15, (player.rc.top) + 10, player_roll->getFrameX() - 2, 0, 100);
			IMAGEMANAGER->findImage("player_roll_shadow")->alphaFrameRender(getMemDC(), player.renderX - 10, (player.rc.top) + 10, player_roll->getFrameX() - 1, 0, 120);
		}
		else
		{
			IMAGEMANAGER->findImage("player_roll_shadow")->alphaFrameRender(getMemDC(), player.renderX + 25, (player.rc.top) + 10, player_roll->getFrameX() - 4, 1, 60);
			IMAGEMANAGER->findImage("player_roll_shadow")->alphaFrameRender(getMemDC(), player.renderX + 20, (player.rc.top) + 10, player_roll->getFrameX() - 3, 1, 80);
			IMAGEMANAGER->findImage("player_roll_shadow")->alphaFrameRender(getMemDC(), player.renderX + 15, (player.rc.top) + 10, player_roll->getFrameX() - 2, 1, 100);
			IMAGEMANAGER->findImage("player_roll_shadow")->alphaFrameRender(getMemDC(), player.renderX + 10, (player.rc.top) + 10, player_roll->getFrameX() - 1, 1, 120);
		}
		
		player_roll->frameRender(getMemDC(), player.renderX, (player.rc.top) + 10);
		break;
	case P_ATTACK:
		if (player.isRight == true)
		{
			//IMAGEMANAGER->findImage("player_attack_shadow")->rotateFrameRender(getMemDC(), player.renderX + 18, (player.rc.top) + 30, player_attack->getFrameX()-1, 0, player.attAngle);
			//IMAGEMANAGER->findImage("player_attack_shadow")->rotateFrameRender(getMemDC(), player.renderX + 22, (player.rc.top) + 30, player_attack->getFrameX()-1, 0, player.attAngle);
			IMAGEMANAGER->findImage("player_attack_shadow")->alphaFrameRender(getMemDC(), player.rc.left - 68, player.rc.top - 10, player_attack->getFrameX()-5, 0, 40);
			IMAGEMANAGER->findImage("player_attack_shadow")->alphaFrameRender(getMemDC(), player.rc.left - 66, player.rc.top - 10, player_attack->getFrameX()-4, 0, 60);
			IMAGEMANAGER->findImage("player_attack_shadow")->alphaFrameRender(getMemDC(), player.rc.left - 64, player.rc.top - 10, player_attack->getFrameX()-3, 0, 80);
			IMAGEMANAGER->findImage("player_attack_shadow")->alphaFrameRender(getMemDC(), player.rc.left - 62, player.rc.top - 10, player_attack->getFrameX()-2, 0, 100);
			IMAGEMANAGER->findImage("player_attack_shadow")->alphaFrameRender(getMemDC(), player.rc.left - 60, player.rc.top - 10, player_attack->getFrameX()-1, 0, 120);
			player_attack->rotateFrameRender(getMemDC(), player.renderX + 30, (player.rc.top) + 30, player_attack->getFrameX(),0, player.attAngle);
			player_slash_basic->rotateFrameRender(getMemDC(), player.renderX + 30, (player.rc.top) + 30, frameIdx, 0, player.attAngle);
		}
		else
		{
			IMAGEMANAGER->findImage("player_attack_shadow")->alphaFrameRender(getMemDC(), player.rc.left - 22, player.rc.top - 10, player_attack->getFrameX() - 5, 1,40);
			IMAGEMANAGER->findImage("player_attack_shadow")->alphaFrameRender(getMemDC(), player.rc.left - 24, player.rc.top - 10, player_attack->getFrameX() - 4, 1,60);
			IMAGEMANAGER->findImage("player_attack_shadow")->alphaFrameRender(getMemDC(), player.rc.left - 26, player.rc.top - 10, player_attack->getFrameX() - 3, 1,80);
			IMAGEMANAGER->findImage("player_attack_shadow")->alphaFrameRender(getMemDC(), player.rc.left - 28, player.rc.top - 10, player_attack->getFrameX() - 2, 1,100);
			IMAGEMANAGER->findImage("player_attack_shadow")->alphaFrameRender(getMemDC(), player.rc.left - 30, player.rc.top - 10, player_attack->getFrameX() - 1, 1,120);
			player_attack->rotateFrameRender(getMemDC(), player.renderX + 30, (player.rc.top) + 30, player_attack->getFrameX(),1, player.attAngle - PI);
			player_slash_basic->rotateFrameRender(getMemDC(), player.renderX + 30, (player.rc.top) + 30, frameIdx, 1, player.attAngle - PI);
		}
		break;
	case P_HURTFLY:
		player_hurtfly->frameRender(getMemDC(), player.renderX, (player.rc.top) + 10);
		break;
	case P_HURTGROUND:
		player_hurtground->frameRender(getMemDC(), player.renderX, (player.rc.top) + 25);
		break;
	case P_GRAB:
		if (player.isRight == true)
		{
			player_grab->frameRender(getMemDC(), player.renderX + 15, (player.rc.top) - 30);
		}
		else
		{
			player_grab->frameRender(getMemDC(), player.renderX - 15, (player.rc.top) - 30);
		}
		break;
	case P_FLIP:
		if (player.isRight)
		{
			IMAGEMANAGER->findImage("player_flip_shadow")->alphaFrameRender(getMemDC(), player.renderX - 45, (player.rc.top) + 18, player_flip->getFrameX()-3, 0, 50);
			IMAGEMANAGER->findImage("player_flip_shadow")->alphaFrameRender(getMemDC(), player.renderX - 35, (player.rc.top) + 8, player_flip->getFrameX()-2, 0, 100);
			IMAGEMANAGER->findImage("player_flip_shadow")->alphaFrameRender(getMemDC(), player.renderX - 25, (player.rc.top) - 8, player_flip->getFrameX()-1, 0, 150);
		}
		else
		{
			IMAGEMANAGER->findImage("player_flip_shadow")->alphaFrameRender(getMemDC(), player.renderX + 30, (player.rc.top) + 18, player_flip->getFrameX() - 3, 1, 50);
			IMAGEMANAGER->findImage("player_flip_shadow")->alphaFrameRender(getMemDC(), player.renderX + 20, (player.rc.top) + 8, player_flip->getFrameX() - 2, 1, 100);
			IMAGEMANAGER->findImage("player_flip_shadow")->alphaFrameRender(getMemDC(), player.renderX + 10, (player.rc.top) - 8, player_flip->getFrameX() - 1, 1, 150);
		}
		player_flip->frameRender(getMemDC(), player.renderX - 15, (player.rc.top) - 18);
		break;
	case P_DOORBREAK:
		player_doorbreak->frameRender(getMemDC(), player.renderX, player.rc.top - 18);
		break;
	//case P_RUNTOIDLE:
	//	if(player.isRight) player_run_to_idle->frameRender(getMemDC(), player.renderX +, player.rc.top);
	//	else player_run_to_idle->frameRender(getMemDC(), player.renderX -3, player.rc.top);
	//	break;
	//case P_IDLETORUN:
	//	player_idle_to_run->frameRender(getMemDC(), player.renderX, player.rc.top+ 6);
	//	break;
	}

	//반사 이펙트
	

	//char str2[256];
	////글자배경모드
	//SetBkMode(getMemDC(), TRANSPARENT);
	////글자색상
	//SetTextColor(getMemDC(), RGB(255, 0, 255));
	////마우스 좌표
	//sprintf(str2, "PLAYER : %f , %f", player.x - DATA->getBgCol().x, player.y - DATA->getBgCol().y);
	//TextOut(getMemDC(), 0, 60, str2, strlen(str2));
	//
	//sprintf(str2, "pickedItemIdx: %d", player.pickedItemIdx);
	//TextOut(getMemDC(), 0, 80, str2, strlen(str2));
	//
	////Rectangle(getMemDC(), player.att.rc);
	////TextOut(getMemDC(), player.x - DATA->getBgCol().x, player.y - DATA->getBgCol().y, str2, strlen(str2));
	//
	////커서 바꾸기
	IMAGEMANAGER->findImage("cursor")->render(getMemDC(), _ptMouse.x - 25, _ptMouse.y - 25);
}

void playerClass::playerAnimation()
{
	switch (player.state)
	{
	case P_IDLE:
		frameTum = 6;
		oldFrameTum = frameTum;
		if (SLOW)
		{
			frameTum *= SLOW_Ratio;
		}
		else
		{
			frameTum = oldFrameTum;
		}
		if (player.isRight)
		{
			player_idle->setFrameY(0);
			if (frameCnt % frameTum == 0)
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
			
			player_idle->setFrameY(1);
			if (frameCnt % frameTum == 0)
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
		frameTum = 5;
		oldFrameTum = frameTum;
		if (SLOW)
		{
			frameTum *= SLOW_Ratio;
		}
		else
		{
			frameTum = oldFrameTum;
		}
		if (player.isRight)
		{

			player_run->setFrameY(0);
			if (frameCnt % frameTum == 0)
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
	
			player_run->setFrameY(1);
			if (frameCnt % frameTum == 0)
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
		frameTum = 4;
		oldFrameTum = frameTum;
		if (SLOW)
		{
			frameTum *= SLOW_Ratio;
		}
		else
		{
			frameTum = oldFrameTum;
		}
		if (player.isRight)
		{
	
			player_jump->setFrameY(0);
			if (frameCnt % frameTum == 0)
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

			player_jump->setFrameY(1);
			if (frameCnt % frameTum == 0)
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
	case P_FALL:
		frameTum = 4;
		oldFrameTum = frameTum;
		if (SLOW)
		{
			frameTum *= SLOW_Ratio;
		}
		else
		{
			frameTum = oldFrameTum;
		}
		if (player.isRight)
		{

			player_fall->setFrameY(0);
			if (frameCnt % frameTum == 0)
			{
				frameIdx++;
				if (frameIdx > 3)
				{
					frameIdx = 0;
				}
				player_fall->setFrameX(frameIdx);
			}
		}
		else
		{

			player_fall->setFrameY(1);
			if (frameCnt % frameTum == 0)
			{
				frameIdx--;
				if (frameIdx < 1)
				{
					frameIdx = 4;
				}
				player_fall->setFrameX(frameIdx);
			}
		}
		break;
	case P_ROLL:
		frameTum = 2;
		oldFrameTum = frameTum;
		if (SLOW)
		{
			frameTum *= SLOW_Ratio;
		}
		else
		{
			frameTum = oldFrameTum;
		}
		if (player.isRight)
		{

			player_roll->setFrameY(0);
			if (frameCnt % frameTum == 0)
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
	
			player_roll->setFrameY(1);
			if (frameCnt % frameTum == 0)
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
	case P_ATTACK:
		frameTum = 2;
		oldFrameTum = frameTum;
		if (SLOW)
		{
			frameTum *= SLOW_Ratio;
		}
		else
		{
			frameTum = oldFrameTum;
		}
		if (player.isRight)
		{
	
			player_attack->setFrameY(0);
			if (frameCnt % frameTum == 0)
			{
				frameIdx++;
				if (frameIdx > 3)player.isAttack = false;
				if (frameIdx > 6)
				{
					frameIdx = 7;
					ATTACK = false;
					player.state = P_IDLE;
				}
				player_attack->setFrameX(frameIdx);
			}
		}
		else
		{
		
			player_attack->setFrameY(1);
			if (frameCnt % frameTum == 0)
			{
				frameIdx--;
				if (frameIdx < 4)player.isAttack = false;
				if (frameIdx < 1)
				{
					frameIdx = 0;
					ATTACK = false;
					player.state = P_IDLE;
				}
				player_attack->setFrameX(frameIdx);
			}
		}
		break;
	case P_HURTFLY:
		frameTum = 4;
		oldFrameTum = frameTum;
		if (SLOW)
		{
			frameTum *= SLOW_Ratio;
		}
		else
		{
			frameTum = oldFrameTum;
		}
		if (player.isRight)
		{

			player_hurtfly->setFrameY(0);
			if (frameCnt % frameTum == 0)
			{
				frameIdx++;
				if (frameIdx > 3)
				{
					frameIdx = 0;
				}
				player_hurtfly->setFrameX(frameIdx);
			}
		}
		else
		{

			player_hurtfly->setFrameY(1);
			if (frameCnt % frameTum == 0)
			{
				frameIdx--;
				if (frameIdx < 1)
				{
					frameIdx = 4;
				}
				player_hurtfly->setFrameX(frameIdx);
			}
		}
		break;
	case P_HURTGROUND:
		frameTum = 4;
		oldFrameTum = frameTum;
		if (SLOW)
		{
			frameTum *= SLOW_Ratio;
		}
		else
		{
			frameTum = oldFrameTum;
		}
		if (player.isRight)
		{

			player_hurtground->setFrameY(0);
			if (frameCnt % frameTum == 0)
			{
				frameIdx++;
				if (frameIdx > 5)
				{
					frameIdx = 6;
					player.regen = true;
				}
				player_hurtground->setFrameX(frameIdx);
			}
		}
		else
		{

			player_hurtground->setFrameY(1);
			if (frameCnt % frameTum == 0)
			{
				frameIdx--;
				if (frameIdx < 1)
				{
					frameIdx = 0;
					player.regen = true;
				}
				player_hurtground->setFrameX(frameIdx);
			}
		}
		break;
	case P_GRAB:
		frameTum = 3;
		oldFrameTum = frameTum;
		if (SLOW)
		{
			frameTum *= SLOW_Ratio;
		}
		else
		{
			frameTum = oldFrameTum;
		}
		if (player.isRight)
		{

			player_grab->setFrameY(0);
			if (frameCnt % frameTum == 0)
			{
				frameIdx++;
				if (frameIdx > 3)
				{
					frameIdx = 4;
					
				}
				player_grab->setFrameX(frameIdx);
			}
		}
		else
		{
			player_grab->setFrameY(1);
			if (frameCnt % frameTum == 0)
			{
				frameIdx--;
				if (frameIdx < 1)
				{
					frameIdx = 0;
				}
				player_grab->setFrameX(frameIdx);
			}
		}
		break;
	case P_FLIP:
		frameTum = 2;
		oldFrameTum = frameTum;
		if (SLOW)
		{
			frameTum *= SLOW_Ratio;
		}
		else
		{
			frameTum = oldFrameTum;
		}
		if (player.isRight)
		{

			player_flip->setFrameY(0);
			if (frameCnt % frameTum == 0)
			{
				frameIdx++;
				if (frameIdx > 10)
				{
					frameIdx = 0;

				}
				player_flip->setFrameX(frameIdx);
			}
		}
		else
		{

			player_flip->setFrameY(1);
			if (frameCnt % frameTum == 0)
			{
				frameIdx--;
				if (frameIdx < 1)
				{
					frameIdx = 11;
				}
				player_flip->setFrameX(frameIdx);
			}
		}
		break;
	case P_DOORBREAK:
		frameTum = 2;
		oldFrameTum = frameTum;
		if (SLOW)
		{
			frameTum *= SLOW_Ratio;
		}
		else
		{
			frameTum = oldFrameTum;
		}
		if (player.isRight)
		{

			player_doorbreak->setFrameY(0);
			if (frameCnt % frameTum == 0)
			{
				frameIdx++;
				if (frameIdx > 9)
				{
					player.state = P_IDLE;
					frameIdx = 0;

				}
				player_doorbreak->setFrameX(frameIdx);
			}
		}
		else
		{

			player_doorbreak->setFrameY(1);
			if (frameCnt % frameTum == 0)
			{
				frameIdx--;
				if (frameIdx < 1)
				{
					player.state = P_IDLE;
					frameIdx = 10;
				}
				player_doorbreak->setFrameX(frameIdx);
			}
		}
		break;
	case P_RUNTOIDLE:
		frameTum = 7;
		oldFrameTum = frameTum;
		if (SLOW)
		{
			frameTum *= SLOW_Ratio;
		}
		else
		{
			frameTum = oldFrameTum;
		}
		if (player.isRight)
		{

			player_run_to_idle->setFrameY(0);
			if (frameCnt % frameTum == 0)
			{
				frameIdx++;
				if (frameIdx > 4)
				{
					player.state = P_IDLE;
					frameIdx = 0;

				}
				player_run_to_idle->setFrameX(frameIdx);
			}
		}
		else
		{

			player_run_to_idle->setFrameY(1);
			if (frameCnt % frameTum == 0)
			{
				frameIdx--;
				if (frameIdx < 1)
				{
					player.state = P_IDLE;
					frameIdx = 11;
				}
				player_run_to_idle->setFrameX(frameIdx);
			}
		}
		break;
	case P_IDLETORUN:
		frameTum = 7;
		oldFrameTum = frameTum;
		if (SLOW)
		{
			frameTum *= SLOW_Ratio;
		}
		else
		{
			frameTum = oldFrameTum;
		}
		if (player.isRight)
		{

			player_idle_to_run->setFrameY(0);
			if (frameCnt % frameTum == 0)
			{
				frameIdx++;
				if (frameIdx > 3)
				{
					player.state = P_RUN;
					frameIdx = 0;

				}
				player_idle_to_run->setFrameX(frameIdx);
			}
		}
		else
		{

			player_idle_to_run->setFrameY(1);
			if (frameCnt % frameTum == 0)
			{
				frameIdx--;
				if (frameIdx < 1)
				{
					player.state = P_RUN;
					frameIdx = 10;
				}
				player_idle_to_run->setFrameX(frameIdx);
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

		if (r == 255 && g == 0 && b == 0 && (player.state == P_JUMP || player.state == P_FALL))
		{
			player.wallGrabLeft = true;
		}
		else
		{
			player.wallGrabLeft = false;
		}

		if (!(r == 255 && g == 0 && b == 255 || r == 0 && g == 0 && b == 0))
		{
			WALLCRASH_LEFT = true;
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

		if (r == 255 && g == 0 && b == 0 && (player.state == P_JUMP || player.state == P_FALL))
		{
			player.wallGrabRight = true;
		}
		else
		{
			player.wallGrabRight = false;
		}

		if (!(r == 255 && g == 0 && b == 255 || r == 0 && g == 0 && b == 0))
		{
			WALLCRASH_RIGHT = true;
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

		if (!(r == 255 && g == 0 && b == 255))
		{
			if(r == 0 && g == 0 && b == 0)continue;
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

		COLORREF color = GetPixel(DATA->getBgCol().img->getMemDC(), i, findbottom );
		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);

		if (r == 0 && g == 0 && b == 0)
		{
			
			STAND_ON_BLACK = true;
		}
		else
		{
			STAND_ON_BLACK = false;
		}

		if (!(r == 255 && g == 0 && b == 255) && JUMP_BELOW == false)
		{
			if (r == 255 && g == 0 && b == 0)continue;
			WALLCRASH_BOTTOM = true;
			gravity = 0;
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
	if (player.state == P_HURTFLY)return;
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
				if (camera.rc.bottom > WINSIZEY && DATA->getBgCol().y < DATA->getBgCol().height- WINSIZEY)
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
			player.clear = true;
		}
	}
}

void playerClass::rollOnFloor()
{
	/*이부분이 픽셀충돌의 핵심*/
	for (int i = findY - 50; i < findY + 50; i++)
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
				if (camera.rc.bottom > WINSIZEY&& DATA->getBgCol().y < DATA->getBgCol().height - WINSIZEY)
				{
					DATA->addBgY(player.rollSpeed);
				}
			}
			else
			{
				if (camera.rc.top < 0 && DATA->getBgCol().y > 0)
				{
					DATA->minusBgY(player.rollSpeed);
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

void playerClass::cameraReset()
{
	if (DATA->getCurrentClear() == STAGE3_CLEARED)
	{
		if (camera.rc.top <= 0 && DATA->getBgCol().y >= 0)
		{
			DATA->addBgY(-0.1f);
		}

		if (camera.rc.bottom >= WINSIZEY && DATA->getBgCol().y <= DATA->getBgCol().height - WINSIZEY)
		{
			DATA->addBgY(0.1f);
		}
	}
	else
	{
		if (camera.rc.top < 0 && DATA->getBgCol().y > 0)
		{
			DATA->addBgY(-0.5f);
		}

		if (camera.rc.bottom > WINSIZEY && DATA->getBgCol().y <= DATA->getBgCol().height - WINSIZEY)
		{
			DATA->addBgY(0.5f);
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

