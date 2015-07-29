/*
 * Game_Version.h
 *
 *  Created on: Aug 24, 2013
 *      Author: Ti
 */

#ifndef GAME_VERSION_H_
#define GAME_VERSION_H_

#include <string>

const char *get_svnversion(void);

void get_version_string(std::string &s);

void get_version_time(std::string &s);

void get_version_svn_url(std::string &s);

void get_version_svn_ver(std::string &s);

void get_version_build(std::string &s);

void get_pack_md5(std::string &s);
void get_path_md5(std::string &s);

#endif /* GAME_VERSION_H_ */
