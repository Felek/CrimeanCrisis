#pragma once

#include <cstdio>
#include <cstdlib>
#include <WinSock2.h>
#include <thread>
#include <list>

class GameServer
{
public:
	GameServer();
	~GameServer();
	int Start();
	void Listen();
	void Send(char* data);

private:
	SOCKET mainSocket;
	std::list<SOCKET> _socketList;
	std::list<int> _id_list;
};

