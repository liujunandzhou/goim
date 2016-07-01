<?php

date_default_timezone_set('PRC');

require_once(dirname(__FILE__)."/util.php");

define('ONE_PUSH','http://127.0.0.1:7172/1/push?uid=%d');

define('MULTIPLE_PUSH','http://127.0.0.1:7172/1/pushs');

define('ROOM_PUSH','http://127.0.0.1:7172/1/push/room?rid=%d');

define('BROADCAST_PUSH','http://127.0.0.1:7172/1/push/all');

function PushOne($userId,$data)
{
	$url=sprintf(ONE_PUSH,$userId);

	$ret=http_post($url,$data,false);

	return $ret;
}

function PushMultiple($userIdList,$data)
{
	$dtarr=array();

	$dtarr["u"]=$userIdList;
	$dtarr["m"]=$data;

	$ret=http_post(MULTIPLE_PUSH,json_encode($dtarr),false);

	return $ret;
}


function PushRoom($roomId,$data)
{
	$url=sprintf(ROOM_PUSH,$roomId);

	$ret=http_post($url,$data,false);

	return $ret;
}

function PushBroadcast($data)
{
	$ret=http_post(BROADCAST_PUSH,$data,false);

	return $ret;
}

function checkResult($notice,$res)
{
	echo "$notice",PHP_EOL;
	var_dump($res);
}

$userId=12345678;

$roomId=15;

$curTime=date("Y-m-d H:i:s");


$messageStruct=array(
	#"message"=>str_repeat("Now is ${curTime},我的名字叫刘军",1000)
	"message"=>"Now is ${curTime},我的名字叫刘军"
);



$data=json_encode($messageStruct);

//$data="当前时间是:${curTime},我的名字叫刘军";

#测试PushOne逻辑
checkResult("PushOne",PushOne($userId,$data));

return;

#测试多人单消息推送
checkResult("PushMultiple",PushMultiple(array($userId),"PushMultiple:".$data));

#测试房间推送
checkResult("PushRoom",PushRoom($roomId,"PushRoom:".$data));

#测试广播推送
checkResult("PushBroadcast",PushBroadcast("PushBroadcast:".$data));
