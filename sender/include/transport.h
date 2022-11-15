#ifndef __TRANSPORT_H
#define __TRANSPORT_H

#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAX_CACHE 64512

namespace transport
{

class UDPClient
{
public:
	UDPClient()
	{
		bzero( &dst_addr, sizeof( dst_addr ) );
		dst_addr_len = sizeof( dst_addr );
	}

	~UDPClient()
	{
	
	}
	
	const bool init()
	{
		client_fd = socket( AF_INET, SOCK_DGRAM, 0 );
		if( client_fd <= 0 ){
			std::cerr<<"Can not init the Socket !"<<std::endl;
			return false;
		}

		std::cout<<"Init the Udp Socket !"<<std::endl;
		return true;
	}

	void setDstAddr( const std::string& ip, const int port )
	{
		dst_addr.sin_family = AF_INET;
		dst_addr.sin_addr.s_addr = inet_addr( ip.c_str() );
		dst_addr.sin_port = htons( port );
	}

	const int write( const unsigned char* buffer, int size )
	{
		int ret = sendto( client_fd, buffer, size, 0, ( struct sockaddr*)&dst_addr, dst_addr_len );
		if( ret <= 0 ){
			std::cerr<<"send data falied ..."<<ret<<std::endl;
			return false;	
		}
		std::cerr<<"send data succussfully ..."<<std::endl;
		return ret;
	}

	const bool sendOneFrame( const unsigned char* buffer, int size )
	{
		// 1. send the length information 
		unsigned char len[8];
		len[0] = 'L';
		len[1] = 'E';
		len[2] = 'N';
		len[3] = 'G';

		memcpy( &len[4], &size, 4 );
		int ret = sendto( client_fd, len, 8, 0, ( struct sockaddr*)&dst_addr, dst_addr_len );
                if( ret <= 0 ){
                        std::cerr<<"send length falied : "<<ret<<std::endl;
                        return false;
                }

		// 2. 
		int times = size / MAX_CACHE;
		int remain = size % MAX_CACHE;
		
		// 3.
		int j = 0; 
		for( ; j < times; j ++ ){
			ret = sendto( client_fd, &buffer[MAX_CACHE * j], MAX_CACHE, 0, ( struct sockaddr*)&dst_addr, dst_addr_len );
                	if( ret <= 0 ){
                        	std::cerr<<"send data falied : "<<ret<<std::endl;
                        	return false;
                	}
		}

		ret = sendto( client_fd, &buffer[MAX_CACHE * j], remain, 0, ( struct sockaddr*)&dst_addr, dst_addr_len );
                if( ret <= 0 ){
	                std::cerr<<"send data falied : "<<ret<<std::endl;
                        return false;
               	}

		return true;

	}

private:

	int client_fd = -1;

	struct sockaddr_in dst_addr;
	socklen_t dst_addr_len;

};

}

#endif
