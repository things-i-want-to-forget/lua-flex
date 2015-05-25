#ifndef MSOCKAPI_H
#define MSOCKAPI_H

#include <WinSock2.h>
#include <Windows.h>
#include <type_traits>
#include <ws2tcpip.h>

class MSockBuffer;
class MSockAPI
{
public:
	MSockAPI() { _connected = false; };

	void Connect(void)
	{
		addrinfo hints, *result;
		WSADATA wsaData;
		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;

		WSAStartup(MAKEWORD(2, 2), &wsaData);

		// Resolve the server address and port
		getaddrinfo("127.0.0.1", "54321", &hints, &result);

		sock = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

#ifdef AIM_FLEX_OVERLAY

		int err = bind(sock, result->ai_addr, result->ai_addrlen);

		freeaddrinfo(result);
		if (err != INVALID_SOCKET)
		{
			listen(sock, 2);
			SOCKET serv = sock;
			sock = accept(serv, NULL, NULL);
			_connected = sock != INVALID_SOCKET;
		}

#else

		int err = connect(sock, result->ai_addr, result->ai_addrlen);
		freeaddrinfo(result);

		_connected = err != SOCKET_ERROR && sock != INVALID_SOCKET;

#endif
	}

	bool connected(void)
	{
		return _connected;
	}

	template <typename t>
	bool read(t *val, unsigned long len = sizeof(t))
	{
		DWORD read = ::recv(sock, (char *)val, len, MSG_PEEK);
		if (read == len)
		{
			::recv(sock, (char *)val, len, 0);
			return true;
		}
		return false;
	}

	void send(MSockBuffer *buf);

	void write(MSockBuffer &b)
	{
		send(&b);
	}
	void write(MSockBuffer *b)
	{
		send(b);
	}

	template <typename t>
	void write(t val, unsigned long len = sizeof(t))
	{
		::send(sock, (const char *)&val, len, 0);
	}

	template <typename t>
	void write(t *val, unsigned long len)
	{
		::send(sock, (const char *)val, len, 0);
	}

public:

	bool _connected;
	SOCKET sock;

};

#endif 