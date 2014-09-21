#pragma once

#include <cstdio>
#include <cstdlib>
#include <WinSock2.h>
#include <string>
#include "enet/enet.h"

class Klient
{
public:
	Klient();
	~Klient();
	int Start();

private:
	ENetHost *_client;
	ENetAddress  address;
	ENetEvent  event;
	ENetPeer  *peer;
	ENetPacket  *packet;
	std::string HOST;
	const int PORT = 8032;
	const int BUFFERSIZE = 50;
};

