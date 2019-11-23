#pragma once
#include <WinSock2.h>
#include <Windows.h>
#include <stdio.h>
#include <atomic>
#include "..\Client\odin.h"
#include "player.h"

#pragma warning(disable: 4996)

class server
{
private:
	WORD winsock_version;
	WSADATA winsock_data;
	int address_family;
	int type;
	int protocol;
	SOCKET sock;
	SOCKADDR_IN local_address;

	int8 buffer[SOCKET_BUFFER_SIZE];
	int flags = 0;
	SOCKADDR_IN from;
	int from_size = sizeof(from);
	int bytes_received;

public:
	player player;
	std::atomic<bool> is_running;

	server();

	void init();
	bool send();
	bool receive();
	void getInput();
};