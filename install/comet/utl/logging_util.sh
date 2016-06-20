#!/bin/sh

GREEN='0;32;1m'

RED='0;41;1m'



function print_info() 
{
  if [ $# -eq 1 ]; then
    echo $1 1>&2
  elif [ $# -eq 2 ]; then
	if [ -t 1 ]; then
		echo -e '\e['$2$1'\e[0m' 1>&2
	else
		echo $1 1>&2
	fi
  fi
}

function succ_or_die() 
{
  invoke "$1" $2
  if [ $? -ne 0 ]; then
    print_info "Failed: [$1] " $RED
    exit -1
  fi
}

function invoke() 
{
  if [ $# -eq 1 ]; then
    print_info "$1" $GREEN
    $1;
  fi
  if [ $# -eq 2 ]; then
    print_info "$1 > $2 2>&1" $GREEN
    $1 1>>$2 2>&1;
  fi
}


function notice()
{
    local date=`date +%Y-%m-%d-%H-%M-%S`
    echo "$date: $*"
}

function notice_stderr()
{
    local date=`date +%Y-%m-%d-%H-%M-%S`
    echo "$date: $*" 1>&2
}

function info_log()
{
	local date=`date +%Y-%m-%d-%H-%M-%S`
	print_info "$date:$1" $GREEN
}

function error_log()
{
	local date=`date +%Y-%m-%d-%H-%M-%S`
	print_info "$date:$1" $RED
}


function clear_log()
{
    if [ -d $1 ];then
        /usr/bin/lockf -t 0 /tmp/clean.lock /usr/bin/find $1  -mtime +5 -type f|/usr/bin/xargs -i rm -rf {} >/dev/null 2>&1
        return
    fi  

    notice_stderr "$1 is not a dir"
}


function file_log()
{
	    local date=`date +%Y-%m-%d-%H-%M-%S`
		echo "$date: $1" >> $2
}
