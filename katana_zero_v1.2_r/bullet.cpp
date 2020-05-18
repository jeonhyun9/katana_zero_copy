#include "stdafx.h"
#include "bullet.h"

//=============================================================
//	## missile ## (missile[0] -> 배열처럼 미리 장전해두고 총알발사)
//=============================================================
HRESULT bossBullet::init(int bulletMax, float range)
{
	//총알 사거리 및 총알 갯수 초기화
	_range = range;
	_bulletMax = bulletMax;

	//총알의 갯수만큼 구조체를 초기화 한 후 벡터에 담기
	for (int i = 0; i < bulletMax; i++)
	{
		//총알 구조체 선언
		tagBullet bullet;
		//총알구조체 초기화
		//제로메모리, 멤셋
		//구조체의 변수들의 값을 한번에 0으로 초기화 시켜준다
		ZeroMemory(&bullet, sizeof(tagBullet));
		bullet.bulletImage = new image;
		bullet.bulletImage->init("images/boss/boss_bullet.bmp", 68, 2, true, RGB(255, 0, 255));
		bullet.speed = 20.0f;
		bullet.fire = false;

		//벡터에 담기
		_vBullet.push_back(bullet);
	}

	return S_OK;
}

void bossBullet::release()
{
	for (int i = 0; i < _vBullet.size(); i++)
	{
		_vBullet[i].bulletImage->release();
		SAFE_DELETE(_vBullet[i].bulletImage);
	}
}

void bossBullet::update()
{
	this->move();
	if (_vBullet.empty() == false)
	{
		for (int i = 0; i < _vBullet.size(); i++)
		{
			RECT tempRECT;
			if (IntersectRect(&tempRECT, &_vBullet[i].rc, &DATA->getPlayerRECT()) && DATA->getPlayerHurt() == false && _vBullet[i].fire == true && DATA->getRoll()==false)
			{
				DATA->setPlayerHurt(true);
			}
			if (IntersectRect(&tempRECT, &DATA->getPlayerAttRect(), &_vBullet[i].rc) && DATA->getPlayerData().isAttack == true && _vBullet[i].isReflect == false && DATA->getPlayerData().state == P_ATTACK)
			{
				_vBullet[i].angle = DATA->getPlayerData().toMouseAngle;
				SOUNDMANAGER->play("p_reflect");
				_vBullet[i].isReflect = true;
				DATA->setReflect(_vBullet[i].x, _vBullet[i].y);
			}
			if (IntersectRect(&tempRECT, &DATA->getBossRect(), &_vBullet[i].rc) && _vBullet[i].isReflect == true && DATA->getBossHit()==false && DATA->getBossData().state!=B_DIEFLY && DATA->getBossData().state!=B_HURTRECOVER && DATA->getBossData().state!=B_DIEGROUND )
			{
				_vBullet[i].isReflect = false;
				SOUNDMANAGER->play("death_bullet");
				_vBullet[i].fire = false;
				DATA->setHitLazer(DATA->getBossData().x, DATA->getBossData().y, _vBullet[i].angle);
				DATA->setBossHit(true);
			}
		}
	}
}

void bossBullet::render()
{
	for (int i = 0; i < _vBullet.size(); i++)
	{
		if (!_vBullet[i].fire) continue;
		//Rectangle(getMemDC(),_vBullet[i].rc);
		_vBullet[i].bulletImage->rotateRender(getMemDC(), _vBullet[i].rc.left, _vBullet[i].rc.top,_vBullet[i].angle);
	}
}

void bossBullet::fire(float x, float y, float _angle)
{
	_viBullet = _vBullet.begin();
	for (_viBullet; _viBullet != _vBullet.end(); ++_viBullet)
	{
		if (_viBullet->fire) continue;
		SOUNDMANAGER->play("e_fire");
		_viBullet->fire = true;
		_viBullet->isReflect = false;
		_viBullet->x = _viBullet->fireX = x;
		_viBullet->y = _viBullet->fireY = y;
		_viBullet->angle = _angle;
		_viBullet->rc = RectMakeCenter(_viBullet->x, _viBullet->y,
			5,
			5);
		break;
	}
}

void bossBullet::move()
{
	_viBullet = _vBullet.begin();
	for (_viBullet; _viBullet != _vBullet.end(); ++_viBullet)
	{
		if (!_viBullet->fire) continue;
		_viBullet->y += -sinf(_viBullet->angle) * (_viBullet->speed / DATA->getSlowRatio());
		_viBullet->x += cosf(_viBullet->angle) * (_viBullet->speed / DATA->getSlowRatio());
		_viBullet->rc = RectMakeCenter(_viBullet->x, _viBullet->y,
			5,
			5);
		
		//총알이 사거리보다 커졌을때
		float distance = getDistance(_viBullet->fireX, _viBullet->fireY, _viBullet->x, _viBullet->y);
		if (_range < distance)
		{
			_viBullet->fire = false;
		}
	}
}

HRESULT bossMine::init(int bulletMax, float range)
{
	//총알 사거리 및 총알 갯수 초기화
	effectPtBullet = new effectClass;
	effectPtBullet->init();

	_range = range;
	_bulletMax = bulletMax;

	//총알의 갯수만큼 구조체를 초기화 한 후 벡터에 담기
	for (int i = 0; i < bulletMax; i++)
	{
		//총알 구조체 선언
		tagBullet bullet;
		//총알구조체 초기화
		//제로메모리, 멤셋
		//구조체의 변수들의 값을 한번에 0으로 초기화 시켜준다
		ZeroMemory(&bullet, sizeof(tagBullet));
		bullet.bulletImage = new image;
		bullet.bulletImage->init("images/boss/boss_mine.bmp", 20, 20, true, RGB(255, 0, 255));
		bullet.speed = 24.0f;
		bullet.fire = false;

		//벡터에 담기
		_vBullet.push_back(bullet);
	}
	return S_OK;
}

void bossMine::release()
{
	effectPtBullet->release();
	SAFE_DELETE(effectPtBullet);

	for (int i = 0; i < _vBullet.size(); i++)
	{
		_vBullet[i].bulletImage->release();
		SAFE_DELETE(_vBullet[i].bulletImage);
	}
}

void bossMine::update()
{
	effectPtBullet->update();
	this->move();
	
	if (effectPtBullet->getVectorExplosion().empty() == false)
	{
		for (int i = 0; i < effectPtBullet->getVectorExplosion().size(); i++)
		{
			RECT tempRECT;
			if (IntersectRect(&tempRECT, &effectPtBullet->getExplosionRect(i), &DATA->getPlayerRECT())&& DATA->getPlayerHurt()==false && effectPtBullet->getExplosion(i).frameIdx < 7)
			{
				DATA->setPlayerHurt(true);
			}
		}
	}
	
}

void bossMine::render()
{
	if (_vBullet.empty() == false)
	{
		for (int i = 0; i < _vBullet.size(); i++)
		{
			if (!_vBullet[i].fire) continue;
			//Rectangle(getMemDC(),_vBullet[i].rc);
			_vBullet[i].frameCnt++;
			if (_vBullet[i].frameCnt % 14 == 0)
			{
				_vBullet[i].frameIdx++;
				if (_vBullet[i].frameIdx > 1)
				{
					_vBullet[i].frameIdx = 0;
				}
			}
			if (_vBullet[i].isSticky)
			{
				_vBullet[i].stickyCnt++;
				IMAGEMANAGER->findImage("boss_mine")->frameRender(getMemDC(), _vBullet[i].rc.left - DATA->getBgCol().x - 10, _vBullet[i].rc.top - DATA->getBgCol().y - 10, _vBullet[i].frameIdx, 1);
				IMAGEMANAGER->findImage("boss_circle")->alphaRender(getMemDC(), _vBullet[i].rc.left - DATA->getBgCol().x - 135, _vBullet[i].rc.top - DATA->getBgCol().y - 135, 80);
				if (_vBullet[i].stickyCnt > 60 * DATA->getSlowRatio())
				{
					for (int j = 0; j < 30; j++)
					{
						float _x = RANDOM->Range(_vBullet[i].rc.left - DATA->getBgCol().x - 90, _vBullet[i].rc.left - DATA->getBgCol().x + 135);
						float _y = RANDOM->Range(_vBullet[i].rc.top - DATA->getBgCol().y - 90, _vBullet[i].rc.top - DATA->getBgCol().y + 135);
						effectPtBullet->generateExplosion(_x, _y, 0);
					}
					SOUNDMANAGER->play("boom", 0.7f);
					_vBullet[i].stickyCnt = 0;
					_vBullet[i].isSticky = false;
					_vBullet[i].fire = false;
				}
			}
			else
			{
				_vBullet[i].bulletImage->rotateRender(getMemDC(), _vBullet[i].rc.left - DATA->getBgCol().x, _vBullet[i].rc.top - DATA->getBgCol().y, _vBullet[i].rotateAngle);
			}
		}
	}
	effectPtBullet->render();
}

void bossMine::fire(float x, float y, float _angle)
{
	_viBullet = _vBullet.begin();
	for (_viBullet; _viBullet != _vBullet.end(); ++_viBullet)
	{
		if (_viBullet->fire) continue;
		SOUNDMANAGER->play("lazer", 0.5f);
		_viBullet->fire = true;
		_viBullet->isSticky = false;
		_viBullet->frameCnt = _viBullet->frameIdx = _viBullet->stickyCnt = 0;
		_viBullet->x = _viBullet->fireX = x;
		_viBullet->y = _viBullet->fireY = y;
		_viBullet->moveCnt = 0;
		_viBullet->stickyTum = RANDOM->Range(5, 35);
		_viBullet->angle = _angle;
		_viBullet->rotateAngle = 0;
		_viBullet->rc = RectMakeCenter(_viBullet->x, _viBullet->y,
			_viBullet->bulletImage->getWidth(),
			_viBullet->bulletImage->getHeight());
		break;
	}
}

void bossMine::move()
{
	_viBullet = _vBullet.begin();
	for (_viBullet; _viBullet != _vBullet.end(); ++_viBullet)
	{
		if (!_viBullet->fire) continue;
		if (_viBullet->isSticky)continue;
		_viBullet->moveCnt++;
		_viBullet->rotateAngle += 0.4 / DATA->getSlowRatio();
		_viBullet->y += -sinf(_viBullet->angle) * (_viBullet->speed / DATA->getSlowRatio());
		_viBullet->x += cosf(_viBullet->angle) * (_viBullet->speed / DATA->getSlowRatio());
		_viBullet->rc = RectMakeCenter(_viBullet->x, _viBullet->y,
			_viBullet->bulletImage->getWidth(),
			_viBullet->bulletImage->getHeight());

		//총알이 사거리보다 커졌을때
		float distance = getDistance(_viBullet->fireX, _viBullet->fireY, _viBullet->x, _viBullet->y);
		if (_viBullet->moveCnt > _viBullet->stickyTum)
		{
			SOUNDMANAGER->play("sticky", 0.5f);
			_viBullet->isSticky = true;
		}
	}
}
