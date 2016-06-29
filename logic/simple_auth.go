package main

import (
	"goim/libs/define"
	"strconv"
	"strings"
)

import ()

type SimpleAuth struct {
}

func NewSimpleAuther() *SimpleAuth {

	return &SimpleAuth{}
}

func (this *SimpleAuth) Auth(token string) (userId int64, roomId int32) {

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
