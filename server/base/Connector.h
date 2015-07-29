// -*- C++ -*-
/*
 * Connector.h
 *
 *  Created on: May 3, 2012
 *      Author: Root
 */

#ifndef CONNECTOR_H_
#define CONNECTOR_H_

class Connector {
public:
	Connector(void);
	virtual ~Connector(void);

	int connect(const char *ip, int port);
	virtual int connect_svc(int connfd);
};

#endif /* CONNECTOR_H_ */
