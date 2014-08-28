#pragma once

#include <cstdio>
#include <cstdlib>
#include <WinSock2.h>
#include <list>
#include "GraphicObject.h"

class Klient
{
public:
	Klient(std::list<GraphicObject>*);
	~Klient();
	int Start();
	void SendInit();
	void Init();
	char* Receive();
	bool Interpret(char*);		//return false if klient has to been destroyed

private:
	int _client_id;
	SOCKET mainSocket;
	std::list<GraphicObject>* _objList;
};

