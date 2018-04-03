// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once

// https://msdn.microsoft.com/en-us/library/windows/desktop/ms737889(v=vs.85).aspx

#include <string>

#include <ws2tcpip.h>
#include <winsock2.h>

#pragma comment (lib, "Ws2_32.lib")

#define DEFAULT_BUFLEN 48
// Use an unassigned port in the 9000 range.
// https://www.iana.org/assignments/service-names-port-numbers/service-names-port-numbers.xhtml?=&skey=-2&page=17
// This must match between SpectatorViewPoseProvider and UnityCompositorInterface.
#define DEFAULT_PORT 9481

class WSASession
{
public:
	WSASession()
	{
		int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (iResult != 0)
		{
			OutputDebugString(L"WSA Startup error.\n");
		}
	}

	~WSASession()
	{
		WSACleanup();
	}

private:
	WSADATA wsaData;
};

//TODO: base class and UDP class.
class TCPSocket
{
private:
	// See here for explanation of error codes:
	// https://msdn.microsoft.com/en-us/library/windows/desktop/ms740668(v=vs.85).aspx
	void PrintSocketError(const wchar_t* function)
	{
		OutputDebugStringW(function);
		OutputDebugStringW(L" failed with error: ");
		OutputDebugStringW(std::to_wstring(WSAGetLastError()).c_str());
		OutputDebugStringW(L"\n");
	}

public:
	TCPSocket()
	{
	}

	~TCPSocket()
	{
		closesocket(listenSocket);

		shutdown(connectSocket, SD_SEND);
		closesocket(connectSocket);
	}

	bool CreateClientListener(PCSTR serverIP)
	{
		struct addrinfo *result = NULL;
		struct addrinfo *ptr = NULL;
		struct addrinfo hints;

		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;

		// Resolve the server address and port
		int iResult = getaddrinfo(serverIP, std::to_string(DEFAULT_PORT).c_str(), &hints, &result);
		if (iResult != 0)
		{
			PrintSocketError(L"Clent GetAddrInfo");
			return false;
		}

		if (result == NULL)
		{
			return false;
		}

		// Attempt to connect to an address until one succeeds
		for (ptr = result; ptr != NULL; ptr = ptr->ai_next)
		{
			// Create a SOCKET for connecting to server
			connectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
			if (connectSocket == INVALID_SOCKET)
			{
				PrintSocketError(L"Create ConnectSocket");
				return false;
			}

			// Connect to server.
			// Blocks.
			iResult = connect(connectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
			if (iResult == SOCKET_ERROR)
			{
				PrintSocketError(L"ConnectSocket");
				closesocket(connectSocket);
				connectSocket = INVALID_SOCKET;
				continue;
			}

			// Successfully connected.
			break;
		}

		freeaddrinfo(result);

		if (connectSocket == INVALID_SOCKET)
		{
			OutputDebugString(L"Unable to connect to server!\n");
			return false;
		}

		bool bOptVal = TRUE;
		setsockopt(connectSocket, SOL_SOCKET, TCP_NODELAY, (char *)&bOptVal, sizeof(bool));

		return true;
	}

	// Create socket to listen for connection.
	void CreateServerListener()
	{
		struct addrinfo *result = NULL;
		struct addrinfo hints;

		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		hints.ai_flags = AI_PASSIVE;

		// Resolve the server address and port
		int iResult = getaddrinfo(NULL, std::to_string(DEFAULT_PORT).c_str(), &hints, &result);
		if (iResult != 0)
		{
			PrintSocketError(L"Server GetAddrInfo");
			return;
		}

		listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
		if (listenSocket == INVALID_SOCKET)
		{
			PrintSocketError(L"Create Server Socket");
			freeaddrinfo(result);
			return;
		}

		iResult = bind(listenSocket, result->ai_addr, (int)result->ai_addrlen);
		if (iResult == SOCKET_ERROR)
		{
			PrintSocketError(L"Bind");
			freeaddrinfo(result);
			return;
		}

		freeaddrinfo(result);
	}

	void ServerEstablishConnection()
	{
		int iResult = listen(listenSocket, SOMAXCONN);
		if (iResult == SOCKET_ERROR)
		{
			PrintSocketError(L"Listen");
			closesocket(listenSocket);
			return;
		}

		// Accept a client socket
		connectSocket = accept(listenSocket, NULL, NULL);
		if (connectSocket == INVALID_SOCKET)
		{
			PrintSocketError(L"Accept");
			closesocket(listenSocket);
			return;
		}

		bool bOptVal = TRUE;
		setsockopt(connectSocket, SOL_SOCKET, TCP_NODELAY, (char *)&bOptVal, sizeof(bool));

		// No longer need server socket
		closesocket(listenSocket);
	}

	// TODO: fast fail if connection has not been made yet.
	// TODO: caller should re-establish connection if failed.
	bool ReceiveData(byte*& bytes, int numBytes)
	{
		int iResult = recv(connectSocket, recvbuf, numBytes, 0);
		if (iResult < 0)
		{
			PrintSocketError(L"Receive");
			closesocket(connectSocket);
			return false;
		}
		else if (iResult == 0)
		{
			// Connection closed.
			iResult = shutdown(connectSocket, SD_SEND);
			if (iResult == SOCKET_ERROR)
			{
				PrintSocketError(L"Shutdown after 0 byte recv");
				closesocket(connectSocket);
			}
			return false;
		}

		memcpy(bytes, recvbuf, iResult);
		return true;
	}

	// TODO: can send data without a recv?
	// TODO: fast fail if connection has not been made yet.
	// TODO: caller should re-establish connection if failed.
	bool SendData(byte* bytes, int len)
	{
		// TODO: Segment packet and send in a loop.
		if (len > DEFAULT_BUFLEN)
		{
			OutputDebugString(L"Error, size is too large.");
			return false;
		}

		int iResult = send(connectSocket, (char*)bytes, len, 0);
		if (iResult == SOCKET_ERROR)
		{
			PrintSocketError(L"Send");
			return false;
		}

		return true;
	}

private:
	// Socket to establish connection.
	SOCKET listenSocket;

	// Socket to send data to connected peer.
	SOCKET connectSocket;

	char recvbuf[DEFAULT_BUFLEN];
};
