/*
 * =====================================================================================
 *
 *       Filename:  simple_packet_handler.cc
 *
 *    Description:  implement the simple packet handler
 *
 *        Version:  1.0
 *        Created:  06/28/2016 07:52:19 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  liujun (qihoo inc.), liujun-xy@360.cn
 *        Company:  qihoo inc.
 *
 * =====================================================================================
 */

#include <assert.h>

#include "log.h"
#include "net_util.h"
#include "simple_packet_handler.h"

SimplePacketHandler::SimplePacketHandler()
{
	this->ip="127.0.0.1";
	this->port=8080;
	this->fd=-1;

	//清空缓冲区
	memset(buffer,0,sizeof(buffer));
}

SimplePacketHandler::~SimplePacketHandler()
{
	if(this->fd!=-1){
		close(this->fd);
		this->fd=-1;
	}
}


void SimplePacketHandler::SetHostInfo(std::string ip,int port)
{
	this->ip=ip;
	this->port=port;
}

void SimplePacketHandler::SetFd(int fd)
{
	assert(fd>0);
	this->fd=fd;
}

bool SimplePacketHandler::CheckConnection()
{
	char err_buffer[NET_ERR_LEN];
	if(this->fd==-1) {
		thread_wrapper::Locking lock(&mutex);

		if(this->fd==-1){

		int temp_fd=netTcpConnect(err_buffer,(char*)ip.c_str(),port);

		if(temp_fd==NET_ERR){
			return false;
		}

		this->fd=temp_fd;

		}
	}

	return true;
}

//block read a packet,return NULL if fail
Packet* SimplePacketHandler::RecvPacket()
{
	read_coder.Reset();

	if(CheckConnection()==false)
		return NULL;

	int rd_size=netRead(this->fd,buffer,sizeof(buffer));

	log_info("netRead rd_size=%d",rd_size);

	if(rd_size==NET_ERR || rd_size==0 || rd_size!=sizeof(buffer))
		return NULL;

	//读取头部
	read_coder.Decode(buffer,rd_size);

	log_info("read_coder.HasHeader=%d",read_coder.HasHeader());

	 int body_len=(int)(read_coder.GetPacketHeader().pack_size-PACKET_SIZE);

	 while(body_len>0){

		 rd_size=netReadPatial(this->fd,buffer,sizeof(buffer));

		 if(rd_size==NET_ERR || rd_size==0){

			 log_info("netReadPatial failed:");
			 return NULL;
	 	 }

		 read_coder.Decode(buffer,rd_size);

		 body_len=body_len-rd_size;
	 }

	 Packet *pkt=NULL;

	 do{
		 pkt=AllocPacket();

		 if(pkt==NULL)
			 break;

	 	pkt->packet_header=read_coder.GetPacketHeader();

		if(read_coder.GetBody(pkt->body)==false){

			log_info("GetBody failed,not valid package");

			FreePacket(pkt);

			pkt=NULL;

			break;
		}

	 }while(0);

	 return pkt;
}

// send a package, return true if success,else fail
bool SimplePacketHandler::SendPacket(Packet &pkt)
{
	//reset the buffer
	write_coder.Reset();

	if(CheckConnection()==false)
		return false;

	std::string str=write_coder.Encode(pkt.packet_header,pkt.body.c_str(),pkt.body.length());

	int wr_size=netWrite(this->fd,(char*)str.c_str(),str.length());

	if(wr_size!=(int)str.length()){
		log_info("netWrite failed:",wr_size);
		return false;
	}

	return true;
}

Packet* SimplePacketHandler::AllocPacket()
{
	Packet *pkt=packet_pool.construct();

	pkt->Reset();

	return pkt;
}

void SimplePacketHandler::FreePacket(Packet *pkt)
{
	packet_pool.destroy(pkt);
}

