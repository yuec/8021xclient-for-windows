/* File: ip.c
 * ----------
 *
 */

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#ifdef WIN32
#include <winsock2.h>
#else
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

void GetIpFromDevice(uint8_t ip[4], const char DeviceName[])
{
#if defined(WIN32)
	struct hostent* hn = NULL;
	char HostName[255];

	gethostname(HostName, sizeof(HostName));	// 获得本机主机名.   

	hn = gethostbyname(HostName);	//根据本机主机名得到本机ip   
	memcpy(ip, ((struct in_addr *)hn->h_addr_list[0]), 4);
#else
	int fd;
	struct ifreq ifr;

	assert(strlen(DeviceName) <= IFNAMSIZ);

	fd = socket(AF_INET, SOCK_DGRAM, 0);
	assert(fd>0);

	strncpy(ifr.ifr_name, DeviceName, IFNAMSIZ);
	ifr.ifr_addr.sa_family = AF_INET;
	if (ioctl(fd, SIOCGIFADDR, &ifr) == 0)
	{
		struct sockaddr_in *p = (void*) &(ifr.ifr_addr);
		memcpy(ip, &(p->sin_addr), 4);
	}
	else
	{
		// 查询不到IP时默认填零处理
		memset(ip, 0x00, 4);
	}

	close(fd);
#endif
}

