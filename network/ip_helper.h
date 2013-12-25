#ifndef MASTER_UTILS_NETWORK_IP_HELPER_H_
#define MASTER_UTILS_NETWORK_IP_HELPER_H_

struct ip_helper
{
	//ips[out]:retrieve the result
	//count[out]: indicate the number of ips
	//is_host_seq[in] : in network byte order or in host byte order
	static int get_local_ip(unsigned int*& ips,int& count,bool is_host_seq);

	//specify if the ip is one of mine
	static int is_local_ip(const char* ip,bool& is_local);
	//translate string to ip in host bit order
	static int atoh(const char* ip,unsigned int& host_ip);
	//translate local host addr to string
	static const char* htoa(unsigned int host_ip);
};

#endif
