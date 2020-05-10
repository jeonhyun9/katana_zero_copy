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
		vItem[i].isPick = _set;
		vItem[i].isFirstPick = true;
		vItem[i].isFloating = true;
		frameCnt = 0;
	}

	//������ �������ϱ�
	void setItemThrow(int i, float _angle, bool _set)
	{
		vItem[i].isThrow = _set;
		vItem[i].isPick = false;
		vItem[i].shootAngle = _angle;
	}

	//������ �¾����� throw �޽�
	void setItemThrowFalse(int i)
	{
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

