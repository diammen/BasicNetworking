#include "client.h"

client::client()
{

}

void client::init()
{
	winsock_version = 0x202;

	if (WSAStartup(winsock_version, &winsock_data))
	{
		printf("WSAStartup failed: %d", WSAGetLastError());
		return;
	}

	address_family = AF_INET;
	type = SOCK_DGRAM;
	protocol = IPPROTO_UDP;
	sock = socket(address_family, type, protocol);

	if (sock == INVALID_SOCKET)
	{
		printf("socket failed: %d", WSAGetLastError());
		return;
	}
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(9999);
	server_address.sin_addr.s_addr = INADDR_ANY;

	is_running = true;
}