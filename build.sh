#!/bin/sh

CWD=`cd $(dirname $0);pwd`

WORKDIR_LIST="$CWD/comet $CWD/logic $CWD/router $CWD/logic/job"

function usage()
{
    cat <<EOF
usage:$0 [build|install]
    build:进行编译,但不打包,默认的操作
    install:在build的基础之上,进行打包
EOF

}

function build()
{
    for line in $WORKDIR_LIST
    do
        local proc=`echo $line|awk -F"/" '{ print $NF }'`
        echo "start to build ${proc}"

        (cd $line;go build)
    done
}

function install()
{
    local date=`date +%Y%m%d%H%M%S`
    cp -R $CWD/install $CWD/goim-${date}

    for line in $WORKDIR_LIST
    do
        local proc=`echo $line|awk -F"/" '{ print $NF }'`
        local src_file=$line/$proc
        local dst_file=$CWD/goim-${date}/$proc/bin/

        cp -R $src_file $dst_file
    done
}

op=build

if [ $# -gt 0 ];then
    op=$1
fi

case $op in
    build)
        build
        ;;
    install)
        build
        install
        ;;
    *)
        usage
esac

