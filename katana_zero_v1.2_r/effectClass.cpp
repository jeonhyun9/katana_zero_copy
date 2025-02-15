#include "stdafx.h"
#include "effectClass.h"

HRESULT effectClass::init()
{
	DATA->reflectOff();
	DATA->hitLazerOff();
	reflect.img = IMAGEMANAGER->findImage("effect_reflect");
	reflect.isRender = false;
	reflect.frameCnt = 0;
	lazerTimeCnt = reflect.frameIdx = 0;
	reflect.x = 0;
	reflect.y = 0;
	
	reflect.isRight = true;
	reflect.frameTum = 5;

	hitLazer.img = IMAGEMANAGER->findImage("effect_hit_lazer");
	hitLazer.isRender = false;
	hitLazer.x = 0;
	hitLazer.y = 0;
	hitLazer.speed = 70.0f;
	hitLazer.width = 1000;
	hitLazer.angle = PI_2;

	timer.img = IMAGEMANAGER->findImage("effect_timer");
	timer.width = 188;
	timer.height = 22;
	timeCnt = 0;

	battery.img = IMAGEMANAGER->findImage("effect_battery");
	battery.width = 110;
	battery.height = 20;

	redRect = IMAGEMANAGER->findImage("effect_red");
	redRect_alpha = 0;
	alphaPlus = true;



	return S_OK;
}

void effectClass::release()
{
}

void effectClass::update()
{
	SLOW_inEffect = DATA->getSlow();
	SLOW_ratio_inEffect = DATA->getSlowRatio();

	if (DATA->getReflect().on == true)
	{
		reflectRender(DATA->getReflect().x, DATA->getReflect().y, true);
	}
	if (DATA->getHitLazer().on == true)
	{
		hitLazerRender(DATA->getHitLazer().x, DATA->getHitLazer().y, DATA->getHitLazer().angle);
	}

	reflect.frameCnt++;
	timeCnt++;
	this->reflectAnimation(5);
	if (DATA->getStop() == false && DATA->getSlow() == false)
	{
		if (timeCnt % 30 == 0)timer.width--;
		if (timeCnt % 100 == 0 && battery.width <= 100)battery.width += 10;
	}
	
	
	this->hitLazerMove();

	//���� �ִϸ��̼�
	if (vExplosion.empty() == false)
	{
		for (int i = 0; i < vExplosion.size(); i++)
		{
			vExplosion[i].rc = RectMakeCenter(vExplosion[i].x - DATA->getBgCol().x, vExplosion[i].y - DATA->getBgCol().y, 60, 60);
			vExplosion[i].frameCnt++;
			if (vExplosion[i].frameCnt % 4 == 0)
			{
				vExplosion[i].frameIdx++;
				if (vExplosion[i].frameIdx == 1 && DATA->getCurrentClear() == STAGE3_CLEARED)
				{
					int rand = RANDOM->Range(1, 3);
					switch (rand)
					{
					case 1:
						SOUNDMANAGER->play("explosion1", 0.5f);
						break;
					case 2:
						SOUNDMANAGER->play("explosion2", 0.5f);
						break;
					case 3:
						SOUNDMANAGER->play("explosion3", 0.5f);
						break;
					}
				}
				if (vExplosion[i].frameIdx > 11)
				{
					vExplosion[i].frameIdx = 12;
					vExplosion.erase(vExplosion.begin() + i);
				}
			}
		}
	}

	//�ٴ� �ִϸ��̼�
	if (vFloor.empty() == false)
	{
		for (int i = 0; i < vFloor.size(); i++)
		{
			if (vFloor[i].isRender == false)
			{
				
				vFloor[i].frameCnt++;
				if (vFloor[i].frameCnt % 4 == 0)
				{
					vFloor[i].frameIdx--;
					if (vFloor[i].frameIdx < 0)
					{
						vFloor[i].frameIdx = 0;
					}
				}
			}
		}
	}

	if (DATA->getSlow() == true)
	{
		if (timeCnt % 30 == 0)battery.width -= 10;

		if (battery.width <= 0)
		{
			SOUNDMANAGER->play("slow_out");
			DATA->setBatteryRemain(false);
		}
		//���͸� �޹�� �����Ÿ���
		alphaPlus ? redRect_alpha+=2 : redRect_alpha-=2;

		if (redRect_alpha == 0)
		{
			alphaPlus = true;
		}
		if (redRect_alpha == 150)
		{
			alphaPlus = false;
		}

	}

	if (battery.width >= 10)
	{
		DATA->setBatteryRemain(true);
	}
}

void effectClass::render()
{
	
	reflect.img->frameRender(getMemDC(), reflect.x - 73 - DATA->getBgCol().x, reflect.y - 70- DATA->getBgCol().y, reflect.frameIdx, reflect.frameY);
	if (hitLazer.isRender)
	{
		hitLazer.img->rotateRender(getMemDC(), hitLazer.x - DATA->getBgCol().x, hitLazer.y - DATA->getBgCol().y, hitLazer.angle);
	}

	timer.img->render(getMemDC(), 551, 6, 0, 0, timer.width, timer.height);
	if (DATA->getSlow() == true)
	{
		redRect->alphaRender(getMemDC(), 24, 13, redRect_alpha);
	}
	battery.img->render(getMemDC(), 24, 13, 0, 0, battery.width, battery.height);


	if (vFloor.empty() == false)
	{
		for (int i = 0; i < vFloor.size(); i++)
		{
			vFloor[i].img->frameRender(getMemDC(), vFloor[i].x - DATA->getBgCol().x, vFloor[i].y - DATA->getBgCol().y, vFloor[i].frameIdx, 0);
		}
	}

	if (vExplosion.empty() == false)
	{
		for (int i = 0; i < vExplosion.size(); i++)
		{
			if (vExplosion[i].x + 200 - DATA->getBgCol().x >= 0 && vExplosion[i].x - 200 - DATA->getBgCol().x <= WINSIZEX)
			{
				vExplosion[i].img->frameRender(getMemDC(), vExplosion[i].x - DATA->getBgCol().x - 100, vExplosion[i].y - DATA->getBgCol().y - 163, vExplosion[i].frameIdx, 0);
			}
		
			//Rectangle(getMemDC(), vExplosion[i].rc);
		}
	}
}

void effectClass::reflectAnimation(int _tum)
{
	reflect.frameTum = _tum;
	//reflect.oldFrameTum = reflect.frameTum;
	//if (SLOW_inEffect)
	//{
	//	reflect.frameTum *= SLOW_ratio_inEffect;
	//}
	//else
	//{
	//	reflect.frameTum = reflect.oldFrameTum;
	//}
	if (reflect.isRight)
	{
		if (reflect.frameCnt % reflect.frameTum == 0)
		{
			reflect.frameIdx++;
			if (reflect.frameIdx > 5)
			{
				reflect.frameIdx = 6;
			}
		}
	}
	else
	{
		if (reflect.frameCnt % reflect.frameTum == 0)
		{
			reflect.frameIdx--;
			if (reflect.frameIdx < 1)
			{
				reflect.frameIdx = 0;
			}
		}
	}
}

void effectClass::hitLazerMove()
{
	if (hitLazer.isRender)
	{
		lazerTimeCnt++;
		hitLazer.x += cosf(hitLazer.angle) * hitLazer.speed;
		hitLazer.y += -sinf(hitLazer.angle) * hitLazer.speed;
		if (lazerTimeCnt > 50)
		{
			lazerTimeCnt = 0;
			hitLazer.isRender = false;
		}
	}
}

void effectClass::generateExplosion(float _x, float _y , int _frameidx)
{
	sExplosion.img = IMAGEMANAGER->findImage("effect_explosion");
	sExplosion.frameIdx = _frameidx;
	sExplosion.frameCnt = 0;
	sExplosion.x = _x;
	sExplosion.y = _y;

	vExplosion.push_back(sExplosion);
}

void effectClass::generateFloor(float _x, float _y, int _frameIdx)
{
	sFloor.img = IMAGEMANAGER->findImage("effect_floor");
	sFloor.frameIdx = _frameIdx;
	sFloor.frameCnt = 0;
	sFloor.isRender = false;
	sFloor.x = _x;
	sFloor.y = _y;

	vFloor.push_back(sFloor);
}

void effectClass::reflectRender(float x, float y,bool _isRight)
{
	DATA->reflectOff();
	
	reflect.x = x;
	reflect.y = y;
	if (_isRight)
	{
		reflect.isRight = true;
		reflect.frameIdx = 0;
		reflect.frameY = 0;
	}
	else
	{
		reflect.isRight = false;
		reflect.frameIdx = 6;
		reflect.frameY = 1;
	}
}

void effectClass::hitLazerRender(float x, float y,float _angle)
{
	DATA->hitLazerOff();
	hitLazer.x = x;
	hitLazer.y = y;
	hitLazer.angle = _angle;
	hitLazer.isRender = true;
}

