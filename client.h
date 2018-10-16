#pragma once
/*
FILE CLIENT.H
���ͻ���ʵ�廯��һ��client�����������ݻ���ص�
*/

#ifndef __CLIENT_H__
#define __CLIENT_H__

#include <WinSock2.h>

#include <windows.h>
#include "object.h"
#include "point.h"
#include "tank.h"
#include "graphic.h"
#include "maintank.h"
#include "enemytank.h"
#include "shape.h"
#include "setting.h"
#include "bullet.h"


//#include <winsock.h>

////#ifdef WIN32
////for win
//#include <winsock.h>
//#include <mmsystem.h>
////#endif



#define dPACKET_ALIVE_CHECK		0	//��������״̬��ȷ��
#define dPACKET_CHANGE_NICK		1	//�ºŵ��޸�
#define dPACKET_MAINTANK_POS	2   //�ͻ�-��������  ���������Լ���λ��
#define dPACKET_IS_READY		3	//�ͻ�-�������� ��ʾ�Լ�׼����ʼ��Ϸ
#define dPACKET_NEW_BULLET		4	//�ͻ�-�������� �µ��ӵ�������ʱ��ͻᴫ������ݰ�(id,x,y,dir)
#define dPACKET_ALL_POS			5	//������-���ͻ�  ����Ԫ�ص�λ����Ϣ��each item:id,x,y,dir��(tank / bullet / )
#define dPACKET_BOOM_ACT		6   //������-���ͻ� ִ�б�ը��Ԫ�أ�̹�� & ը������id,boom_x,boom_Y��
#define dPACKET_FRIEND			7	//������-���ͻ� �������ҵ���һ����֮��ͬ��ս�Ķ��ѣ���Ϣ:�Է���nickName��ip��ַ




#define dMAX_SOCK_BUFF 1024	// buffer length
#define dNAME_LEN 50		// name length 
#define dIP_LEN 20			// IP length

//using namespace std;	//may be deleted

//class Object();
//class Point();
//class Tank();
//class Graphic();
//class MainTank();
//class EnemyTank();
//class Shape();
//class Setting();
//class Byllet();

class client {
public:
	/*
	��Ϸ����Ҫ����
	*/
	MainTank mainTank;

	MainTank friendTank;    //new added here

	char friend_ip[20];       //���ѵ�ip��ַ
	char friend_nickName[64];	//���ѵ�����		

	bool got_friend = false;	//�Ƿ��õ��˶��ѵ���Ϣ

								// Bullet list

								//�Ѿ����Թ���һ���ӵ�����
	list<Object*> lstMainTankBullets;

	//����̹�˹���һ���ӵ�����
	list<Object*> lstBullets;

	// Bomb List
	list<Object*> lstBombs;

	// Tank list
	list<Tank*> lstTanks;


	/*
	end of ��Ϸ����Ҫ����
	*/


	/*
	��ʼ������
	*/
	client();
	client(MainTank mainTank, MainTank friendTank, list<Object*> lstMainTankBullets, list<Object*>lstBullets, list<Object*>lstBombs, list<Tank*>lstTanks);

	/*
	���Դ��׽��ִ���ȡ���ݣ�select����
	*/
	void RecvData();

	/*
	Ҫ���ӵķ�����IP
	*/
	char m_ServerIP[128];

	/*
	�º�
	*/
	char m_NickName[64];

	/*
	����recv queue�ı���
	*/
	int m_recvPos;
	int m_recvSize;
	char m_recvBuff[dMAX_SOCK_BUFF];



	/*
	�ͻ��˵��׽���
	*/
	SOCKET m_sock;


	/*
	�ر���Ϸ
	*/
	void closeGame()
		;

	/*
	�����׽���ΪNonBlockingģʽ

	*/
	void NonBlock(SOCKET sock);

	/*
	function:�������ӣ������׽��ֱ�����client.m_sock��
	@return: TRUE�����ӳɹ���FALSE������ʧ��
	*/
	BOOL ConnenctToServer(char*host, int port);

	/*
	�����н��յ��Ļ������ݰ�������Ӧ
	*/
	void ReadRecvBuff();


	/*
	���ݴ���Ԫ����
	*/
	//BYTE
	void PutByte(char *packet, BYTE value, int &nPos);
	BYTE GetByte(char* packet, int & nPos);


	/*
	��pPacket������nPos����WORD���͵�value��ֵ
	*/
	void PutWord(char *pPacket, WORD value, int & nPos);

	/*
	��pPacket����nPosλ�õ�ֵ��WORD���ͷ���
	*/

	WORD GetWord(char* pPacket, int& nPos);

	/*
	��pPacket������nPos����DWORD���͵�value��ֵ
	*/
	void PutDword(char *pPacket, DWORD value, int & nPos);

	/*
	��pPacket����nPosλ�õ�ֵ��DWORD���ͷ���
	*/

	DWORD GetDword(char* pPacket, int& nPos);



	/*
	��pPacket������nPos����short���͵�value��ֵ
	*/
	void PutShort(char *pPacket, short value, int & nPos);

	/*
	��pPacket����nPosλ�õ�ֵ��short���ͷ���
	*/

	short GetShort(char* pPacket, int& nPos);


	/*
	��pPacket������nPos����int���͵�value��ֵ
	*/
	void PutInteger(char *pPacket, int value, int & nPos);

	/*
	��pPacket����nPosλ�õ�ֵ��int���ͷ���
	*/

	int GetInteger(char* pPacket, int& nPos);

	/*
	��pPacket������nPos�����ַ���
	*/
	void PutString(char *pPacket, char *str, int & nPos);

	/*
	��pPacket����nPosλ�õ��ַ������Ƶ�pBuffer������NULL�Ĵ���
	*/

	void GetString(char* pPacket, char *pBuffer, int& nPos);


	/*
	���ݰ���С���뺯��
	*/
	void PutSize(char* packet, WORD nPos);

	/*
	���ݴ�����
	*/
	void ReadRecvBuff(bool& bGameOver);


	/*
	���ݰ�����
	*/
	void  OnSendIsReady();

	void OnSendNewBullet(Bullet newBullet);


	void OnSendMainPos();

	void OnSendChangeNick(char *nick);


	/*
	������յ������ݰ�
	*/

	/*
	��������״̬ȷ�ϵ����ݰ�
	*/
	//void HANDLE_PACKET_AliveCheck(sPCLIENT_DATA pClient);

	/*
	�޸Ĵºŵ����ݰ�
	*/
	//	void HANDLE_PACKET_ChangeNick(sPCLIENT_DATA pClient);


	/*
	��������Ԫ�ص�λ��
	@para:char* package
	*/
	void HANDLE_ALL_POS(char*package, bool& bGameOver);

	/*
	ִ�б�ը
	*/
	void HANDLE_BOOM_ACT(char* package, bool& bGameOver);


	/*
	���������Ϊ�ͻ��ҵ��˶��ѵ���Ϣ
	*/
	void HANDLE_FRIEND(char* package);


	//�ͻ���Ӧ��Ҫά���ӵ����ҷ�̹�ˣ��о�̹�ˡ��ѷ�̹�˵���Ϣ



	/*
		����������ж��Ƿ������һ�أ�
		������ǣ�ִ�����������move֮��display(including bombs)
	չʾĿǰ���ڵ��������壬�����ս̹��������չʾgameOver����
	*/
	void move_display_all(bool& bGameOver);

};


#endif // !__CLIENT_H_
