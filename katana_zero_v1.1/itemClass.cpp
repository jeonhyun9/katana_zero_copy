#include "stdafx.h"
#include "itemClass.h"

HRESULT itemClass::init()
{
	frameCnt = 0;
	frameIdx = 0;
	
	return S_OK;
}

void itemClass::release()
{
}

void itemClass::update()
{
	if (vItem.empty() == false)
	{
		for (int i = 0; i < vItem.size(); i++)
		{
			frameCnt++;
			if (vItem[i].isFloating)
			{
				vItem[i].floatingCnt++;
				if (vItem[i].floatingCnt < 10)
				{
					vItem[i].alpha = 255;
				}
				if (vItem[i].floatingCnt >= 10)
				{
					vItem[i].alpha = 0;
				}
				if (vItem[i].floatingCnt >= 20)
				{
					vItem[i].floatingCnt = 0;
					vItem[i].floatingTime++;
				}
				if (vItem[i].floatingTime > 3)
				{
					vItem[i].isFloating = false;
				}
				if (vItem[i].isThrow)vItem[i].isFloating = false;
			}
			
			vItem[i].rc = RectMake(vItem[i].x - DATA->getBgCol().x, vItem[i].y - DATA->getBgCol().y, vItem[i].width, vItem[i].height);

			this->itemMove(i);
			this->arrowAnimation(i);
		}
	}
}

void itemClass::render()
{
	if (vItem.empty() == false)
	{
		for (int i = 0; i < vItem.size(); i++)
		{
			if (vItem[i].isFirstPick == false)
			{
				vItem[i].img->render(getMemDC(), vItem[i].rc.left, vItem[i].rc.top);
				//화살표 프레임 렌더
				if (vItem[i].arrow.isAvailable)
				{
					switch (vItem[i].category)
					{
					case I_BOTTLE:
						vItem[i].arrow.img->frameRender(getMemDC(), vItem[i].rc.left - 9, vItem[i].rc.top - 51, frameIdx, 0);
						break;
					case I_BUST:
						vItem[i].arrow.img->frameRender(getMemDC(), vItem[i].rc.left + 1, vItem[i].rc.top - 51, frameIdx, 0);
						break;
					case I_KNIFE:
						vItem[i].arrow.img->frameRender(getMemDC(), vItem[i].rc.left + 2, vItem[i].rc.top - 51, frameIdx, 0);
						break;
					case I_MOLOTOV:
						vItem[i].arrow.img->frameRender(getMemDC(), vItem[i].rc.left - 7, vItem[i].rc.top - 51, frameIdx, 0);
						break;
					}
				}
			}

			if (vItem[i].isThrow)
			{
				vItem[i].img->rotateRender(getMemDC(), vItem[i].rc.left, vItem[i].rc.top, vItem[i].rotateAngle);
			}

			if (vItem[i].isPick == true && vItem[i].isThrow == false)
			{
				switch (vItem[i].category)
				{
				case I_BUST:
					vItem[i].uiImg->render(getMemDC(), 1218, 3);
					break;
				case I_BOTTLE:
					vItem[i].uiImg->render(getMemDC(), 1225, 6);
					break;
				case I_KNIFE:
					vItem[i].uiImg->render(getMemDC(), 1214, 3);
					break;
				case I_MOLOTOV:
					vItem[i].uiImg->render(getMemDC(), 1222, -2);
					break;
				}
			}

			if (vItem[i].isFloating)
			{
				switch (vItem[i].category)
				{
				case I_BOTTLE:
					vItem[i].floatingImg->alphaRender(getMemDC(), vItem[i].rc.left - 5, vItem[i].rc.top - 80, vItem[i].alpha);
					break;
				case I_BUST:
					vItem[i].floatingImg->alphaRender(getMemDC(), vItem[i].rc.left - 20, vItem[i].rc.top - 100, vItem[i].alpha);
					break;
				case I_KNIFE:
					vItem[i].floatingImg->alphaRender(getMemDC(), vItem[i].rc.left - 15, vItem[i].rc.top - 80, vItem[i].alpha);
					break;
				case I_MOLOTOV:
					vItem[i].floatingImg->alphaRender(getMemDC(), vItem[i].rc.left - 8, vItem[i].rc.top - 88, vItem[i].alpha);
					break;
				}
			}
			
		}
	}
}

void itemClass::generateItem(float _x, float _y, enum_ItemCategory _category)
{
	sItem.x = _x;
	sItem.y = _y;
	sItem.category = _category;
	sItem.isPick = false;
	sItem.isThrow = false;
	sItem.isFirstPick = false;
	sItem.isFloating = false;
	sItem.floatingTime = 0;
	sItem.arrow.img = IMAGEMANAGER->findImage("item_arrow");
	sItem.arrow.width = 17;
	sItem.arrow.height = 21;
	sItem.arrow.isAvailable = false;
	sItem.speed = 24.0f;
	sItem.rotateAngle = PI / 2;
	sItem.shootAngle = PI / 2;
	sItem.floatingCnt = 0;
	
	switch (_category)
	{
	case I_BOTTLE:
		sItem.width = 12;
		sItem.height = 30;
		sItem.img = IMAGEMANAGER->findImage("item_bottle");
		sItem.uiImg = IMAGEMANAGER->findImage("item_bottle");
		sItem.floatingImg = IMAGEMANAGER->findImage("item_bottle_get");
		break;
	case I_BUST:
		sItem.width = 38;
		sItem.height = 56;
		sItem.img = IMAGEMANAGER->findImage("item_bust");
		sItem.uiImg = IMAGEMANAGER->findImage("item_bust_small");
		sItem.floatingImg = IMAGEMANAGER->findImage("item_bust_get");
		break;
	case I_KNIFE:
		sItem.width = 34;
		sItem.height = 34;
		sItem.img = IMAGEMANAGER->findImage("item_knife");
		sItem.uiImg = IMAGEMANAGER->findImage("item_knife");
		sItem.floatingImg = IMAGEMANAGER->findImage("item_knife_get");
		break;
	case I_MOLOTOV:
		sItem.width = 12;
		sItem.height = 30;
		sItem.img = IMAGEMANAGER->findImage("item_molotov");
		sItem.uiImg = IMAGEMANAGER->findImage("item_molotov");
		sItem.floatingImg = IMAGEMANAGER->findImage("item_molotov_get");
		break;
	}

	vItem.push_back(sItem);
}

void itemClass::itemMove(int i)
{
	if (vItem[i].isThrow)
	{
		vItem[i].x += cosf(vItem[i].shootAngle) * vItem[i].speed / DATA->getSlowRatio();
		vItem[i].y += -sinf(vItem[i].shootAngle) * vItem[i].speed / DATA->getSlowRatio();
		vItem[i].rotateAngle += 0.4 / DATA->getSlowRatio();
	}
}

void itemClass::arrowAnimation(int i)
{
	if (frameCnt % 3 == 0)
	{
		frameIdx++;
		if (frameIdx > 7)
		{
			frameIdx = 0;
		}
	}
}
