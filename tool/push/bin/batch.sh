#!/bin/sh

CWD=`cd $(dirname $0);pwd`

COUNT=1000

for i in `seq 0 $COUNT`
do
	php $CWD/push.php
done
