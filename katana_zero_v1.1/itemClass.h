#pragma once
#include "gameNode.h"
class itemClass : public gameNode
{
private:
	struct_Item sItem;
	vector<struct_Item> vItem;

	int frameIdx;
	int frameCnt;

public:
	HRESULT init();
	void release();
	void update();
	void render();

	//아이템 가져오기
	struct_Item getItem(int i) { return vItem[i]; }
	vector<struct_Item> getVectorItem() { return vItem; }

	//아이템 렉트 가져오기
	RECT getItemRect(int i) { return vItem[i].rc; }

	//아이템 생성
	void generateItem(float _x, float _y, enum_ItemCategory _category);

	//아이템 줍기
	void setItemPick(int i, bool _set)
	{
		SOUNDMANAGER->play("p_pick",0.5f);
		vItem[i].isPick = _set;
		vItem[i].isFirstPick = true;
		vItem[i].isFloating = true;
		frameCnt = 0;
	}

	//아이템 던지게하기
	void setItemThrow(int i, float _angle, bool _set)
	{
		if (DATA->getDead() == false)
		{
			SOUNDMANAGER->play("p_throw", 0.5f);
			vItem[i].isThrow = _set;
			vItem[i].isPick = false;
			vItem[i].shootAngle = _angle;
		}	
	}

	//아이템 맞았으면 throw 펄스
	void setItemThrowFalse(int i)
	{
		switch (vItem[i].category)
		{
		case I_BOTTLE:
			SOUNDMANAGER->play("bottle", 0.8f);
			SOUNDMANAGER->play("glass", 0.8f);
			break;
		case I_BUST:
			SOUNDMANAGER->play("bottle", 0.8f);
			break;
		case I_KNIFE:
			SOUNDMANAGER->play("knife", 0.8f);
			break;
		case I_MOLOTOV:
			SOUNDMANAGER->play("bottle", 0.8f);
			SOUNDMANAGER->play("glass", 0.8f);
			break;
		}
		vItem[i].isThrow = false;
	}

	//아이템 던지기
	void itemMove(int i);

	//화살표 프레임
	void arrowAnimation(int i);

	//아이템 좌표 설정
	void setItemLocation(int i, float _x, float _y)
	{
		vItem[i].x = _x;
		vItem[i].y = _y;
	}

	//화살표 렌더
	void setArrowRender(int i, bool _set)
	{
		vItem[i].arrow.isAvailable = _set;
	}

	itemClass() {}
	~itemClass() {}
};

