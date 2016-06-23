package main

import "fmt"
import "os"
import "strconv"
import "net/rpc"

func usage() {
	fmt.Printf("usage:%s userid\n", os.Args[0])
	os.Exit(1)
}

func debugRootCount(userId int64) {

	var client *rpc.Client
	var err error
	var counter map[int32]int32

	if client, err = getRouterByUID(userId); err != nil {
		fmt.Println("getRouterByUID failed:", err)
		return
	}

	if counter, err = allRoomCount(client); err != nil {
		fmt.Println("allRoomCount failed:", err)
		return
	}

	for roomId, count := range counter {
		fmt.Printf("rootid=%d count=%d\n", roomId, count)
	}
}

func debugServerCount(userId int64) {

	var client *rpc.Client
	var err error
	var counter map[int32]int32

	if client, err = getRouterByUID(userId); err != nil {
		fmt.Println("getRouterByUID failed")
		return
	}

	if counter, err = allServerCount(client); err != nil {
		fmt.Println("allServerCount failed:", err)
		return
	}

	for serverId, count := range counter {
		fmt.Printf("serverId=%d count=%d\n", serverId, count)
	}
}

func debuggenSubKey(userId int64) {

	res := genSubKey(userId)

	for server, subkeys := range res {

		fmt.Println(server, subkeys)
	}
}

func debugAllInfo() {

}

func main() {

	var (
		err error
	)

	if len(os.Args) < 2 {
		usage()
	}

	//初始化配置文件
	if err = InitConfig(); err != nil {
		fmt.Println("InitConfig failed:%s", err)
		return
	}

	if Conf.Debug == true {

		for key, value := range Conf.RouterRPCAddrs {
			fmt.Printf("serverid=%s addr=%s\n", key, value)
		}

	}

	if err = InitRouter(); err != nil {
		fmt.Printf("InitRouter failed:%s\n", err)
		return
	}

	var userId int64
	var errConv error

	if userId, errConv = strconv.ParseInt(os.Args[1], 10, 64); errConv != nil {
		fmt.Println("not valid userid")
		return
	}

	fmt.Println("start to run debugRootCount")
	debugRootCount(userId)

	fmt.Println("start to run debugServerCount")
	debugServerCount(userId)

	fmt.Println("start to run debuggenSubKeys")
	debuggenSubKey(userId)
}
