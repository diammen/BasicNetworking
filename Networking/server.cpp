#include "server.h"

server::server()
{
	init();
}

void server::init()
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
	local_address.sin_family = AF_INET;
	local_address.sin_port = htons(9999);
	local_address.sin_addr.s_addr = INADDR_ANY;

	if (bind(sock, (SOCKADDR*)&local_address, sizeof(local_address)) == SOCKET_ERROR)
	{
		printf("bind failed: %d", WSAGetLastError());
		return;
	}

	is_running = 1;
}

bool server::send()
{
	// create state packet
	int32 bytes_written = 0;
	memcpy(&buffer[bytes_written], &player.player_x, sizeof(player.player_x));
	bytes_written += sizeof(player.player_x);

	memcpy(&buffer[bytes_written], &player.player_y, sizeof(player.player_y));
	bytes_written += sizeof(player.player_y);

	memcpy(&buffer[bytes_written], &player.player_facing, sizeof(player.player_facing));
	bytes_written += sizeof(player.player_facing);

	memcpy(&buffer[bytes_written], &is_running, sizeof(is_running));

	// send back to client
	int buffer_length = sizeof(player.player_x) + sizeof(player.player_y) + sizeof(is_running);
	flags = 0;
	SOCKADDR* to = (SOCKADDR*)&from;
	int to_length = sizeof(from);
	if (sendto(sock, buffer, buffer_length, flags, to, to_length) == SOCKET_ERROR)
	{
		printf("sendto failed: %d", WSAGetLastError());
		return false;
	}
	return true;
}

bool server::receive()
{
	bytes_received = recvfrom(sock, buffer, SOCKET_BUFFER_SIZE, flags, (SOCKADDR*)&from, &from_size);

	if (bytes_received == SOCKET_ERROR)
	{
		printf("recfrom returned SOCKET_ERROR, WSAGetLastError() %d", WSAGetLastError());
		return false;
	}
	return true;
}

void server::getInput()
{
	// process input and update state
	int8 client_input = buffer[0];
	printf("%d.%d.%d.%d:%d - %c\n",
		from.sin_addr.S_un.S_un_b.s_b1,
		from.sin_addr.S_un.S_un_b.s_b2,
		from.sin_addr.S_un.S_un_b.s_b3,
		from.sin_addr.S_un.S_un_b.s_b4,
		from.sin_port,
		client_input);

	if (client_input & 0x1)
	{
		player.player_speed += ACCELERATION * SECONDS_PER_TICK;
		if (player.player_speed > MAX_SPEED)
		{
			player.player_speed = MAX_SPEED;
		}
	}
	if (client_input & 0x2)
	{
		player.player_speed -= ACCELERATION * SECONDS_PER_TICK;
		if (player.player_speed < 0.0f)
		{
			player.player_speed = 0.0f;
		}
	}
	if (client_input & 0x4)
	{
		player.player_facing -= TURN_SPEED * SECONDS_PER_TICK;
	}
	if (client_input & 0x8)
	{
		player.player_facing += TURN_SPEED * SECONDS_PER_TICK;
	}

	player.player_x += player.player_speed * SECONDS_PER_TICK * sinf(player.player_facing);
	player.player_y += player.player_speed * SECONDS_PER_TICK * cosf(player.player_facing);
}
