#include "Klient.h"
#include <thread>

Klient::Klient() : HOST("localhost")
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
	address.port = PORT;
}


Klient::~Klient()
{
	//WSACleanup();
	//closesocket(mainSocket);
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
			strncpy_s(buffer, "23345", BUFFERSIZE);
			packet = enet_packet_create(buffer, strlen(buffer) + 1,
				ENET_PACKET_FLAG_RELIABLE);
			enet_peer_send(peer, 0, packet);
			break;
		case ENET_EVENT_TYPE_RECEIVE:
			printf("A packet of length %u containing %s was received from %s on channel %u.\n",
				event.packet->dataLength,
				event.packet->data,
				event.peer->data,
				event.channelID);
			/* Clean up the packet now that we're done using it. */
			enet_packet_destroy(event.packet);

			break;

		case ENET_EVENT_TYPE_DISCONNECT:
			printf("%s disconnected.\n", event.peer->data);
			/* Reset the peer's client information. */
			event.peer->data = NULL;
		}
	}

	enet_host_destroy(_client);
	return 0;
}