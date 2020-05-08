#include "stdafx.h"
#include "mainGame.h"

//=============================================================
//	## �ʱ�ȭ ## init()
//=============================================================
HRESULT mainGame::init()
{
	gameNode::init();
	//Ŭ���� �ʱ�ȭ�� �̰����� �Ѵ�
	
	/*������ ���ΰ����� ������ ���鸸 ������ �Ѵ�*/
	//������ ������ ���ΰ��� ������ �Ѵ�

	/*���߰�*/
	SCENEMANAGER->addScene("����ȭ��", new titleScene);
	SCENEMANAGER->addScene("����ȭ��", new inGameScene);
	SCENEMANAGER->addScene("�ε�ȭ��", new loadingScene);
	SCENEMANAGER->addScene("��������1", new stage1);
	SCENEMANAGER->addScene("��������2", new stage2);
	SCENEMANAGER->addScene("��������3", new stage3);
	SCENEMANAGER->addScene("��������4", new stage4);

	/*����� ����*/
	SCENEMANAGER->loadScene("�ε�ȭ��");


	return S_OK;
}

//=============================================================
//	## ���� ## release()
//=============================================================
void mainGame::release()
{
	gameNode::release();

}

//=============================================================
//	## ������Ʈ ## update()
//=============================================================
void mainGame::update()
{
	gameNode::update();
	
	//���Ŵ��� ������Ʈ
	SCENEMANAGER->update();
	//����Ŵ��� ������Ʈ(�̳�����ָ� ���尡 �����۵�����)
	SOUNDMANAGER->update();
}

//=============================================================
//	## ���� ## render()
//=============================================================
void mainGame::render()
{
	//��� �� ��Ʈ��
	PatBlt(getMemDC(), 0, 0, WINSIZEX, WINSIZEY, WHITENESS);
//=============================================================	

	//���Ŵ��� ����
	SCENEMANAGER->render();
	//Ÿ�ӸŴ��� ����
	TIMEMANAGER->render(getMemDC());

	//���ڹ����
	SetBkMode(getMemDC(), TRANSPARENT);
	//�ؽ�Ʈ ���� ����
	SetTextColor(getMemDC(), RGB(0, 0, 0));

//=============================================================
	//������� ������ ȭ��DC�� �׸���
	this->getBackBuffer()->render(getHDC());
}
