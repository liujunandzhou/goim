/*
 * =====================================================================================
 *
 *       Filename:  packet_coder.cc
 *
 *    Description:  implement the packet_coder.
 *
 *        Version:  1.0
 *        Created:  06/28/2016 05:49:58 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  liujun (qihoo inc.), liujun-xy@360.cn
 *        Company:  qihoo inc.
 *
 * =====================================================================================
 */

#include <string.h>
#include <string>

#include "packet_coder.h"

PacketCoder::PacketCoder()
{
	header=false;
	memset(&packet_header,0,sizeof(packet_header));
}

PacketCoder::~PacketCoder()
{
	header=false;
}

//对数据进行编码,生成的数据直接放到Encode中
std::string PacketCoder::Encode(PacketHeader pkg_header,const char *body,int len)
{
	packet_header=pkg_header;
	header=true;
	pkg_header.pack_size=PACKET_SIZE+len;
	pkg_header.header_size=PACKET_SIZE;

	data_buffer.writeInt32(pkg_header.pack_size);
	data_buffer.writeInt16(pkg_header.header_size);
	data_buffer.writeInt16(pkg_header.ver_size);
	data_buffer.writeInt32(pkg_header.operation_size);
	data_buffer.writeInt32(pkg_header.seqid_size);

	if(len>0)
		data_buffer.writeBytes((uint8_t*)body,len);

	std::string body_str;
	body_str.assign(data_buffer.getData(),data_buffer.getDataLen());

	data_buffer.drainData(PACKET_SIZE);

	return body_str;
}

void PacketCoder::Decode(const char *buffer,int len)
{

	if(len>0)
		data_buffer.writeBytes((uint8_t*)buffer,len);

	if(header==false){

		if((unsigned long)data_buffer.getDataLen()<PACKET_SIZE)
			return;

		header=true;

		packet_header.pack_size=data_buffer.readInt32();
		packet_header.header_size=data_buffer.readInt16();
		packet_header.ver_size=data_buffer.readInt16();
		packet_header.operation_size=data_buffer.readInt32();
		packet_header.seqid_size=data_buffer.readInt32();
	}
}

bool PacketCoder::HasHeader()
{
	return header;
}

void PacketCoder::Reset()
{
	header=false;
	memset(&packet_header,0,sizeof(packet_header));
	data_buffer.clear();
}


PacketHeader& PacketCoder::GetPacketHeader()
{
	return this->packet_header;
}

bool  PacketCoder::GetBody(std::string &body)
{
	body="";

	if(header==false)
		return false;

	if((unsigned long)data_buffer.getDataLen()<(packet_header.pack_size-PACKET_SIZE))
		return false;

	//empty pointer assign,will cause crash
	if(data_buffer.getDataLen()>0)
		body.assign(data_buffer.getData(),data_buffer.getDataLen());

	return true;
}
