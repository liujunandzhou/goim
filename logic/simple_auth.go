package main

import (
	"encoding/json"
	"goim/libs/define"
	"strconv"
	"strings"
)

type SimpleAuth struct {
}

func NewSimpleAuther() *SimpleAuth {

	return &SimpleAuth{}
}

type AuthData struct {
	UserId int64 `json:"userid"`
	RoomId int32 `json:"roomid"`
}

//说明发送json格式的数据
func (this *SimpleAuth) Auth(token string) (userId int64, roomId int32) {

	return this.parseJsonData(token)
}

func (this *SimpleAuth) parseStrData(token string) (userId int64, roomId int32) {

	var err error
	var room int

	parts := strings.SplitN(token, "@", 2)

	if userId, err = strconv.ParseInt(parts[0], 10, 64); err != nil {

		userId = 0
		roomId = define.NoRoom
	}

	roomId = 1

	if len(parts) > 1 {

		if room, err = strconv.Atoi(parts[1]); err == nil {
			roomId = int32(room)
		}
	}

	return

}

func (this *SimpleAuth) parseJsonData(token string) (userId int64, roomId int32) {

	var authData AuthData

	if err := json.Unmarshal([]byte(token), &authData); err != nil {

		userId = 0

		roomId = define.NoRoom

		return
	}

	userId = authData.UserId

	roomId = authData.RoomId

	return
}
