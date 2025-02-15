#pragma once
#include "gameNode.h"

class effectClass : public gameNode
{
private:
	effect reflect;
	effect hitLazer;
	effect timer;
	effect battery;

	effect sExplosion;
	vector<effect> vExplosion;

	effect sFloor;
	vector<effect> vFloor;

	image* redRect;
	int redRect_alpha;
	bool alphaPlus;

	int timeCnt;
	int lazerTimeCnt;

	bool SLOW_inEffect;
	float SLOW_ratio_inEffect;

public:

	HRESULT init();
	void release();
	void update();
	void render();

	effect getReflect() { return reflect; }
	effect getHitLazer() { return hitLazer; }
	effect getBattery() { return battery; }
	effect getTimer() { return timer; }

	//���Լ�
	vector<effect> getVectorExplosion() { return vExplosion; }
	effect getExplosion(int i) { return vExplosion[i]; }
	RECT getExplosionRect(int i) { return vExplosion[i].rc; }

	void reflectAnimation(int _tum);
	void hitLazerMove();

	void generateExplosion(float _x, float _y, int _frameidx);
	void generateFloor(float _x, float _y, int _frameIdx);

	void reflectRender(float x, float y, bool _isRight);
	void hitLazerRender(float x, float y, float _angle);

	effectClass() {}
	~effectClass() {}
};

