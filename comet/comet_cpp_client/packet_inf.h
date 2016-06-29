/*
 * =====================================================================================
 *
 *       Filename:  packet_inf.h
 *
 *    Description:  define the basic packet operation
 *
 *        Version:  1.0
 *        Created:  06/28/2016 07:28:46 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  liujun (qihoo inc.), liujun-xy@360.cn
 *        Company:  qihoo inc.
 *
 * =====================================================================================
 */

#ifndef __PACKET_INF_H__
#define __PACKET_INF_H__

#include <string.h>
#include <string>

#include "packet.h"
#include "log.h"

// typedef struct Packet
// {
// 		PacketHeader packet_header;
// 		char [1]; //this makes every time, i have to alloc a memory
//
// }Packet;

typedef struct Packet{

public:
	PacketHeader packet_header;
	std::string body;

	//Do not call memset(&Packet,0,sizeof(Packet)),because there is  class object in the sturct,which will cause memory crash
	//call Reset instead
	void Reset()
	{
		memset(&packet_header,0,sizeof(PacketHeader));
		body.clear();
	}

	void DebugPacket()
	{
		log_info("[pack_size:%d]"
				"[header_size:%d]"
				"[ver_size:%d]"
				"[operation_size:%s]"
				"[seqid_size:%d]"
				"[body:%s]",
				packet_header.pack_size,
				packet_header.header_size,
				packet_header.ver_size,
				packet_operation_map[packet_header.operation_size].c_str(),
				packet_header.seqid_size,
				body.c_str()
				);
	}

}Packet;

class PacketHandler
{
public:
	virtual  Packet* RecvPacket()=0;
	virtual  bool SendPacket(Packet &pkt)=0;
};

#endif
