

#include "client.h"
#include <WS2tcpip.h>

client::client() {
	/*
	变量初始化
	*/
	*m_recvBuff = NULL;
	m_recvPos = 0;
	m_recvSize = 0;


}

client::client(MainTank m_mainTank, MainTank m_friendTank, list<Object*> m_lstMainTankBullets, list<Object*>m_lstBullets, list<Object*>m_lstBombs, list<Tank*>m_lstTanks) {
	/*
	变量初始化
	*/
	*m_recvBuff = NULL;
	m_recvPos = 0;
	m_recvSize = 0;

	mainTank = m_mainTank;
	friendTank = m_friendTank;

}

/*
连接到服务器:win部分采用网络博客上提供的做法
*/
BOOL client::ConnenctToServer(char*host, int port) {
	struct  sockaddr_in s_addr_in;

	/*
	套接字的创建
	*/

	m_sock = socket(AF_INET, SOCK_STREAM,	0);

	/*
	Error:错误的套接字
	*/
	if (m_sock == INVALID_SOCKET) {
		printf("sth bad!");
		return FALSE;
	}

	s_addr_in.sin_family = AF_INET;
	s_addr_in.sin_port = htons(port);
	u_long addr;
	inet_pton(AF_INET,host,&addr);
	s_addr_in.sin_addr.S_un.S_addr = addr;


	/*
	连接 connect
	*/
	if (connect(m_sock, (struct sockaddr*)&s_addr_in, sizeof(s_addr_in)) != 0)
		return FALSE;

	/*
	设置成NonBlocking模式
	*/
	NonBlock(m_sock);

	return TRUE;
}

/*
将套接字设置成NonBlocking模式
*/
void client::NonBlock(SOCKET sock) {

#ifdef  WIN32
	u_long u10n;
	u10n = 1L;
	ioctlsocket(sock, FIONBIO, (unsigned long*)&u10n);
#else
	/*
	LINUX
	*/
	int flags;
	flags = fcntl(s, F_GETFL, 0);
	flags |= O_NONBLOCK;
	if (fcntl(s, F_SETFL, flags) < 0)
		return; //fail
#endif // ! WIN32
}

/*
游戏结束
*/
void client::closeGame() {
	if (m_sock != INVALID_SOCKET) {
		closesocket(m_sock);
	}
}

/*
从套接字中读取数据
*/
void client::RecvData() {
	fd_set read_set;
	struct timeval tv;

	//set tv
	tv.tv_sec = 0;
	tv.tv_usec = 0;

	/*
	init fd_set
	*/
	FD_ZERO(&read_set);
	FD_SET(m_sock, &read_set);

	//call select
	if (select(m_sock + 1, &read_set, NULL, NULL, &tv) < 0)
		return;

	//检查是否套接字希望传输数据到客户端
	if (FD_ISSET(m_sock, &read_set)) {
		int recvSize;
		char recvBuff[dMAX_SOCK_BUFF];

		recvSize = recv(m_sock, recvBuff, 1024, 0);

		/*
		if nothing
		*/
		if (recvSize == 0) {
			printf("与服务器的连接被断开了。");//just a try
			closesocket(m_sock);
			return;
		}


		/*
		Error
		*/
		if (recvSize < 0) {
			/*
			排除是NonBlocking模式带来的错误
			*/
			if (errno != EWOULDBLOCK)
				return;  //不是因为nonblocking机制而出现的错误——真的错误
		}

		/*
		Error:Buffer Overflow
		*/
		if ((m_recvSize + recvSize) >= dMAX_SOCK_BUFF) {
			printf("Overflow");
			return;
		}


		/*
		everything is OK
		*/
		memcpy(&m_recvBuff[m_recvSize], recvBuff, recvSize);
		m_recvSize += recvSize;

	}


}

/*
数据处理元函数
*/

/*
在pPacket参数的nPos放置BYTE类型的value的值
*/
void client::PutByte(char *pPacket, BYTE value, int & nPos) {

	*(BYTE *)(pPacket + nPos) = value;
	nPos += sizeof(BYTE);
}

/*
吧pPacket参数nPos位置的值以BYTE类型返回
*/

BYTE client::GetByte(char* pPacket, int& nPos) {
	BYTE value = *(BYTE *)(pPacket + nPos);
	nPos += sizeof(BYTE);
	return value;
}


/*
在pPacket参数的nPos放置WORD类型的value的值
*/
void client::PutWord(char *pPacket, WORD value, int & nPos) {
	*(WORD *)(pPacket + nPos) = value;
	nPos += sizeof(WORD);
}

/*
吧pPacket参数nPos位置的值以WORD类型返回
*/

WORD client::GetWord(char* pPacket, int &nPos) {
	WORD value = *(WORD *)(pPacket + nPos);
	nPos += sizeof(WORD);
	return value;
}

/*
在pPacket参数的nPos放置DWORD类型的value的值
*/
void client::PutDword(char *pPacket, DWORD value, int & nPos) {
	*(DWORD *)(pPacket + nPos) = value;
	nPos += sizeof(DWORD);
}

/*
吧pPacket参数nPos位置的值以DWORD类型返回
*/

DWORD client::GetDword(char* pPacket, int &nPos) {
	DWORD value = *(DWORD *)(pPacket + nPos);
	nPos += sizeof(DWORD);
	return value;
}


/*
在pPacket参数的nPos放置short类型的value的值
*/
void client::PutShort(char *pPacket, short value, int & nPos) {
	*(short *)(pPacket + nPos) = value;
	nPos += sizeof(short);
}

/*
把pPacket参数nPos位置的值以short类型返回
*/

short client::GetShort(char* pPacket, int &nPos) {
	short value = *(short*)(pPacket + nPos);
	nPos += sizeof(short);
	return value;
}


/*
在pPacket参数的nPos放置int类型的value的值
*/
void client::PutInteger(char *pPacket, int value, int &nPos) {
	*(int *)(pPacket + nPos) = value;
	nPos += sizeof(int);
}

/*
吧pPacket参数nPos位置的值以int类型返回
*/

int client::GetInteger(char* pPacket, int &nPos) {
	int value = *(int*)(pPacket + nPos);
	nPos += sizeof(int);
	return value;
}


/*
在pPacket参数的nPos放置字符串
*/
void client::PutString(char *pPacket, char *str, int & nPos) {
	*(WORD*)(pPacket + nPos) = strlen(str);
	nPos += sizeof(WORD);
	memcpy(pPacket + nPos, str, strlen(str));
	nPos += strlen(str);
}

/*
吧pPacket参数nPos位置的字符串复制到pBuffer（包括NULL的处理）
*/

void client::GetString(char* pPacket, char *pBuffer, int &nPos) {
	WORD length = *(WORD*)(pPacket + nPos);
	nPos += sizeof(WORD);
	memcpy(pBuffer, pPacket + nPos, length);
	*(pBuffer + length) = '\0';
	nPos += length;
}

/*
设置数据包的总长度:保存在数据包开始部分
*/
void client::PutSize(char *pPacket, WORD nPos) {
	*(WORD *)pPacket = nPos;
}
#define dPACKET_ALL_POS			5	//服务器-》客户  所有元素的位置信息（each item:id,x,y,dir）(tank / bullet / )
#define dPACKET_BOOM_ACT		6   //服务器-》客户 执行爆炸的元素（坦克 & 炸弹）（id,boom_x,boom_Y）


/*
接收到的数据处理函数
*/
void client::ReadRecvBuff(bool& bGameOver) {
	WORD msgSize;
	WORD tag;


	/*
	一次性希望将所有缓存区里面的数据包处理完成
	*/
	while (m_recvSize > 0) {
		m_recvPos = 0; //标志一个数据包内读到的位置

		msgSize = GetWord(m_recvBuff, m_recvPos);
		tag = GetWord(m_recvBuff, m_recvPos);

		/*
		当目前m_recvBuff内数据的长度小于数据包的总长度时
		*/
		if (m_recvSize < msgSize) {
			return;							//???
		}

		/*
		根据不同的类型进行相应的处理：服务器-》客户
		*/
		switch (tag)
		{
		case (dPACKET_ALL_POS):
			HANDLE_ALL_POS(m_recvBuff, bGameOver);
			break;
		case(dPACKET_BOOM_ACT):
			HANDLE_BOOM_ACT(m_recvBuff, bGameOver);
			break;
		case(dPACKET_FRIEND):
			HANDLE_FRIEND(m_recvBuff);
			break;
		default:
			break;
		}
		memmove(&m_recvBuff[0],
			&m_recvBuff[msgSize],
			m_recvSize - msgSize);
		m_recvSize -= msgSize;
	}
	return;

}

/*
处理服务器为客户找到了队友的信息
*/
void client::HANDLE_FRIEND(char* package) {

	/*
	FRIEND数据包结构：size:word->tag:word->ip:string->nickName:string
	*/

	//step1. get data
	int pos = 4;
	GetString(package, friend_ip, pos);
	GetString(package, friend_nickName, pos);

	/*
	拿到了队友的信息
	*/
	got_friend = true;

}

/*
更新所有元素的位置
@para:char* package
*/
void client::HANDLE_ALL_POS(char*package, bool& bGameOver) {
	/*
	ALL_POS数据包结构：size word->tag word->主战坦克1数据{id:byte,x:int,y:int.dir:int,坦克生命值:byte}->友军主战坦克数据{数据同上}->子弹类数据{子弹个数:byte,loop{id:byte,x:int,y:int,dir:int,type:byte0代表我军集合，1代表敌军集合}}->
	电脑坦克数据{坦克数,loop{同上}}
	*/
	//step1 获取所有数据，根据id找到对应的对象更新数据，如果id不存在就新建数据
	int handled_pos = 4;

	BYTE id;
	int pos_x;
	int pos_y;
	Dir dir;
	BYTE life;
	BYTE type;

	int count;

	//更新两个主战坦克类的数据
	for (int i = 0; i < 2; i++) {
		id = GetByte(package, handled_pos);
		pos_x = GetInteger(package, handled_pos);
		pos_y = GetInteger(package, handled_pos);
		dir = (Dir)GetInteger(package, handled_pos);
		life = GetByte(package, handled_pos);

		switch (i)
		{
		case 0:
			mainTank.set_id(id); //第一次主战坦克的id由服务器赋予
			mainTank.SetDir(dir);
			mainTank.set_pos(pos_x, pos_y);
			Setting::setM_nlife(life);
			break;
		case 1:
			//更新数据
			friendTank.set_id(id);
			friendTank.SetDir(dir);
			friendTank.set_pos(pos_x, pos_y);
			Setting::setFriend_Life(life);
			break;
		default:
			break;
		}
	}

	//子弹类数据获取与更新
	count = GetByte(package, handled_pos);

	while (count--) {
		//获取数据
		id = GetByte(package, handled_pos);
		pos_x = GetInteger(package, handled_pos);
		pos_y = GetInteger(package, handled_pos);
		dir = (Dir)GetInteger(package, handled_pos);
		type = (Dir)GetByte(package, handled_pos);

		int flag = 0;
		//按照id找到该子弹
		/*

		基本逻辑：先找敌军坦克的子弹集合，再找我军坦克的子弹集合，如果没有找到，就新建一个子弹，将该子弹加入到对应的集合（0-》我军集合；1：敌军集合）
		*/
		// Check main tank damage
		for (list<Object*>::iterator it = lstBullets.begin(); it != lstBullets.end(); it++)
		{
			if ((*it)->get_id() == id)
			{
				//找到了这样一个子弹,更新相应的数据
				(*it)->set_pos(pos_x, pos_y);
				(*it)->SetDir(dir);

				flag = 1; //表示找到了
						  //退出循环
				break;
			}
		}

		// Check enermy tank damage
		for (list<Object*>::iterator it = lstMainTankBullets.begin(); it != lstMainTankBullets.end() && flag != 1; it++)
		{
			if ((*it)->get_id() == id)
			{
				//找到了这样一个子弹,更新相应的数据
				(*it)->set_pos(pos_x, pos_y);
				(*it)->SetDir(dir);

				flag = 1; //表示找到了
						  //退出循环
				break;
			}
		}

		//如果没有找到满足条件的子弹，说明该子弹是由另一个玩家刚创建的，于是创建一个子弹
		if (flag == 0) {
			Point  point;
			point.Set(pos_x, pos_y);
			Bullet* pBullet = new Bullet(point, dir, mainTank.get_color());
			pBullet->SetStep(20);
			if (type == 0) {
				lstMainTankBullets.push_back(pBullet);
			}
			else if (type == 1) {
				lstBullets.push_back(pBullet);
			}
		}
	}

	//敌军坦克类数据更新
	//敌军坦克类数据更新
	count = GetByte(package, handled_pos);
	while (count--) {
		/*
		基本逻辑：根据id进行检索，如果没有找到，就新创建一个敌军坦克加入到集合中
		*/
		//读取数据
		id = GetByte(package, handled_pos);
		pos_x = GetInteger(package, handled_pos);
		pos_y = GetInteger(package, handled_pos);
		dir = (Dir)GetInteger(package, handled_pos);
		int flag = 0; //表示没有找到对应的坦克
					  //循环寻找
		for (list<Tank*>::iterator it = lstTanks.begin(); it != lstTanks.end();)
		{
			if ((*it)->get_id() == id)
			{
				//找到了这样一个坦克，就更新
				(*it)->set_pos(pos_x, pos_y);
				(*it)->SetDir(dir);

				flag = 1; //表示找到了
						  //退出循环
				break;
			}
		}
		//没有找到
		if (flag != 1) {
			//新建一个敌军坦克，并且加入到坦克集合里面
			EnemyTank* p = new EnemyTank();
			p->set_id(id);
			p->set_pos(pos_x, pos_y);
			p->SetDir(dir);
			lstTanks.push_back(p);
		}
	}

	//step2 调用display：不管是子弹还是啥、都是更新完了所有数据之后，调用display函数进行渲染
	//经过step1之后，两个子弹集合、两个主战坦克，敌军坦克集合可能都有新的元素加入，同时有的旧元素可能已经进行了更新，重新调用渲染函数
	move_display_all(bGameOver);
}

void client::move_display_all(bool& bGameOver)
{


	// Draw Background
	cleardevice();
	Graphic::DrawBattleGround();

	//waiting to be modified
	Graphic::ShowScore();



	// New Game Level
	if (Setting::m_bNewLevel)
	{
		Setting::m_bNewLevel = false;

		Setting::NewGameLevel();

		Graphic::ShowGameLevel(Setting::GetGameLevel());
		
		return;
	}


	//初步粗糙逻辑：如果主战坦克死亡，那么在客户一段终止游戏
	if (mainTank.IsDisappear())
	{
		//skip = true;
		bGameOver = true;

		Graphic::ShowGameOver();
		//return;

	}

	mainTank.Move();
	mainTank.Display();



	/*
	友军坦克
	*/
	if (!friendTank.IsDisappear())
	{
		friendTank.Move();
		friendTank.Display();
	}



	/* Draw Tanks */
	for (list<Tank*>::iterator it = lstTanks.begin(); it != lstTanks.end();)
	{
		(*it)->Move();

		if ((*it)->IsDisappear())
		{
			Setting::TankDamaged();

			// Add a bomb
			(*it)->Boom(lstBombs);

			// Delete the tank
			delete *it;
			it = lstTanks.erase(it);
			continue;
		}

		(*it)->Display();

		if ((*it)->NeedShoot())
		{
			EnemyTank* p = (EnemyTank*)*it;
			p->Shoot(lstBullets);
		}
		it++;
	}

	/* Draw Bullets */
	for (list<Object*>::iterator it = lstMainTankBullets.begin(); it != lstMainTankBullets.end();)
	{
		(*it)->Move();

		if ((*it)->IsDisappear())
		{
			// Add a bomb
			(*it)->Boom(lstBombs);

			// Delete the bullet
			delete *it;
			it = lstMainTankBullets.erase(it);
			continue;
		}

		(*it)->Display();
		it++;
	}

	for (list<Object*>::iterator it = lstBullets.begin(); it != lstBullets.end();)
	{
		(*it)->Move();

		if ((*it)->IsDisappear())
		{
			// Add a bomb
			(*it)->Boom(lstBombs);

			// Delete the bullet
			delete *it;
			it = lstBullets.erase(it);
			continue;
		}

		(*it)->Display();
		it++;
	}

	/* Draw Bombs */
	for (list<Object*>::iterator it = lstBombs.begin(); it != lstBombs.end();)
	{
		(*it)->Move();

		if ((*it)->IsDisappear())
		{
			delete *it;
			it = lstBombs.erase(it);
			continue;
		}

		(*it)->Display();
		it++;
	}
}

/*
执行爆炸
1.根据id找到对象执行setDisappear()
2.委托move_display_all判断isDisappear来做相应处理（删除元素-》爆炸效果绘制）
*/
void client::HANDLE_BOOM_ACT(char* package, bool& bGameOver) {
	//boom_act数据包结构：size:word->tag:word->id:byte->id:byte
	//由于主战坦克和友方主战坦克都有不止一条命，而通过boom_act数据包传过来的id一定是爆炸——也就是生命值已经为零了


	//step1. 获取数据
	int pos = 4;
	byte id_1;
	byte id_2;

	id_1 = GetByte(package, pos);
	id_2 = GetByte(package, pos);

	//step2. 进行处理
	/*
	根据id寻找主战坦克或者友方主战坦克
	*/
	int count = 0;
	if (mainTank.get_id() == id_1 || mainTank.get_id() == id_2) {
		mainTank.SetDisappear();
		count++;
	}
	if (friendTank.get_id()== id_1 || friendTank.get_id() == id_2) {
		friendTank.SetDisappear();
		count++;
	}

	/*
	根据id寻找敌军坦克
	*/
	for (list<Tank*>::iterator it = lstTanks.begin(); it != lstTanks.end() && count<2; it++)
	{
		if ((*it)->get_id() == id_1 || (*it)->get_id() == id_2) {
			count++;
			(*it)->SetDisappear();
		}

	}



	/*
	根据id寻找敌军子弹
	*/
	for (list<Object*>::iterator it = lstBullets.begin(); it != lstBullets.end() && count<2; it++)
	{
		if ((*it)->get_id() == id_1 || (*it)->get_id() == id_2) {
			count++;
			(*it)->SetDisappear();
		}

	}


	/*
	根据id寻找我方子弹
	*/
	for (list<Object*>::iterator it = lstMainTankBullets.begin(); it != lstMainTankBullets.end() && count<2; it++)
	{
		if ((*it)->get_id() == id_1 || (*it)->get_id() == id_2) {
			count++;
			(*it)->SetDisappear();
		}

	}


	/*
	找到了所有要执行爆炸的两个对象之后，调用move_display_all()
	*/
	move_display_all(bGameOver);


}




/*
发送数据：MAINTANK_POS数据包
*/
void client::OnSendMainPos() {
	//update data???

	/*
	数据包结构：size->tag->{Maintank.id:byte,x int ,y int,dir int}->
	*/
	int packetPos;
	char packet[dMAX_SOCK_BUFF];

	packetPos = 2;
	PutWord(packet, dPACKET_MAINTANK_POS, packetPos);

	/*
	放入主战坦克的id
	*/
	PutByte(packet, mainTank.get_id(), packetPos);

	/*
	放入主战坦克的x、y
	*/
	PutInteger(packet, mainTank.get_pos().GetX(), packetPos);
	PutInteger(packet, mainTank.get_pos().GetY(), packetPos);

	/*
	放入主战坦克的dir

	*/
	PutInteger(packet, mainTank.GetDir(), packetPos);
	PutSize(packet, packetPos);

	/*
	发送数据
	*/
	send(m_sock, packet, packetPos, 0);

}


/*
发送数据：IS_READY数据包
*/

void client::OnSendIsReady() {

	/*
	is_ready数据包： size,tag;
	*/
	int  pos;
	char packet[dMAX_SOCK_BUFF];

	pos = 2;
	PutWord(packet, dPACKET_IS_READY, pos);
	PutSize(packet, pos);


	/*
	发送数据
	*/
	send(m_sock, packet, pos, 0);


}

/*
发送数据： NEW_BULLET数据包
*/

void client::OnSendNewBullet(Bullet newBullet) {
	//update data???

	/*
	数据包结构：size->tag->{Bullet.id:byte , x int ,y int,dir int}->
	*/
	int packetPos;
	char packet[dMAX_SOCK_BUFF];

	packetPos = 2;
	PutWord(packet, dPACKET_NEW_BULLET, packetPos);

	/*
	放入bullet的id
	*/
	PutByte(packet, newBullet.get_id(), packetPos);

	/*
	放入bullet的x、y
	*/
	PutInteger(packet, newBullet.get_pos().GetX(), packetPos);
	PutInteger(packet, newBullet.get_pos().GetY(), packetPos);

	/*
	放入bullet的dir

	*/
	PutInteger(packet, newBullet.getDir(), packetPos);
	PutSize(packet, packetPos);

	/*
	发送数据
	*/
	send(m_sock, packet, packetPos, 0);

}