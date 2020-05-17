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

	//������ ��������
	struct_Item getItem(int i) { return vItem[i]; }
	vector<struct_Item> getVectorItem() { return vItem; }

	//������ ��Ʈ ��������
	RECT getItemRect(int i) { return vItem[i].rc; }

	//������ ����
	void generateItem(float _x, float _y, enum_ItemCategory _category);

	//������ �ݱ�
	void setItemPick(int i, bool _set)
	{
		SOUNDMANAGER->play("p_pick",0.5f);
		vItem[i].isPick = _set;
		vItem[i].isFirstPick = true;
		vItem[i].isFloating = true;
		frameCnt = 0;
	}

	//������ �������ϱ�
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

	//������ �¾����� throw �޽�
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

	//������ ������
	void itemMove(int i);

	//ȭ��ǥ ������
	void arrowAnimation(int i);

	//������ ��ǥ ����
	void setItemLocation(int i, float _x, float _y)
	{
		vItem[i].x = _x;
		vItem[i].y = _y;
	}

	//ȭ��ǥ ����
	void setArrowRender(int i, bool _set)
	{
		vItem[i].arrow.isAvailable = _set;
	}

	itemClass() {}
	~itemClass() {}
};

