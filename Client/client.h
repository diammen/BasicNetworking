#pragma once
#include <WinSock2.h>
#include <Windows.h>
#include <stdio.h>
#include <atomic>
#include "..\Networking\player.h"

#pragma warning(disable:4996)

class client
{
private:
	WORD winsock_version;
	WSADATA winsock_data;
	int address_family;
	int type;
	int protocol;
	SOCKET sock;
	SOCKADDR_IN server_address;

public:
	player player;
	std::atomic<bool> is_running;

	client();

	void init();
};