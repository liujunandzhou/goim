/*
 * =====================================================================================
 *
 *       Filename:  packet_coder.h
 *
 *    Description:  define the encode/decode the packet operation
 *
 *        Version:  1.0
 *        Created:  06/28/2016 05:17:09 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  liujun (qihoo inc.), liujun-xy@360.cn
 *        Company:  qihoo inc.
 *
 * =====================================================================================
 */

#ifndef __PACKET_CODER_H__
#define __PACKET_CODER_H__

#include "packet.h"
#include "databuffer.h"

using namespace tbnet;

class PacketCoder
{
public:
	PacketCoder();
	virtual ~PacketCoder();

public:
	//获取完整的数据包大小
	std::string Encode(PacketHeader pkg_header,const char *body,int len);
	void Decode(const char *buffer,int len);

public:
	void Reset();
	bool HasHeader();
	PacketHeader &GetPacketHeader();
	bool GetBody(std::string &body);


private:
	PacketHeader packet_header;
	DataBuffer data_buffer;
	bool header;
};

#endif
