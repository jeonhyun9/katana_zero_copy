#include "stdafx.h"
#include "objectClass.h"

HRESULT objectClass::init()
{
	return S_OK;
}

void objectClass::release()
{
}

void objectClass::update()
{
	SLOW_inObject = DATA->getSlow();
	SLOW_ratio_inObject = DATA->getSlowRatio();

	for (int i = 0; i < vObject.size(); i++)
	{
		vObject[i].rc = RectMake(vObject[i].x - DATA->getBgCol().x, vObject[i].y - DATA->getBgCol().y, vObject[i].width, vObject[i].height);
		if(vObject[i].category==O_DOOR)vObject[i].findRange = RectMake(vObject[i].x + 30 - DATA->getBgCol().x, vObject[i].y - DATA->getBgCol().y, vObject[i].findWidth, vObject[i].height);
		vObject[i].frameCnt++;
		this->objectAnimation(i);
	}
}

void objectClass::render()
{
	for (int i = 0; i < vObject.size(); i++)
	{
		//Rectangle(getMemDC(),vObject[i].rc);
		//Rectangle(getMemDC(),vObject[i].findRange);
		if (vObject[i].x + 200 - DATA->getBgCol().x >= 0 && vObject[i].x - 200 - DATA->getBgCol().x <= WINSIZEX)
		{
			if (vObject[i].isRender)
			{
				switch (vObject[i].category)
				{
				case O_DOOR:
					if (vObject[i].isRight)
					{
						vObject[i].img->frameRender(getMemDC(), vObject[i].rc.left - 16, vObject[i].rc.top, vObject[i].frameIdx, vObject[i].frameY);
					}
					else
					{
						vObject[i].img->frameRender(getMemDC(), vObject[i].rc.left - 96, vObject[i].rc.top, vObject[i].frameIdx, vObject[i].frameY);
					}
					break;
				case O_DRUM:
					vObject[i].img->render(getMemDC(), vObject[i].rc.left, vObject[i].rc.top);
					break;
				}
			}
		}
		
	}
}

void objectClass::generateObject(float _x, float _y, int _findWidth, enum_ObjectCategory  _category)
{
	sObject.category = _category;
	sObject.x = _x;
	sObject.y = _y;
	sObject.findWidth = _findWidth;
	sObject.frameCnt = sObject.frameY = 0;
	sObject.isRight = true;
	sObject.isBreak = false;
	sObject.isRender = true;
	sObject.isPlayerReachedFindArea = false;
	
	switch (_category)
	{
	case O_DOOR:
		sObject.img = IMAGEMANAGER->findImage("object_door_break");
		sObject.width = 30;
		sObject.height = 128;
		sObject.frameIdx = 0;
		break;
	case O_DRUM:
		sObject.img = IMAGEMANAGER->findImage("object_drum");
		sObject.width = 42;
		sObject.height = 54;
		sObject.frameIdx = 0;
		break;
	}

	vObject.push_back(sObject);

}

void objectClass::generateObject(float _x, float _y, int _findWidth, enum_ObjectCategory  _category, bool _set)
{
	sObject.category = _category;
	sObject.x = _x;
	sObject.y = _y;
	sObject.findWidth = _findWidth;
	sObject.frameCnt = sObject.frameY = 0;
	sObject.isRight = true;
	sObject.isBreak = false;
	sObject.isRender = false;
	sObject.isPlayerReachedFindArea = false;

	switch (_category)
	{
	case O_DOOR:
		sObject.img = IMAGEMANAGER->findImage("object_door_break");
		sObject.width = 120;
		sObject.height = 128;
		sObject.frameIdx = 0;
		break;
	}

	vObject.push_back(sObject);

}

void objectClass::objectAnimation(int i)
{
	switch (sObject.category)
	{
	case O_DOOR:
		if (vObject[i].isBreak)
		{
			vObject[i].frameTum = 4;
			vObject[i].oldFrameTum = vObject[i].frameTum;
			if (SLOW_inObject)
			{
				vObject[i].frameTum *= SLOW_ratio_inObject;
			}
			else
			{
				vObject[i].frameTum = vObject[i].oldFrameTum;
			}
			if (vObject[i].isRight)
			{
				vObject[i].frameY = 0;
				if (vObject[i].frameCnt % vObject[i].frameTum == 0)
				{
					vObject[i].frameIdx++;
					if (vObject[i].frameIdx > 19)
					{
						vObject[i].frameIdx = 20;
					}
				}
			}
			else
			{
				vObject[i].frameY = 1;
				if (vObject[i].frameCnt % vObject[i].frameTum == 0)
				{
					vObject[i].frameIdx--;
					if (vObject[i].frameIdx < 1)
					{
						vObject[i].frameIdx = 0;
					}
				}
			}
			break;
		}
	}
}

