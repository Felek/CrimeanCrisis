#include "GameServer.h"

using namespace std;

GameServer::GameServer()
{
	if (enet_initialize() != 0) {
		printf("Could not initialize enet.");
		//return 0;
	}

	address.host = ENET_HOST_ANY;
	address.port = PORT;
}


GameServer::~GameServer()
{
	enet_host_destroy(server);
	enet_deinitialize();
}


void GameServer::Listen(ENetHost *server)
{
	ENetEvent event;
	ENetPacket *packet;
	char buffer[BUFFERSIZE];

	while (enet_host_service(server, &event, 1000) > 0) {
		cout << "switch";
		switch (event.type) {

		case ENET_EVENT_TYPE_CONNECT:
			printf("A new client connected from %x:%u.\n",
				event.peer->address.host,
				event.peer->address.port);
			/* Store any relevant client information here. */
			event.peer->data = "Client information";
			break;

		case ENET_EVENT_TYPE_RECEIVE:
			if (event.peer->data == NULL) {
				event.peer->data =
					malloc(strlen((char*)event.packet->data) + 1);
				strcpy_s((char*)event.peer->data, 1000, (char*)
					event.packet->data);

				sprintf_s(buffer, "%s has connected\n",
					(char*)event.packet->data);
				SendToBroadcast(buffer);
			}
			else {
				Interpret(event.packet);
				SendToPeers("dupa;asds");
			}
			break;

		case ENET_EVENT_TYPE_DISCONNECT:
			sprintf_s(buffer, "%s has disconnected.", (char*)
				event.peer->data);
			SendToBroadcast(buffer);
			//	free(event.peer->data);
			//	event.peer->data = NULL;
			break;

		default:
			printf("Tick tock.\n");
			break;
		}
	}
}


void GameServer::SendToPeers(char* data)
{
	for (int i = 0; i < server->peerCount; i++) {
		//if (&server->peers[i] != event.peer) {
		ENetPacket *packet = enet_packet_create(data,
			strlen(data) + 1, 0);
		enet_peer_send(&server->peers[i], 0,
			packet);
		enet_host_flush(server);
		//}
		//else {
		//}
	}
}

void GameServer::SendToBroadcast(char* data) 
{
	ENetPacket *packet = enet_packet_create(data, strlen(data) + 1, 0);
	enet_host_broadcast(server, 1, packet);
	enet_host_flush(server);
}

int GameServer::Start()
{
	server = enet_host_create(&address, 32, 2, 0, 0);

	if (server == NULL) {
		printf("Could not start server.\n");
		//return 0;
	}
	else
		cout << "Server started!";

	while (1) {
		Listen(server);
	}

	return 0;
}

void GameServer::Interpret(ENetPacket *packet)
{
	string data = (char*)packet->data;
	
	vector<string> params = Split(data, ';');

	int i;

	//for (int i = 0; i < params.size(); i++) {
	//	cout << params[i] << endl;
	//}

	if (params[0] == "broadcast")
	{
		char* buf = new char[20];
		_itoa_s((int)server->address.host, buf, 20, 10);
		string bufor = "SERVER_INFO;" + string(buf);
		SendToPeers((char*)bufor.c_str());
	}
}

vector<string> GameServer::Split(const string &s, char delim) {
	vector<string> elems;
	Split(s, delim, elems);
	return elems;
}

std::vector<std::string>& GameServer::Split(const std::string &s, char delim, std::vector<std::string> &elems) {
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}