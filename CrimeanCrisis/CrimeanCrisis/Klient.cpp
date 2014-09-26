#include "Klient.h"
#include <thread>

using namespace std;

Klient::Klient() : HOST("192.168.0.255")
{
	if (enet_initialize() != 0) {
		printf("Could not initialize enet.\n");
		//	return 0;
	}
	atexit(enet_deinitialize);
	_client = enet_host_create(NULL, 1, 2, 5760 / 8, 1440 / 8);

	if (_client == NULL) {
		printf("Could not create client.\n");
		//	return 0;
	}

	enet_address_set_host(&address, HOST.c_str());
	address.host = ENET_HOST_BROADCAST;
	address.port = PORT;

	//FindServers();
}


Klient::~Klient()
{
	enet_host_destroy(_client);
	enet_deinitialize();
}


void Klient::FindServers()
{
	char buffer[50];
	peer = enet_host_connect(_client, &address, 2, 0);

	if (peer == NULL) {
		printf("Could not connect to server\n");
		//	return 0;
	}

	ENetEvent event;
	/* Wait up to 1000 milliseconds for an event. */
	for (int i = 0; i < 2; i++)
		while (enet_host_service(_client, &event, 1000) > 0)
		{
		switch (event.type)
		{
		case ENET_EVENT_TYPE_CONNECT:
			printf("Connected to %x:%u.\n",
				event.peer->address.host,
				event.peer->address.port);
			/* Store any relevant client information here. */
			//event.peer->data = "Client information";
			Send("broadcast");
			break;
		case ENET_EVENT_TYPE_RECEIVE:
			printf("A packet of length %u containing %s was received from %s on channel %u.\n",
				event.packet->dataLength,
				event.packet->data,
				event.peer->data,
				event.channelID);
			/* Clean up the packet now that we're done using it. */
			Interpret(event.packet);
			event.peer->data = NULL;
			enet_packet_destroy(event.packet);
			break;

		case ENET_EVENT_TYPE_DISCONNECT:
			event.peer->data = NULL;
		}
		}

}


int Klient::Start()
{
	char buffer[50];
	peer = enet_host_connect(_client, &address, 2, 0);

	if (peer == NULL) {
		printf("Could not connect to server\n");
		//	return 0;
	}

	ENetEvent event;
	/* Wait up to 1000 milliseconds for an event. */
	while (enet_host_service(_client, &event, 1000) > 0)
	{
		switch (event.type)
		{
		case ENET_EVENT_TYPE_CONNECT:
			printf("Connected to %x:%u.\n",
				event.peer->address.host,
				event.peer->address.port);
			/* Store any relevant client information here. */
			//event.peer->data = "Client information";
			Send("dupa;od;klienta");
			break;
		case ENET_EVENT_TYPE_RECEIVE:
			printf("A packet of length %u containing %s was received from %s on channel %u.\n",
				event.packet->dataLength,
				event.packet->data,
				event.peer->data,
				event.channelID);
			/* Clean up the packet now that we're done using it. */
			Interpret(event.packet);
			enet_packet_destroy(event.packet);
			break;

		case ENET_EVENT_TYPE_DISCONNECT:
			printf("%s disconnected.\n", event.peer->data);
			/* Reset the peer's client information. */
			event.peer->data = NULL;
		}
	}

	return 0;
}

void Klient::Interpret(ENetPacket *packet)
{
	string data = (char*)packet->data;

	vector<string> params = Split(data, ';');

	int i;

	if (params[0] == "SERVER_INFO")
	{
		//ENetAddress adr;
		//adr.host = atoi(params[1].c_str());
		//_servers.push_back(adr);
		//_servers.push_back(params[1]);
		//address.host = atoi(params[1].c_str());
		//address.port = PORT;
		enet_address_set_host(&address, params[1].c_str());
		address.port = PORT;
	}

	//for (int i = 0; i < params.size(); i++) {
	//	std::cout << params[i] << endl;
	//}
}

void Klient::Send(char* data)
{
	ENetPacket *packet = enet_packet_create(data, strlen(data) + 1,
		ENET_PACKET_FLAG_RELIABLE);
	enet_peer_send(peer, 0, packet);
}

void Klient::SendToBroadcast(char* data)
{
	ENetPacket *packet = enet_packet_create(data, strlen(data) + 1, 0);
	enet_host_broadcast(_client, 1, packet);
}

vector<string> Klient::Split(const string &s, char delim) {
	vector<string> elems;
	Split(s, delim, elems);
	return elems;
}

std::vector<std::string>& Klient::Split(const std::string &s, char delim, std::vector<std::string> &elems) {
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}