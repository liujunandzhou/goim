/*
 * =====================================================================================
 *
 *       Filename:  simple_packet_handler.h
 *
 *    Description:  implement the simple packet handler
 *
 *        Version:  1.0
 *        Created:  06/28/2016 07:39:02 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  liujun (qihoo inc.), liujun-xy@360.cn
 *        Company:  qihoo inc.
 *
 * =====================================================================================
 */

#ifndef __SIMPLE_PACKET_HANDLER_H__
#define __SIMPLE_PACKET_HANDLER_H__

#include <boost/pool/object_pool.hpp>

#include "thread.h"
#include "packet_inf.h"
#include "packet_coder.h"

//for simple,the member function may not thread_safe

class SimplePacketHandler: public PacketHandler
{

public:
	SimplePacketHandler();
	virtual ~SimplePacketHandler();

public:
	void SetHostInfo(std::string ip,int port);
	void SetFd(int fd);

public:
	Packet *AllocPacket();
	void FreePacket(Packet *pkt);

public:
	Packet* RecvPacket();
	bool SendPacket(Packet &pkt);

private:
	bool CheckConnection();

private:
	//以为packet_size为单位接收消息
	char buffer[PACKET_SIZE];

	thread_wrapper::Mutex mutex;

	//mutex for check

	//消息的编解码器
	PacketCoder read_coder;
	PacketCoder write_coder;

	//关于主机ip相关的信息
	std::string ip;
	int port;
	int fd;

private:
	boost::object_pool<Packet> packet_pool;
};

#endif
