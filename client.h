#pragma once
/*
FILE CLIENT.H
将客户机实体化成一个client对象，用于数据缓存池等
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



#define dPACKET_ALIVE_CHECK		0	//用于连接状态的确认
#define dPACKET_CHANGE_NICK		1	//绰号的修改
#define dPACKET_MAINTANK_POS	2   //客户-》服务器  用来报告自己的位置
#define dPACKET_IS_READY		3	//客户-》服务器 表示自己准备开始游戏
#define dPACKET_NEW_BULLET		4	//客户-》服务器 新的子弹创建的时候就会传输该数据包(id,x,y,dir)
#define dPACKET_ALL_POS			5	//服务器-》客户  所有元素的位置信息（each item:id,x,y,dir）(tank / bullet / )
#define dPACKET_BOOM_ACT		6   //服务器-》客户 执行爆炸的元素（坦克 & 炸弹）（id,boom_x,boom_Y）
#define dPACKET_FRIEND			7	//服务器-》客户 服务器找到了一个与之共同作战的队友，信息:对方的nickName和ip地址




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
	游戏的主要数据
	*/
	MainTank mainTank;

	MainTank friendTank;    //new added here

	char friend_ip[20];       //队友的ip地址
	char friend_nickName[64];	//队友的名字		

	bool got_friend = false;	//是否拿到了队友的信息

								// Bullet list

								//友军可以共享一个子弹集合
	list<Object*> lstMainTankBullets;

	//电脑坦克共享一个子弹集合
	list<Object*> lstBullets;

	// Bomb List
	list<Object*> lstBombs;

	// Tank list
	list<Tank*> lstTanks;


	/*
	end of 游戏的主要数据
	*/


	/*
	初始化方法
	*/
	client();
	client(MainTank mainTank, MainTank friendTank, list<Object*> lstMainTankBullets, list<Object*>lstBullets, list<Object*>lstBombs, list<Tank*>lstTanks);

	/*
	尝试从套接字处读取数据：select机制
	*/
	void RecvData();

	/*
	要连接的服务器IP
	*/
	char m_ServerIP[128];

	/*
	绰号
	*/
	char m_NickName[64];

	/*
	用于recv queue的变量
	*/
	int m_recvPos;
	int m_recvSize;
	char m_recvBuff[dMAX_SOCK_BUFF];



	/*
	客户端的套接字
	*/
	SOCKET m_sock;


	/*
	关闭游戏
	*/
	void closeGame()
		;

	/*
	设置套接字为NonBlocking模式

	*/
	void NonBlock(SOCKET sock);

	/*
	function:建立连接，连接套接字保存在client.m_sock中
	@return: TRUE，连接成功；FALSE：连接失败
	*/
	BOOL ConnenctToServer(char*host, int port);

	/*
	对所有接收到的缓存数据包进行响应
	*/
	void ReadRecvBuff();


	/*
	数据处理元函数
	*/
	//BYTE
	void PutByte(char *packet, BYTE value, int &nPos);
	BYTE GetByte(char* packet, int & nPos);


	/*
	在pPacket参数的nPos放置WORD类型的value的值
	*/
	void PutWord(char *pPacket, WORD value, int & nPos);

	/*
	吧pPacket参数nPos位置的值以WORD类型返回
	*/

	WORD GetWord(char* pPacket, int& nPos);

	/*
	在pPacket参数的nPos放置DWORD类型的value的值
	*/
	void PutDword(char *pPacket, DWORD value, int & nPos);

	/*
	吧pPacket参数nPos位置的值以DWORD类型返回
	*/

	DWORD GetDword(char* pPacket, int& nPos);



	/*
	在pPacket参数的nPos放置short类型的value的值
	*/
	void PutShort(char *pPacket, short value, int & nPos);

	/*
	吧pPacket参数nPos位置的值以short类型返回
	*/

	short GetShort(char* pPacket, int& nPos);


	/*
	在pPacket参数的nPos放置int类型的value的值
	*/
	void PutInteger(char *pPacket, int value, int & nPos);

	/*
	吧pPacket参数nPos位置的值以int类型返回
	*/

	int GetInteger(char* pPacket, int& nPos);

	/*
	在pPacket参数的nPos放置字符串
	*/
	void PutString(char *pPacket, char *str, int & nPos);

	/*
	吧pPacket参数nPos位置的字符串复制到pBuffer（包括NULL的处理）
	*/

	void GetString(char* pPacket, char *pBuffer, int& nPos);


	/*
	数据包大小输入函数
	*/
	void PutSize(char* packet, WORD nPos);

	/*
	数据处理函数
	*/
	void ReadRecvBuff(bool& bGameOver);


	/*
	数据包发送
	*/
	void  OnSendIsReady();

	void OnSendNewBullet(Bullet newBullet);


	void OnSendMainPos();

	void OnSendChangeNick(char *nick);


	/*
	处理接收到的数据包
	*/

	/*
	处理连接状态确认的数据包
	*/
	//void HANDLE_PACKET_AliveCheck(sPCLIENT_DATA pClient);

	/*
	修改绰号的数据包
	*/
	//	void HANDLE_PACKET_ChangeNick(sPCLIENT_DATA pClient);


	/*
	更新所有元素的位置
	@para:char* package
	*/
	void HANDLE_ALL_POS(char*package, bool& bGameOver);

	/*
	执行爆炸
	*/
	void HANDLE_BOOM_ACT(char* package, bool& bGameOver);


	/*
	处理服务器为客户找到了队友的信息
	*/
	void HANDLE_FRIEND(char* package);


	//客户机应当要维护子弹、我方坦克，敌军坦克、友方坦克的信息



	/*
		清除画布，判断是否进入新一关；
		如果不是，执行所有物体的move之后display(including bombs)
	展示目前存在的所有物体，如果主战坦克死亡则展示gameOver界面
	*/
	void move_display_all(bool& bGameOver);

};


#endif // !__CLIENT_H_
