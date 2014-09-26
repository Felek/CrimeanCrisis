#pragma once

#include <cstdio>
#include <cstdlib>
#include <iostream>
//#include <WinSock2.h>
#include <thread>
#include <string.h>
#include <vector>
#include <sstream>
#include "enet/enet.h"

#define BUFFERSIZE 50

class GameServer
{
public:
	GameServer();
	~GameServer();
	int Start();
	void Listen(ENetHost *server);
	void SendToPeers(char* data);
	void SendToBroadcast(char* data);
	void Interpret(ENetPacket*);

private:
	std::vector<std::string> Split(const std::string &s, char delim);
	std::vector<std::string>& Split(const std::string &s, char delim, std::vector<std::string> &elems);

	SOCKET mainSocket;
	ENetHost *server;
	ENetAddress address;
	const int PORT = 8032;
};

