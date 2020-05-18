#include "stdafx.h"
#include "timeManager.h"
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
//timeGetTime�Լ��� ����ϱ� ���ؼ�
//�����찡 ���۵ǰ� �� �� �ð��� ���Ͻ����ش�
//GetTickCount���� ������ �ɿ���~ �� ����

HRESULT timeManager::init(void)
{
	//���� Ÿ�̸� �������� üũ
	if (QueryPerformanceFrequency((LARGE_INTEGER*)&_periodFrequency))
	{
		//���� Ÿ�̸Ӹ� �����ϴϱ�
		_isHardware = true;
		//�ʴ� �ð� ���(1�� ��� ī���� �ϳ�)
		_timeScale = 1.0f / _periodFrequency;
		//�ʱ�ȭ ������ �ð��� ������ �ð�����
		QueryPerformanceCounter((LARGE_INTEGER*)&_lastTime);
	}
	else
	{
		//���� Ÿ�̸Ӹ� �������� �����ϱ�
		_isHardware = false;
		_timeScale = 1.0f / 1000.0f;
		_lastTime = timeGetTime();
	}

	//������ �ʱ�ȭ
	_frameRate = 0;
	_FPSFrameCount = 0;
	_FPSTimeElapsed = 0.0f;
	_worldTime = 0;

	return S_OK;
}

void timeManager::release(void)
{
}

void timeManager::update(float lockFPS)
{
	if (_isHardware)
	{
		//����ð� ���
		QueryPerformanceCounter((LARGE_INTEGER*)&_curTime);
	}
	else
	{
		//���� Ÿ�̸Ӹ� �������� �ʱ⶧���� �׳� time�Լ� ����Ѵ�
		_curTime = timeGetTime();
	}
	
	//�������� ������ �� ���
	_timeElapsed = (_curTime - _lastTime) * _timeScale;
	
	//������ ��Ű��
	if (lockFPS > 0.0f)
	{
		while (_timeElapsed < (1.0f / lockFPS))
		{
			if (_isHardware)
			{
				//����ð� ���
				QueryPerformanceCounter((LARGE_INTEGER*)&_curTime);
			}
			else
			{
				//���� Ÿ�̸Ӹ� �������� �ʱ⶧���� �׳� time�Լ� ����Ѵ�
				_curTime = timeGetTime();
			}
			//�������� ������ �� ���
			_timeElapsed = (_curTime - _lastTime) * _timeScale;
		}
	}
	
	//���� �ð� ����
	_lastTime = _curTime;
	//������ �� ����
	_FPSTimeElapsed += _timeElapsed;
	//������ �ð� ����
	_worldTime += _timeElapsed;
	
	//������ �ʱ�ȭ�� 1�ʸ��� �����ϱ�
	if (_FPSTimeElapsed > 1.0f)
	{
		_frameRate = _FPSFrameCount;
		_FPSFrameCount = 0;
		_FPSTimeElapsed = 0.0f;
	}
	
	_FPSFrameCount++;
}

void timeManager::render(HDC hdc)
{
	if (DATA->getRenderUi() == true)
	{
		char str[256];
		//���ڹ����
		SetBkMode(hdc, TRANSPARENT);
		//���ڻ���
		SetTextColor(hdc, RGB(255, 0, 255));

		//FPS
		sprintf(str, "FPS: %d", _frameRate);
		TextOut(hdc, 155, 15, str, strlen(str));

		//��ü�ð�
		//sprintf(str, "Time: %.2f", _worldTime);
		//TextOut(hdc, 0, 20, str, strlen(str));

		//���콺 ��ǥ
		sprintf(str, "MOUSE : %.0f , %.0f", (float)_ptMouse.x + DATA->getBgCol().x, (float)_ptMouse.y + DATA->getBgCol().y);
		TextOut(hdc, 750, 8, str, strlen(str));

		////���콺 ��ǥ
		//sprintf(str, "MOUSE2 : %d , %d", _ptMouse.x , _ptMouse.y);
		//TextOut(hdc, 0, 40, str, strlen(str));

	}
	
}
