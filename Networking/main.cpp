#include <WinSock2.h>
#include <Windows.h>
#include <stdio.h>
#include <math.h>
#include "..\Client\odin.h"
#include "server.h"

static float32 time_since(LARGE_INTEGER t, LARGE_INTEGER frequency)
{
	LARGE_INTEGER now;
	QueryPerformanceCounter(&now);

	return float32(now.QuadPart - t.QuadPart) / float32(frequency.QuadPart);
}

int main()
{
	//WORD winsock_version = 0x202;
	//WSADATA winsock_data;

	//if (WSAStartup(winsock_version, &winsock_data))
	//{
	//	printf("WSAStartup failed: %d", WSAGetLastError());
	//	return 0;
	//}

	//int address_family = AF_INET;
	//int type = SOCK_DGRAM;
	//int protocol = IPPROTO_UDP;
	//SOCKET sock = socket(address_family, type, protocol);

	//if (sock == INVALID_SOCKET)
	//{
	//	printf("socket failed: %d", WSAGetLastError());
	//	return 0;
	//}

	//SOCKADDR_IN local_address;
	//local_address.sin_family = AF_INET;
	//local_address.sin_port = htons(9999);
	//local_address.sin_addr.s_addr = INADDR_ANY;

	//if (bind(sock, (SOCKADDR*)&local_address, sizeof(local_address)) == SOCKET_ERROR)
	//{
	//	printf("bind failed: %d", WSAGetLastError());
	//	return 0;
	//}

	server server1;

	UINT sleep_granularity_ms = 1;
	bool32 sleep_granularity_was_set = timeBeginPeriod(sleep_granularity_ms) == TIMERR_NOERROR;

	LARGE_INTEGER clock_frequency;
	QueryPerformanceFrequency(&clock_frequency);

	//int8 buffer[SOCKET_BUFFER_SIZE];
	//float32 player_x = 0.0f;
	//float32 player_y = 0.0f;
	//float32 player_facing = 0.0f;
	//float32 player_speed = 0.0f;

	//bool32 is_running = 1;
	while (server1.is_running)
	{
		LARGE_INTEGER tick_start_time;
		QueryPerformanceCounter(&tick_start_time);

		//int flags = 0;
		//SOCKADDR_IN from;
		//int from_size = sizeof(from);
		//int bytes_received = recvfrom(sock, buffer, SOCKET_BUFFER_SIZE, flags, (SOCKADDR*)&from, &from_size);

		//if (bytes_received == SOCKET_ERROR)
		//{
		//	printf("recfrom returned SOCKET_ERROR, WSAGetLastError() %d", WSAGetLastError());
		//	break;
		//}
		server1.receive();

		server1.getInput();

		server1.send();

		//// process input and update state
		//int8 client_input = buffer[0];
		//printf("%d.%d.%d.%d:%d - %c\n",
		//from.sin_addr.S_un.S_un_b.s_b1,
		//from.sin_addr.S_un.S_un_b.s_b2,
		//from.sin_addr.S_un.S_un_b.s_b3,
		//from.sin_addr.S_un.S_un_b.s_b4,
		//from.sin_port,
		//client_input);

		//if (client_input & 0x1)
		//{
		//	player_speed += ACCELERATION * SECONDS_PER_TICK;
		//	if (player_speed > MAX_SPEED)
		//	{
		//		player_speed = MAX_SPEED;
		//	}
		//}
		//if (client_input & 0x2)
		//{
		//	player_speed -= ACCELERATION * SECONDS_PER_TICK;
		//	if (player_speed < 0.0f)
		//	{
		//		player_speed = 0.0f;
		//	}
		//}
		//if (client_input & 0x4)
		//{
		//	player_facing -= TURN_SPEED * SECONDS_PER_TICK;
		//}
		//if (client_input & 0x8)
		//{
		//	player_facing += TURN_SPEED * SECONDS_PER_TICK;
		//}

		//player_x += player_speed * SECONDS_PER_TICK * sinf(player_facing);
		//player_y += player_speed * SECONDS_PER_TICK * cosf(player_facing);

		//// create state packet
		//int32 bytes_written = 0;
		//memcpy(&buffer[bytes_written], &player_x, sizeof(player_x));
		//bytes_written += sizeof(player_x);

		//memcpy(&buffer[bytes_written], &player_y, sizeof(player_y));
		//bytes_written += sizeof(player_y);

		//memcpy(&buffer[bytes_written], &player_facing, sizeof(player_facing));
		//bytes_written += sizeof(player_facing);

		//memcpy(&buffer[bytes_written], &is_running, sizeof(is_running));

		//int buffer_length = sizeof(player_x) + sizeof(player_y) + sizeof(is_running);
		//flags = 0;
		//SOCKADDR* to = (SOCKADDR*)&from;
		//int to_length = sizeof(from);
		//if (sendto(sock, buffer, buffer_length, flags, to, to_length) == SOCKET_ERROR)
		//{
		//	printf("sendto failed: %d", WSAGetLastError());
		//	return 0;
		//}

		float32 time_taken_s = time_since(tick_start_time, clock_frequency);

		while (time_taken_s < SECONDS_PER_TICK)
		{
			if (sleep_granularity_was_set)
			{
				DWORD time_to_wait_ms = DWORD((SECONDS_PER_TICK - time_taken_s) * 1000);
				if (time_to_wait_ms > 0)
				{
					Sleep(time_to_wait_ms);
				}
			}

			time_taken_s = time_since(tick_start_time, clock_frequency);
		}
	}
	return 0;
}