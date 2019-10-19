led = 4
gpio.mode(led, gpio.OUTPUT)
gpio.write(led, gpio.HIGH)
local remotPport=5000
local remotIP="255.255.255.255"
local udp_ok = 0
udpSocket = net.createUDPSocket()

function udp_send(data)
    if udp_ok == 1 then
        udpSocket:send(remotPport,remotIP,data)
    end
end
function setUdp()   
    udpSocket:listen(5001)
    udpSocket:on("receive", function(s, data, port, ip)
        --print(data)
	uart.write(0, data)
    end)
    udp_ok = 1
end
 
function setSta()
 
    wifi.setmode(wifi.STATION)
    station_cfg={}
    station_cfg.ssid="carssid"
    station_cfg.pwd="carpassword"
    wifi.sta.config(station_cfg)
    wifi.sta.connect()
    local t1 = tmr.create()
    t1:alarm(500, tmr.ALARM_AUTO, function()
        if wifi.sta.getip() ~= nil then
           -- print(wifi.sta.getip())
--            print("wifiok")
            gpio.write(led,0)
            setUdp()  
            wifi.eventmon.register(wifi.eventmon.STA_DISCONNECTED, function(T)
                 node.restart()
             end)
            t1:unregister()
        else
            if(gpio.read(led)==1)then
                gpio.write(led,0)
            else
                gpio.write(led,1)
            end
        end
    end)
end
