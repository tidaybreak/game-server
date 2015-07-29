// -*- C++ -*-
/*
 * Acceptor.cpp
 *
 *  Created on: Apr 24, 2012
 *      Author: Root
 */

#include "Acceptor.h"
#include "comm_func.h"
#include "Lib_Log.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
#include <arpa/inet.h>
#include <cstring>
#include <cstdlib>

Acceptor::Acceptor(void)
: port_(0),
  listenfd_(0)
{ }

Acceptor::~Acceptor(void) { }

void Acceptor::set(int port) {
	port_ = port;
}

int Acceptor::init(void) {
	return 0;
}

int Acceptor::fini(void) {
	LOG_DEBUG("close listenfd = %d", listenfd_);
	::close(listenfd_);
	return 0;
}

void Acceptor::server_listen(void) {
	struct sockaddr_in serveraddr;

	if ((listenfd_ = ::socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		LOG_SYS("socket");
		LOG_ABORT();
	}

	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(port_);

	int flag = 1;
	if (::setsockopt(listenfd_, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag)) == -1) {
		LOG_SYS("setsockopt SO_REUSEADDR");
		LOG_ABORT();
	}

	int buf_size = 10 * 1024 * 1024;
	if (::setsockopt(listenfd_, SOL_SOCKET, SO_RCVBUF, &buf_size, sizeof(buf_size)) == -1) {
		LOG_SYS("setsockopt SO_REUSEADDR");
		LOG_ABORT();
	}

	if (::setsockopt(listenfd_, SOL_SOCKET, SO_SNDBUF, &buf_size, sizeof(buf_size)) == -1) {
		LOG_SYS("setsockopt SO_REUSEADDR");
		LOG_ABORT();
	}

	struct timeval timeout = {1, 0};//0.1s
	if (::setsockopt(listenfd_, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout)) == -1) {
		LOG_SYS("setsockopt SO_REUSEADDR");
		LOG_ABORT();
	}

	if (::bind(listenfd_, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) == -1) {
		LOG_SYS("bind");
		LOG_ABORT();
	}
	if (::listen(listenfd_, 1024) == -1) {
		LOG_SYS("listen");
		LOG_ABORT();
	}

	LOG_DEBUG("listened at %d", port_);
}

void Acceptor::server_accept(void) {
	int connfd = 0;
	struct sockaddr_in client_addr;
	socklen_t addr_len = 0;
	//char client_ip[128];

	timespec pause_ts;
	pause_ts.tv_sec = 1;
	pause_ts.tv_nsec = 0;

	while (1) {
		memset(&client_addr, 0, sizeof(client_addr));
		addr_len = sizeof(client_addr);
		if ((connfd = ::accept4(listenfd_, (struct sockaddr *) &client_addr, &addr_len, SOCK_NONBLOCK | SOCK_CLOEXEC)) == -1) {
			LOG_SYS("accept");

			if (errno == EMFILE)
	    		::nanosleep (&pause_ts, 0);

			continue;
		}
		struct linger so_linger;
		so_linger.l_onoff = 1;
		so_linger.l_linger = 0;
		if (::setsockopt(connfd, SOL_SOCKET, SO_LINGER, &so_linger, sizeof(so_linger)) < 0) {
			LOG_SYS("setsockopt linger");
			continue;
		}

		// turn on/off TCP Nagel algorithm
		int nodelay_on=1;
		if (::setsockopt(connfd, IPPROTO_TCP, TCP_NODELAY, &nodelay_on, sizeof(nodelay_on)) < 0) {
			LOG_SYS("setsockopt nodelay");
		}

		/**
		memset(client_ip, 0, sizeof(client_ip));
		if (::inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, sizeof(client_ip)) == NULL) {
			LOG_SYS("inet_ntop");
			continue;
		}

		LOG_DEBUG("connfd:%d, listen port:[%d] client:[%s:%d]", connfd, port_, client_ip, ntohs(client_addr.sin_port));
		*/

		accept_svc(connfd);
	}
}

int Acceptor::accept_svc(int connfd) {
	LOG_USER_TRACE("SHOULD NOT HERE");
	return 0;
}


void Acceptor::run_handler(void) {
	//LOG_DEBUG("start acceptor");
	server_listen();
	server_accept();
}

void Acceptor::exit_handler(void) {
	fini();
}

int Acceptor::get_port(void) {
	return port_;
}
