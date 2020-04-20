#pragma once
#include "singletonBase.h"

enum enum_clearedStage
{
	NONE_CLEARED,
	STAGE1_CLEARED,
	STAGE2_CLEARED,
};


class allData : public singletonBase <allData>
{
private:
	enum_clearedStage clearedStage;

public:
	HRESULT init();
	void release();

	enum_clearedStage getCurrentClear() { return clearedStage; }
	void setClear(enum_clearedStage _set) { clearedStage = _set; }

	allData() {}
	~allData() {}
};

