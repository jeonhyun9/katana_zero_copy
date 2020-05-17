#pragma once
#include "gameNode.h"
class objectClass :public gameNode
{
private:
	struct_Object sObject;
	vector<struct_Object> vObject;
	
	bool SLOW_inObject;
	float SLOW_ratio_inObject;

public:
	HRESULT init();
	void release();
	void update();
	void render();

	//������Ʈ ����
	void generateObject(float _x, float _y, int _findWidht ,enum_ObjectCategory _category);

	//������Ʈ �Ⱥ��̰� ����
	void generateObject(float _x, float _y, int _findWidht ,enum_ObjectCategory _category,bool _set);
	void objectAnimation(int i);

	//������Ʈ ��������
	vector<struct_Object> getVectorObject() { return vObject; }
	struct_Object getObject(int i) { return vObject[i]; }

	RECT getObjectRect(int i) { return vObject[i].rc; }
	RECT getObjectFindRect(int i) { return vObject[i].findRange; }

	void setDoorBreak(int i) 
	{ 
		SOUNDMANAGER->play("p_doorbreak",1.5f);
		vObject[i].isBreak = true; 
		vObject[i].isPlayerReachedFindArea = true;
	}

	void setDoorBreak(int i,bool _set)
	{
		SOUNDMANAGER->play("p_doorbreak", 1.5f);
		vObject[i].frameIdx = 19;
		vObject[i].isRight = false;
		vObject[i].isBreak = true;
		vObject[i].isPlayerReachedFindArea = true;
	}
	void setDoorReachedFindArea(int i) { vObject[i].isPlayerReachedFindArea = true; }

	//�� ũ�� ����
	void setDoorWidth(int i, int _set) { vObject[i].width = _set; }

	//�� ���� ����
	void setDoorDirection(int i, bool _set) { vObject[i].isRight = _set; 
		if (vObject[i].isRight == false)
		{
			vObject[i].x -= 80;
			vObject[i].frameY = 1;
		}
	}
	void setDoorFrameIdx(int i, int _set) 
	{ 
		vObject[i].frameIdx = _set; 
	}

	void setRenderFalse(int i)
	{
		vObject[i].isRender = false;
	}

};

