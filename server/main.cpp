// -*- C++ -*-
/*
 * main.cpp
 *
 *  Created on: Apr 19, 2012
 *      Author: Ti
 */

#include "Daemon_Server.h"
#include "Time_Value.h"
#include <signal.h>

static void sighandler(int sig_no) { exit(0); } /// for gprof need normal exit

int main(int argc, char *argv[]) {
	srand(Time_Value::gettimeofday().sec() + Time_Value::gettimeofday().usec());
	srandom(Time_Value::gettimeofday().sec() + Time_Value::gettimeofday().usec());
	signal(SIGPIPE, SIG_IGN);
	signal(SIGUSR1, sighandler);

	Daemon_Server::instance()->init(argc, argv);
	Daemon_Server::instance()->start(argc, argv);

	return 0;
}
