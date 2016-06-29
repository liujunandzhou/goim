/*
 * =====================================================================================
 *
 *       Filename:  packet.cc
 *
 *    Description:  implement the packet operation
 *
 *        Version:  1.0
 *        Created:  06/29/2016 10:30:07 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  liujun (qihoo inc.), liujun-xy@360.cn
 *        Company:  qihoo inc.
 *
 * =====================================================================================
 */

#include <boost/assign.hpp>
#include "packet.h"


std::map<int,std::string> packet_operation_map=boost::assign::map_list_of
	(OP_HANDSHAKE,"OP_HANDSHAKE")
	(OP_HANDSHAKE_REPLY,"OP_HANDSHAKE_REPLY")
	(OP_HEARTBEAT,"OP_HEARTBEAT")
	(OP_HEARTBEAT_REPLY,"OP_HEARTBEAT_REPLY")
	(OP_SEND_SMS,"OP_SEND_SMS")
	(OP_SEND_SMS_REPLY,"OP_SEND_SMS_REPLY")
	(OP_DISCONNECT_REPLY,"OP_DISCONNECT_REPLY")
	(OP_AUTH,"OP_AUTH")
	(OP_AUTH_REPLY,"OP_AUTH_REPLY")
	(OP_HANDSHAKE_SID,"OP_HANDSHAKE_SID")
	(OP_HANDSHAKE_SID_REPLY,"OP_HANDSHAKE_SID_REPLY")
	(OP_RAW,"OP_RAW")
	(OP_ROOM_READY,"OP_ROOM_READY")
	(OP_PROTO_READY,"OP_PROTO_READY")
	(OP_PROTO_FINISH,"OP_PROTO_FINISH")
	(OP_TEST,"OP_TEST")
	(OP_TEST_REPLY,"OP_TEST_REPLY")
	;

