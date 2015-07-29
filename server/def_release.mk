server_CFLAGS=-Wall -Wno-reorder -O1 -g3 -fmessage-length=0 -std=c++0x -mtune=core2 -rdynamic
server_CXXFLAGS=-Wall -Wno-reorder -O1 -g3 -fmessage-length=0 -std=c++0x -mtune=core2 -rdynamic
DEFS =-D__GXX_EXPERIMENTAL_CXX0X__

CXX = ccache g++
CC = ccache gcc

server_LDFLAGS=-lpthread -lmongoclient -lmysqlcppconn -lboost_thread-mt -lboost_regex -lboost_filesystem -lboost_program_options -ljsoncpp -lz -lcrypto -lcurl

AUTOMAKE_OPTIONS=subdir-objects
