

#include "client.h"
#include <WS2tcpip.h>

client::client() {
	/*
	������ʼ��
	*/
	*m_recvBuff = NULL;
	m_recvPos = 0;
	m_recvSize = 0;


}

client::client(MainTank m_mainTank, MainTank m_friendTank, list<Object*> m_lstMainTankBullets, list<Object*>m_lstBullets, list<Object*>m_lstBombs, list<Tank*>m_lstTanks) {
	/*
	������ʼ��
	*/
	*m_recvBuff = NULL;
	m_recvPos = 0;
	m_recvSize = 0;

	mainTank = m_mainTank;
	friendTank = m_friendTank;

}

/*
���ӵ�������:win���ֲ������粩�����ṩ������
*/
BOOL client::ConnenctToServer(char*host, int port) {
	struct  sockaddr_in s_addr_in;

	/*
	�׽��ֵĴ���
	*/

	m_sock = socket(AF_INET, SOCK_STREAM,	0);

	/*
	Error:������׽���
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
	���� connect
	*/
	if (connect(m_sock, (struct sockaddr*)&s_addr_in, sizeof(s_addr_in)) != 0)
		return FALSE;

	/*
	���ó�NonBlockingģʽ
	*/
	NonBlock(m_sock);

	return TRUE;
}

/*
���׽������ó�NonBlockingģʽ
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
��Ϸ����
*/
void client::closeGame() {
	if (m_sock != INVALID_SOCKET) {
		closesocket(m_sock);
	}
}

/*
���׽����ж�ȡ����
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

	//����Ƿ��׽���ϣ���������ݵ��ͻ���
	if (FD_ISSET(m_sock, &read_set)) {
		int recvSize;
		char recvBuff[dMAX_SOCK_BUFF];

		recvSize = recv(m_sock, recvBuff, 1024, 0);

		/*
		if nothing
		*/
		if (recvSize == 0) {
			printf("������������ӱ��Ͽ��ˡ�");//just a try
			closesocket(m_sock);
			return;
		}


		/*
		Error
		*/
		if (recvSize < 0) {
			/*
			�ų���NonBlockingģʽ�����Ĵ���
			*/
			if (errno != EWOULDBLOCK)
				return;  //������Ϊnonblocking���ƶ����ֵĴ��󡪡���Ĵ���
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
���ݴ���Ԫ����
*/

/*
��pPacket������nPos����BYTE���͵�value��ֵ
*/
void client::PutByte(char *pPacket, BYTE value, int & nPos) {

	*(BYTE *)(pPacket + nPos) = value;
	nPos += sizeof(BYTE);
}

/*
��pPacket����nPosλ�õ�ֵ��BYTE���ͷ���
*/

BYTE client::GetByte(char* pPacket, int& nPos) {
	BYTE value = *(BYTE *)(pPacket + nPos);
	nPos += sizeof(BYTE);
	return value;
}


/*
��pPacket������nPos����WORD���͵�value��ֵ
*/
void client::PutWord(char *pPacket, WORD value, int & nPos) {
	*(WORD *)(pPacket + nPos) = value;
	nPos += sizeof(WORD);
}

/*
��pPacket����nPosλ�õ�ֵ��WORD���ͷ���
*/

WORD client::GetWord(char* pPacket, int &nPos) {
	WORD value = *(WORD *)(pPacket + nPos);
	nPos += sizeof(WORD);
	return value;
}

/*
��pPacket������nPos����DWORD���͵�value��ֵ
*/
void client::PutDword(char *pPacket, DWORD value, int & nPos) {
	*(DWORD *)(pPacket + nPos) = value;
	nPos += sizeof(DWORD);
}

/*
��pPacket����nPosλ�õ�ֵ��DWORD���ͷ���
*/

DWORD client::GetDword(char* pPacket, int &nPos) {
	DWORD value = *(DWORD *)(pPacket + nPos);
	nPos += sizeof(DWORD);
	return value;
}


/*
��pPacket������nPos����short���͵�value��ֵ
*/
void client::PutShort(char *pPacket, short value, int & nPos) {
	*(short *)(pPacket + nPos) = value;
	nPos += sizeof(short);
}

/*
��pPacket����nPosλ�õ�ֵ��short���ͷ���
*/

short client::GetShort(char* pPacket, int &nPos) {
	short value = *(short*)(pPacket + nPos);
	nPos += sizeof(short);
	return value;
}


/*
��pPacket������nPos����int���͵�value��ֵ
*/
void client::PutInteger(char *pPacket, int value, int &nPos) {
	*(int *)(pPacket + nPos) = value;
	nPos += sizeof(int);
}

/*
��pPacket����nPosλ�õ�ֵ��int���ͷ���
*/

int client::GetInteger(char* pPacket, int &nPos) {
	int value = *(int*)(pPacket + nPos);
	nPos += sizeof(int);
	return value;
}


/*
��pPacket������nPos�����ַ���
*/
void client::PutString(char *pPacket, char *str, int & nPos) {
	*(WORD*)(pPacket + nPos) = strlen(str);
	nPos += sizeof(WORD);
	memcpy(pPacket + nPos, str, strlen(str));
	nPos += strlen(str);
}

/*
��pPacket����nPosλ�õ��ַ������Ƶ�pBuffer������NULL�Ĵ���
*/

void client::GetString(char* pPacket, char *pBuffer, int &nPos) {
	WORD length = *(WORD*)(pPacket + nPos);
	nPos += sizeof(WORD);
	memcpy(pBuffer, pPacket + nPos, length);
	*(pBuffer + length) = '\0';
	nPos += length;
}

/*
�������ݰ����ܳ���:���������ݰ���ʼ����
*/
void client::PutSize(char *pPacket, WORD nPos) {
	*(WORD *)pPacket = nPos;
}
#define dPACKET_ALL_POS			5	//������-���ͻ�  ����Ԫ�ص�λ����Ϣ��each item:id,x,y,dir��(tank / bullet / )
#define dPACKET_BOOM_ACT		6   //������-���ͻ� ִ�б�ը��Ԫ�أ�̹�� & ը������id,boom_x,boom_Y��


/*
���յ������ݴ�����
*/
void client::ReadRecvBuff(bool& bGameOver) {
	WORD msgSize;
	WORD tag;


	/*
	һ����ϣ�������л�������������ݰ��������
	*/
	while (m_recvSize > 0) {
		m_recvPos = 0; //��־һ�����ݰ��ڶ�����λ��

		msgSize = GetWord(m_recvBuff, m_recvPos);
		tag = GetWord(m_recvBuff, m_recvPos);

		/*
		��Ŀǰm_recvBuff�����ݵĳ���С�����ݰ����ܳ���ʱ
		*/
		if (m_recvSize < msgSize) {
			return;							//???
		}

		/*
		���ݲ�ͬ�����ͽ�����Ӧ�Ĵ���������-���ͻ�
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
���������Ϊ�ͻ��ҵ��˶��ѵ���Ϣ
*/
void client::HANDLE_FRIEND(char* package) {

	/*
	FRIEND���ݰ��ṹ��size:word->tag:word->ip:string->nickName:string
	*/

	//step1. get data
	int pos = 4;
	GetString(package, friend_ip, pos);
	GetString(package, friend_nickName, pos);

	/*
	�õ��˶��ѵ���Ϣ
	*/
	got_friend = true;

}

/*
��������Ԫ�ص�λ��
@para:char* package
*/
void client::HANDLE_ALL_POS(char*package, bool& bGameOver) {
	/*
	ALL_POS���ݰ��ṹ��size word->tag word->��ս̹��1����{id:byte,x:int,y:int.dir:int,̹������ֵ:byte}->�Ѿ���ս̹������{����ͬ��}->�ӵ�������{�ӵ�����:byte,loop{id:byte,x:int,y:int,dir:int,type:byte0�����Ҿ����ϣ�1����о�����}}->
	����̹������{̹����,loop{ͬ��}}
	*/
	//step1 ��ȡ�������ݣ�����id�ҵ���Ӧ�Ķ���������ݣ����id�����ھ��½�����
	int handled_pos = 4;

	BYTE id;
	int pos_x;
	int pos_y;
	Dir dir;
	BYTE life;
	BYTE type;

	int count;

	//����������ս̹���������
	for (int i = 0; i < 2; i++) {
		id = GetByte(package, handled_pos);
		pos_x = GetInteger(package, handled_pos);
		pos_y = GetInteger(package, handled_pos);
		dir = (Dir)GetInteger(package, handled_pos);
		life = GetByte(package, handled_pos);

		switch (i)
		{
		case 0:
			mainTank.set_id(id); //��һ����ս̹�˵�id�ɷ���������
			mainTank.SetDir(dir);
			mainTank.set_pos(pos_x, pos_y);
			Setting::setM_nlife(life);
			break;
		case 1:
			//��������
			friendTank.set_id(id);
			friendTank.SetDir(dir);
			friendTank.set_pos(pos_x, pos_y);
			Setting::setFriend_Life(life);
			break;
		default:
			break;
		}
	}

	//�ӵ������ݻ�ȡ�����
	count = GetByte(package, handled_pos);

	while (count--) {
		//��ȡ����
		id = GetByte(package, handled_pos);
		pos_x = GetInteger(package, handled_pos);
		pos_y = GetInteger(package, handled_pos);
		dir = (Dir)GetInteger(package, handled_pos);
		type = (Dir)GetByte(package, handled_pos);

		int flag = 0;
		//����id�ҵ����ӵ�
		/*

		�����߼������ҵо�̹�˵��ӵ����ϣ������Ҿ�̹�˵��ӵ����ϣ����û���ҵ������½�һ���ӵ��������ӵ����뵽��Ӧ�ļ��ϣ�0-���Ҿ����ϣ�1���о����ϣ�
		*/
		// Check main tank damage
		for (list<Object*>::iterator it = lstBullets.begin(); it != lstBullets.end(); it++)
		{
			if ((*it)->get_id() == id)
			{
				//�ҵ�������һ���ӵ�,������Ӧ������
				(*it)->set_pos(pos_x, pos_y);
				(*it)->SetDir(dir);

				flag = 1; //��ʾ�ҵ���
						  //�˳�ѭ��
				break;
			}
		}

		// Check enermy tank damage
		for (list<Object*>::iterator it = lstMainTankBullets.begin(); it != lstMainTankBullets.end() && flag != 1; it++)
		{
			if ((*it)->get_id() == id)
			{
				//�ҵ�������һ���ӵ�,������Ӧ������
				(*it)->set_pos(pos_x, pos_y);
				(*it)->SetDir(dir);

				flag = 1; //��ʾ�ҵ���
						  //�˳�ѭ��
				break;
			}
		}

		//���û���ҵ������������ӵ���˵�����ӵ�������һ����Ҹմ����ģ����Ǵ���һ���ӵ�
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

	//�о�̹�������ݸ���
	//�о�̹�������ݸ���
	count = GetByte(package, handled_pos);
	while (count--) {
		/*
		�����߼�������id���м��������û���ҵ������´���һ���о�̹�˼��뵽������
		*/
		//��ȡ����
		id = GetByte(package, handled_pos);
		pos_x = GetInteger(package, handled_pos);
		pos_y = GetInteger(package, handled_pos);
		dir = (Dir)GetInteger(package, handled_pos);
		int flag = 0; //��ʾû���ҵ���Ӧ��̹��
					  //ѭ��Ѱ��
		for (list<Tank*>::iterator it = lstTanks.begin(); it != lstTanks.end();)
		{
			if ((*it)->get_id() == id)
			{
				//�ҵ�������һ��̹�ˣ��͸���
				(*it)->set_pos(pos_x, pos_y);
				(*it)->SetDir(dir);

				flag = 1; //��ʾ�ҵ���
						  //�˳�ѭ��
				break;
			}
		}
		//û���ҵ�
		if (flag != 1) {
			//�½�һ���о�̹�ˣ����Ҽ��뵽̹�˼�������
			EnemyTank* p = new EnemyTank();
			p->set_id(id);
			p->set_pos(pos_x, pos_y);
			p->SetDir(dir);
			lstTanks.push_back(p);
		}
	}

	//step2 ����display���������ӵ�����ɶ�����Ǹ���������������֮�󣬵���display����������Ⱦ
	//����step1֮�������ӵ����ϡ�������ս̹�ˣ��о�̹�˼��Ͽ��ܶ����µ�Ԫ�ؼ��룬ͬʱ�еľ�Ԫ�ؿ����Ѿ������˸��£����µ�����Ⱦ����
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


	//�����ֲ��߼��������ս̹����������ô�ڿͻ�һ����ֹ��Ϸ
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
	�Ѿ�̹��
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
ִ�б�ը
1.����id�ҵ�����ִ��setDisappear()
2.ί��move_display_all�ж�isDisappear������Ӧ����ɾ��Ԫ��-����ըЧ�����ƣ�
*/
void client::HANDLE_BOOM_ACT(char* package, bool& bGameOver) {
	//boom_act���ݰ��ṹ��size:word->tag:word->id:byte->id:byte
	//������ս̹�˺��ѷ���ս̹�˶��в�ֹһ��������ͨ��boom_act���ݰ���������idһ���Ǳ�ը����Ҳ��������ֵ�Ѿ�Ϊ����


	//step1. ��ȡ����
	int pos = 4;
	byte id_1;
	byte id_2;

	id_1 = GetByte(package, pos);
	id_2 = GetByte(package, pos);

	//step2. ���д���
	/*
	����idѰ����ս̹�˻����ѷ���ս̹��
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
	����idѰ�ҵо�̹��
	*/
	for (list<Tank*>::iterator it = lstTanks.begin(); it != lstTanks.end() && count<2; it++)
	{
		if ((*it)->get_id() == id_1 || (*it)->get_id() == id_2) {
			count++;
			(*it)->SetDisappear();
		}

	}



	/*
	����idѰ�ҵо��ӵ�
	*/
	for (list<Object*>::iterator it = lstBullets.begin(); it != lstBullets.end() && count<2; it++)
	{
		if ((*it)->get_id() == id_1 || (*it)->get_id() == id_2) {
			count++;
			(*it)->SetDisappear();
		}

	}


	/*
	����idѰ���ҷ��ӵ�
	*/
	for (list<Object*>::iterator it = lstMainTankBullets.begin(); it != lstMainTankBullets.end() && count<2; it++)
	{
		if ((*it)->get_id() == id_1 || (*it)->get_id() == id_2) {
			count++;
			(*it)->SetDisappear();
		}

	}


	/*
	�ҵ�������Ҫִ�б�ը����������֮�󣬵���move_display_all()
	*/
	move_display_all(bGameOver);


}




/*
�������ݣ�MAINTANK_POS���ݰ�
*/
void client::OnSendMainPos() {
	//update data???

	/*
	���ݰ��ṹ��size->tag->{Maintank.id:byte,x int ,y int,dir int}->
	*/
	int packetPos;
	char packet[dMAX_SOCK_BUFF];

	packetPos = 2;
	PutWord(packet, dPACKET_MAINTANK_POS, packetPos);

	/*
	������ս̹�˵�id
	*/
	PutByte(packet, mainTank.get_id(), packetPos);

	/*
	������ս̹�˵�x��y
	*/
	PutInteger(packet, mainTank.get_pos().GetX(), packetPos);
	PutInteger(packet, mainTank.get_pos().GetY(), packetPos);

	/*
	������ս̹�˵�dir

	*/
	PutInteger(packet, mainTank.GetDir(), packetPos);
	PutSize(packet, packetPos);

	/*
	��������
	*/
	send(m_sock, packet, packetPos, 0);

}


/*
�������ݣ�IS_READY���ݰ�
*/

void client::OnSendIsReady() {

	/*
	is_ready���ݰ��� size,tag;
	*/
	int  pos;
	char packet[dMAX_SOCK_BUFF];

	pos = 2;
	PutWord(packet, dPACKET_IS_READY, pos);
	PutSize(packet, pos);


	/*
	��������
	*/
	send(m_sock, packet, pos, 0);


}

/*
�������ݣ� NEW_BULLET���ݰ�
*/

void client::OnSendNewBullet(Bullet newBullet) {
	//update data???

	/*
	���ݰ��ṹ��size->tag->{Bullet.id:byte , x int ,y int,dir int}->
	*/
	int packetPos;
	char packet[dMAX_SOCK_BUFF];

	packetPos = 2;
	PutWord(packet, dPACKET_NEW_BULLET, packetPos);

	/*
	����bullet��id
	*/
	PutByte(packet, newBullet.get_id(), packetPos);

	/*
	����bullet��x��y
	*/
	PutInteger(packet, newBullet.get_pos().GetX(), packetPos);
	PutInteger(packet, newBullet.get_pos().GetY(), packetPos);

	/*
	����bullet��dir

	*/
	PutInteger(packet, newBullet.getDir(), packetPos);
	PutSize(packet, packetPos);

	/*
	��������
	*/
	send(m_sock, packet, packetPos, 0);

}