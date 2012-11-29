#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include "ip_helper.h"

int ip_helper::get_local_ip(unsigned int*& ips,int& count,bool is_local_ip)
{
	ifconf conf;
	int result = -1;
	// prepare 128 items, enough
    conf.ifc_len = sizeof(ifreq)*128;
	conf.ifc_buf= new char[sizeof(ifreq)*128];

    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    if( sockfd == -1 )
    {
        delete conf.ifc_buf;
        return result;
    }

    // man/7/netdevice
    if(ioctl(sockfd,SIOCGIFCONF,&conf) == -1)
    {
        delete conf.ifc_buf;
        close(sockfd);
        return result;
    }

    int num = conf.ifc_len / sizeof(struct ifreq);
    ifreq *req = conf.ifc_req;
	ips = new unsigned int [num];
    for(int i = 0;i<num; ++i,++req)
    {
		unsigned int ip = ((struct sockaddr_in*)&(req->ifr_addr))->sin_addr.s_addr;
		if(is_local_ip)
		{
			ips[i] = ntohl(ip);
		}
		else
		{
			ips[i] = ip;
		}
	}
    delete conf.ifc_buf;
    close(sockfd);
	return result;
}

int ip_helper::is_local_ip(const char* ip,bool& is_local)
{
	if(!ip)
	{
		is_local = false;
		return 0;
	}
	in_addr the_ip;
	if(inet_aton(ip,&the_ip) != 0)
	{
		is_local = false;
		return 0;
	}

	unsigned int* ips = NULL;
	int ncount = 0;
	if(get_local_ip(ips,ncount,false) == -1)
	{
		return -1;
	}
	for(int i=0;i<ncount;++i)
	{
		if(ips[i] == the_ip.s_addr)
		{
			is_local = true;
			break;
		}
	}
	return 0;
}

int ip_helper::atoh(const char* ip,unsigned int& host_ip)
{
	in_addr the_addr;
	if(inet_aton(ip,&the_addr) != 0)
	{
		return -1;
	}
	host_ip = ntohl(the_addr.s_addr);
	return 0;
}
const char* ip_helper::htoa(unsigned int host_ip)
{
	in_addr addr;
	addr.s_addr = (unsigned long int)htonl(host_ip);
	return inet_ntoa(addr);
}