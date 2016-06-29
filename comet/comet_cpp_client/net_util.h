#ifndef __NET_UTIL_H__
#define __NET_UTIL_H__

/* 
 *  code from redis 
 *
 *  modified by liujun
 *
 * */

#ifdef __cplusplus
extern "C" {
#endif


#define NET_OK 0
#define NET_ERR -1
#define NET_ERR_LEN 256

int netTcpConnect(char *err, char *addr, int port);

int netTcpNonBlockConnect(char *err, char *addr, int port);

int netUnixConnect(char *err, char *path);

int netUnixNonBlockConnect(char *err, char *path);

int netReadPatial(int fd,char *buf,int count);

int netRead(int fd, char *buf, int count);

int netResolve(char *err, char *host, char *ipbuf);

//创建TCP类型的监听
int netTcpServer(char *err, int port, char *bindaddr,int backlog=511);

//创建Unix类型的监听
int netUnixServer(char *err, char *path, mode_t perm,int backlog=511);

int netTcpAccept(char *err, int serversock, char *ip, int *port);

int netUnixAccept(char *err, int serversock);

int  netWritePatial(int fd,char *buf,int count);

int netWrite(int fd, char *buf, int count);

int netNonBlock(char *err, int fd);

int netTcpNoDelay(char *err, int fd);

int netSetSendBuffer(char *err, int fd, int buffsize);

int netSetRecvBuffer(char *err, int fd, int buffsize);

int netTcpKeepAlive(char *err, int fd);

int netPeerToString(int fd, char *ip, int *port);

#ifdef __cplusplus
}
#endif


#endif
