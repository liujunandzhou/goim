package main

import "flag"

import "github.com/Terry-Mao/goconf"

var (
	_gconf   *goconf.Config
	Conf     *Config
	confFile string
)

func init() {

	flag.StringVar(&confFile, "c", "monitor.conf", "monitor的基本配置,跟logic差不多,用来查看router基本信息")
}

type Config struct {
	Debug          bool              `goconf:"base:debug"`
	RouterRPCAddrs map[string]string `-`
}

func NewConfig() *Config {
	return &Config{
		Debug:          false,
		RouterRPCAddrs: make(map[string]string),
	}
}

func InitConfig() (err error) {

	Conf = NewConfig()

	_gconf = goconf.New()

	if err = _gconf.Parse(confFile); err != nil {
		return err
	}

	if err := _gconf.Unmarshal(Conf); err != nil {
		return err
	}

	for _, serverid := range _gconf.Get("router.addrs").Keys() {
		addr, err := _gconf.Get("router.addrs").String(serverid)

		if err != nil {
			return err
		}

		Conf.RouterRPCAddrs[serverid] = addr
	}

	return nil
}
