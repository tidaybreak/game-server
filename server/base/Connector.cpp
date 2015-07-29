// -*- C++ -*-
/*
 * Connector.cpp
 *
 *  Created on: May 3, 2012
 *      Author: Root
 */

#include "Connector.h"
#include "comm_func.h"
#include "Lib_Log.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>

Connector::Connector(void) { }

Connector::~Connector(void) { }

int Connector::connect_svc(int connfd) {
	LOG_USER_TRACE("SHOULD NOT HERE");
	return 0;
}

int Connector::connect(const char *ip, int port) {
	int connfd = 0;
	struct sockaddr_in serveraddr;

	if ((connfd = ::socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		LOG_SYS("socket");
		return -1;
	}

	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(port);

	if (::inet_pton(AF_INET, ip, &serveraddr.sin_addr) <= 0) {
		LOG_SYS("inet_pton ip:%s, port:%d", ip, port);
		::close(connfd);
		return -1;
	}

	int buf_size = 10 * 1024 * 1024;
	if (::setsockopt(connfd, SOL_SOCKET, SO_RCVBUF, &buf_size, sizeof(buf_size)) == -1) {
		LOG_SYS("setsockopt SO_REUSEADDR");
		LOG_ABORT();
	}

	if (::setsockopt(connfd, SOL_SOCKET, SO_SNDBUF, &buf_size, sizeof(buf_size)) == -1) {
		LOG_SYS("setsockopt SO_REUSEADDR");
		LOG_ABORT();
	}

	{
		struct timeval timeo = {1, 0};
		socklen_t len = sizeof(timeo);
		::setsockopt(connfd, SOL_SOCKET, SO_SNDTIMEO, &timeo, len);
	}

	{
		if (::connect(connfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0) {
			LOG_SYS("connect ip:%s, port:%d", ip, port);
			::close(connfd);
			return -1;
		}
	}

	set_nonblock(connfd);

	return connect_svc(connfd);
}

