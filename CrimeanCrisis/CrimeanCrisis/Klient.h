#pragma once

#include <cstdio>
#include <cstdlib>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include "enet/enet.h"

using namespace std;

class Klient
{
public:
	Klient();
	~Klient();
	int Start();
	void FindServers();
	void Send(char* data);
	void SendToBroadcast(char* data);
	void Interpret(ENetPacket*);

private:
	std::vector<std::string> Split(const std::string &s, char delim);
	std::vector<std::string>& Split(const std::string &s, char delim, std::vector<std::string> &elems);

	//SOCKET mainSocket;
	ENetHost *_client;
	ENetAddress  address;
	ENetEvent  event;
	ENetPeer  *peer;
	string HOST;
	const int PORT = 8032;
	const int BUFFERSIZE = 50;
	//vector<ENetAddress> _servers;
	//vector<string> _servers;
};

