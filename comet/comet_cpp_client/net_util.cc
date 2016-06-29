#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <netdb.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>

#include "net_util.h"

//用来设置err code
static void netSetError(char *err, const char *fmt, ...)
{
    va_list ap;

    if (!err) return;
    va_start(ap, fmt);
    vsnprintf(err, NET_ERR_LEN, fmt, ap);
    va_end(ap);
}

//用来设置非阻塞
int netNonBlock(char *err, int fd)
{
    int flags;

    /* Set the socket nonblocking.
     * Note that fcntl(2) for F_GETFL and F_SETFL can't be
     * interrupted by a signal. */
    if ((flags = fcntl(fd, F_GETFL)) == -1) {
        netSetError(err, "fcntl(F_GETFL): %s", strerror(errno));
        return NET_ERR;
    }
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
        netSetError(err, "fcntl(F_SETFL,O_NONBLOCK): %s", strerror(errno));
        return NET_ERR;
    }
    return NET_OK;
}

//用来设置NoDelay
int netTcpNoDelay(char *err, int fd)
{
    int yes = 1;
    if (setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &yes, sizeof(yes)) == -1)
    {
        netSetError(err, "setsockopt TCP_NODELAY: %s", strerror(errno));
        return NET_ERR;
    }
    return NET_OK;
}

//用来设置buffer的大小
int netSetSendBuffer(char *err, int fd, int buffsize)
{
    if (setsockopt(fd, SOL_SOCKET, SO_SNDBUF, &buffsize, sizeof(buffsize)) == -1)
    {
        netSetError(err, "setsockopt SO_SNDBUF: %s", strerror(errno));
        return NET_ERR;
    }
    return NET_OK;
}

//用来设置buffer的大小
int netSetRecvBuffer(char *err, int fd, int buffsize)
{
    if (setsockopt(fd, SOL_SOCKET, SO_RCVBUF, &buffsize, sizeof(buffsize)) == -1)
    {
        netSetError(err, "setsockopt SO_RCVBUF: %s", strerror(errno));
        return NET_ERR;
    }
    return NET_OK;
}

//用来设置socket的Keep Alive,来解决异常断开问题
int netTcpKeepAlive(char *err, int fd)
{
    int yes = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, &yes, sizeof(yes)) == -1) {
        netSetError(err, "setsockopt SO_KEEPALIVE: %s", strerror(errno));
        return NET_ERR;
    }
    return NET_OK;
}

//用来将host解析为ip到ipbuf中
int netResolve(char *err, char *host, char *ipbuf)
{
    struct sockaddr_in sa;

    sa.sin_family = AF_INET;
    if (inet_aton(host, &sa.sin_addr) == 0) {
        struct hostent *he;

        he = gethostbyname(host);
        if (he == NULL) {
            netSetError(err, "can't resolve: %s", host);
            return NET_ERR;
        }
        memcpy(&sa.sin_addr, he->h_addr, sizeof(struct in_addr));
    }
    strcpy(ipbuf,inet_ntoa(sa.sin_addr));
    return NET_OK;
}

//用来设置具体的domain,来创建一个socket
static int netCreateSocket(char *err, int domain) {
    int s, on = 1;
    if ((s = socket(domain, SOCK_STREAM, 0)) == -1) {
        netSetError(err, "creating socket: %s", strerror(errno));
        return NET_ERR;
    }
    /* Make sure connection-intensive things like the redis benckmark
     * will be able to close/open sockets a zillion of times */
    if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1) {
        netSetError(err, "setsockopt SO_REUSEADDR: %s", strerror(errno));
        return NET_ERR;
    }
    return s;
}

#define NET_CONNECT_NONE 0
#define NET_CONNECT_NONBLOCK 1

static int netTcpGenericConnect(char *err, char *addr, int port, int flags)
{
    int s;
    struct sockaddr_in sa;

    if ((s = netCreateSocket(err,AF_INET)) == NET_ERR)
        return NET_ERR;

    sa.sin_family = AF_INET;
    sa.sin_port = htons(port);
    if (inet_aton(addr, &sa.sin_addr) == 0) {
        struct hostent *he;

        he = gethostbyname(addr);
        if (he == NULL) {
            netSetError(err, "can't resolve: %s", addr);
            close(s);
            return NET_ERR;
        }
        memcpy(&sa.sin_addr, he->h_addr, sizeof(struct in_addr));
    }
    if (flags & NET_CONNECT_NONBLOCK) {
        if (netNonBlock(err,s) != NET_OK)
            return NET_ERR;
    }
    if (connect(s, (struct sockaddr*)&sa, sizeof(sa)) == -1) {
        if (errno == EINPROGRESS &&
            flags & NET_CONNECT_NONBLOCK)
            return s;

        netSetError(err, "connect: %s", strerror(errno));

        close(s);

        return NET_ERR;
    }
    return s;
}

//使用阻塞连接方式
int netTcpConnect(char *err, char *addr, int port)
{
    return netTcpGenericConnect(err,addr,port,NET_CONNECT_NONE);
}

//使用非阻塞连接方式
int netTcpNonBlockConnect(char *err, char *addr, int port)
{
    return netTcpGenericConnect(err,addr,port,NET_CONNECT_NONBLOCK);
}

//使用Unix 套接字进行连接
static int netUnixGenericConnect(char *err, char *path, int flags)
{
    int s;
    struct sockaddr_un sa;

    if ((s = netCreateSocket(err,AF_LOCAL)) == NET_ERR)
        return NET_ERR;

    sa.sun_family = AF_LOCAL;
    strncpy(sa.sun_path,path,sizeof(sa.sun_path)-1);
    if (flags & NET_CONNECT_NONBLOCK) {
        if (netNonBlock(err,s) != NET_OK)
            return NET_ERR;
    }
    if (connect(s,(struct sockaddr*)&sa,sizeof(sa)) == -1) {
        if (errno == EINPROGRESS &&
            flags & NET_CONNECT_NONBLOCK)
            return s;

        netSetError(err, "connect: %s", strerror(errno));
        close(s);
        return NET_ERR;
    }
    return s;
}

//使用默认连接方式
int netUnixConnect(char *err, char *path)
{
    return netUnixGenericConnect(err,path,NET_CONNECT_NONE);
}

//使用非阻塞方式创建unix socket
int netUnixNonBlockConnect(char *err, char *path)
{
    return netUnixGenericConnect(err,path,NET_CONNECT_NONBLOCK);
}

/* Like read(2) but make sure 'count' is read before to return
 * (unless error or EOF condition is encountered) */

//读取count的数目,除非失败或者连接断开
int netRead(int fd, char *buf, int count)
{
    int nread, totlen = 0;
    while(totlen != count) {
        nread = read(fd,buf,count-totlen);
        if (nread == 0) return totlen;

        if (nread == -1) {
			/*
			if(errno == EAGAIN || errno == EINTR)
				continue;
			*/
			return -1;
		}

        totlen += nread;
        buf += nread;
    }
    return totlen;
}

//读取局部数据
int netReadPatial(int fd,char *buf,int count)
{
	return read(fd,buf,count);
}

/* Like write(2) but make sure 'count' is read before to return
 * (unless error is encountered) */

//写count个字节数据,除非失败或者连接断开,保留原作者的意图,将异常逻辑交给用户判断
int netWrite(int fd, char *buf, int count)
{
    int nwritten, totlen = 0;
    while(totlen != count) {
        nwritten = write(fd,buf,count-totlen);
        if (nwritten == 0) return totlen;
        if (nwritten == -1) {

			/*
			if(errno == EAGAIN || errno == EINTR)
				continue;
			*/

			return -1;
		}

        totlen += nwritten;
        buf += nwritten;
    }
    return totlen;
}

//读取局部数据
int netWritePatial(int fd,char *buf,int count)
{
	return write(fd,buf,count);
}

//监听套接字
static int netListen(char *err, int s, struct sockaddr *sa, socklen_t len,int backlog=511) 
{
    if (bind(s,sa,len) == -1) {
        netSetError(err, "bind: %s", strerror(errno));
        close(s);
        return NET_ERR;
    }
    /* Use a backlog of 512 entries. We pass 511 to the listen() call because
     * the kernel does: backlogsize = roundup_pow_of_two(backlogsize + 1);
     * which will thus give us a backlog of 512 entries */
    if (listen(s, backlog) == -1) {
        netSetError(err, "listen: %s", strerror(errno));
        close(s);
        return NET_ERR;
    }
    return NET_OK;
}

//使用tcp类型的server,对于监听所有的机器,直接传递NULL
int netTcpServer(char *err, int port, char *bindaddr,int backlog)
{
    int s;
    struct sockaddr_in sa;

    if ((s = netCreateSocket(err,AF_INET)) == NET_ERR)
        return NET_ERR;

    memset(&sa,0,sizeof(sa));
    sa.sin_family = AF_INET;
    sa.sin_port = htons(port);
    sa.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bindaddr && inet_aton(bindaddr, &sa.sin_addr) == 0) {
        netSetError(err, "invalid bind address");
        close(s);
        return NET_ERR;
    }

	//直接进行监听
    if (netListen(err,s,(struct sockaddr*)&sa,sizeof(sa),backlog) == NET_ERR)
        return NET_ERR;
    return s;
}

//使用unix类型的server
int netUnixServer(char *err, char *path, mode_t perm,int backlog)
{
    int s;
    struct sockaddr_un sa;

    if ((s = netCreateSocket(err,AF_LOCAL)) == NET_ERR)
        return NET_ERR;

    memset(&sa,0,sizeof(sa));
    sa.sun_family = AF_LOCAL;
    strncpy(sa.sun_path,path,sizeof(sa.sun_path)-1);
    if (netListen(err,s,(struct sockaddr*)&sa,sizeof(sa),backlog) == NET_ERR)
        return NET_ERR;
    if (perm)
        chmod(sa.sun_path, perm);
    return s;
}

//通用的获取监听的连接
static int netGenericAccept(char *err, int s, struct sockaddr *sa, socklen_t *len) 
{
    int fd;
    while(1) {
        fd = accept(s,sa,len);
        if (fd == -1) {
            if (errno == EINTR)
                continue;
            else {
                netSetError(err, "accept: %s", strerror(errno));
                return NET_ERR;
            }
        }
        break;
    }
    return fd;
}

//TCP类型的accept
int netTcpAccept(char *err, int s, char *ip, int *port) 
{
    int fd;
    struct sockaddr_in sa;
    socklen_t salen = sizeof(sa);
    if ((fd = netGenericAccept(err,s,(struct sockaddr*)&sa,&salen)) == NET_ERR)
        return NET_ERR;

    if (ip) strcpy(ip,inet_ntoa(sa.sin_addr));
    if (port) *port = ntohs(sa.sin_port);
    return fd;
}

//使用unix类型的accept
int netUnixAccept(char *err, int s) 
{
    int fd;
    struct sockaddr_un sa;
    socklen_t salen = sizeof(sa);
    if ((fd = netGenericAccept(err,s,(struct sockaddr*)&sa,&salen)) == NET_ERR)
        return NET_ERR;

    return fd;
}

//通过socket,获取ip与端口
int netPeerToString(int fd, char *ip, int *port) 
{
    struct sockaddr_in sa;
    socklen_t salen = sizeof(sa);

    if (getpeername(fd,(struct sockaddr*)&sa,&salen) == -1) {
        *port = 0;
        ip[0] = '?';
        ip[1] = '\0';
        return -1;
    }
    if (ip) strcpy(ip,inet_ntoa(sa.sin_addr));
    if (port) *port = ntohs(sa.sin_port);
    return 0;
}

//通过socket,获取ip与端口号
int netSockName(int fd, char *ip, int *port) 
{
    struct sockaddr_in sa;
    socklen_t salen = sizeof(sa);

    if (getsockname(fd,(struct sockaddr*)&sa,&salen) == -1) {
        *port = 0;
        ip[0] = '?';
        ip[1] = '\0';
        return -1;
    }
    if (ip) strcpy(ip,inet_ntoa(sa.sin_addr));
    if (port) *port = ntohs(sa.sin_port);
    return 0;
}
