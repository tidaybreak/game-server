/*
 * Game_Version.cpp
 *
 *  Created on: Aug 24, 2013
 *      Author: Ti
 */

#include "Game_Version.h"
#include "svn_info.h"
#include <sstream>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <openssl/md5.h>

void get_version_string(std::string &s) {
	std::string datestr = __DATE__;
	std::istringstream iss_date( datestr );
	std::string str_month;
	int day;
	int year;
	iss_date >> str_month >> day >> year;

	int month = 1;
	if     ( str_month == "Jan" ) month = 1;
	else if( str_month == "Feb" ) month = 2;
	else if( str_month == "Mar" ) month = 3;
	else if( str_month == "Apr" ) month = 4;
	else if( str_month == "May" ) month = 5;
	else if( str_month == "Jun" ) month = 6;
	else if( str_month == "Jul" ) month = 7;
	else if( str_month == "Aug" ) month = 8;
	else if( str_month == "Sep" ) month = 9;
	else if( str_month == "Oct" ) month = 10;
	else if( str_month == "Nov" ) month = 11;
	else if( str_month == "Dec" ) month = 12;


	std::stringstream stream;
	stream 	<< REV << std::endl
			<< URL << std::endl
			<< "Build Time: " << year << "-" << month << "-" << day << " " << __TIME__ << std::endl
			<< BUILD << std::endl;
	s = stream.str();
}

void get_version_time(std::string &s) {
	std::string datestr = __DATE__;
	std::istringstream iss_date( datestr );
	std::string str_month;
	int day;
	int year;
	iss_date >> str_month >> day >> year;

	int month = 1;
	if     ( str_month == "Jan" ) month = 1;
	else if( str_month == "Feb" ) month = 2;
	else if( str_month == "Mar" ) month = 3;
	else if( str_month == "Apr" ) month = 4;
	else if( str_month == "May" ) month = 5;
	else if( str_month == "Jun" ) month = 6;
	else if( str_month == "Jul" ) month = 7;
	else if( str_month == "Aug" ) month = 8;
	else if( str_month == "Sep" ) month = 9;
	else if( str_month == "Oct" ) month = 10;
	else if( str_month == "Nov" ) month = 11;
	else if( str_month == "Dec" ) month = 12;


	std::stringstream stream;
	stream 	<< "Build Time: " << year << "-" << month << "-" << day << " " << __TIME__ << std::endl;

	s = stream.str();
}

void get_version_svn_url(std::string &s) {
	std::stringstream stream;
	stream 	<< URL << std::endl;

	s = stream.str();
}

void get_version_svn_ver(std::string &s) {
	std::stringstream stream;
	stream 	<< REV << std::endl;

	s = stream.str();
}

void get_version_build(std::string &s) {
	std::stringstream stream;
	stream 	<< BUILD << std::endl;

	s = stream.str();
}

void get_pack_md5(std::string &s) {
	int fd = 0;
	if ((fd = open("/data/rsync/server/server.tar.gz", O_RDONLY)) < 0) {
		s = "找不到包文件!";
		return ;
	}

	struct stat statbuf;
	if (fstat(fd, &statbuf) < 0) {
		s = "fstat失败!";
		return ;
	}

	void *src = 0;
	if ((src = mmap(0, statbuf.st_size, PROT_READ, MAP_SHARED, fd, 0)) == MAP_FAILED) {
		s = "mmap失败!";
		return ;
	}

    char mine_md5[256 + 1];
    memset(mine_md5, 0x00, 256 + 1);
    const unsigned char *tmp_md5 = MD5((const unsigned char *) src, statbuf.st_size, 0);
    for (uint i = 0; i < 16; i++) {
        sprintf(&mine_md5[i * 2], "%.2x", tmp_md5[i]);
    }

	munmap(src, statbuf.st_size);

	close(fd);

	s.assign(mine_md5);
}

void get_path_md5(std::string &s) {
	char *path_c;
	path_c = getcwd(NULL, 0);
	s.assign(path_c);
}
