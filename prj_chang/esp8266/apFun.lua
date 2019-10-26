--ADCD,1234567890,1981,12e401114,
local localport=6531
local localip="255.255.255.255"
led = 4
gpio.mode(led, gpio.OUTPUT)
udpSocket = net.createUDPSocket()

function udp_send(data)
    udpSocket:send(localport,localip,data)
end
function setUdp()   
    udpSocket:listen(5000)
    udpSocket:on("receive", function(s, data, port, ip)
        localport = port
        localip   = ip
        if string.find(data, "s", 1,1) then
            writFildDat(string.sub(data,2,-1),"data.lua")
            writFildDat("m1","mode.lua")
            node.restart()
        elseif string.find(data, "s", 1,1) then
            writFildDat(string.sub(data,2,-1),"data.lua")
        elseif string.find(data, "g", 1,1) then
            if file.open("data.lua", "r") then
                udpSocket:send(localport,localip,file.readline())
                file.close()
            end            
        else
            print(data)
        end
        
        --print(string.format("received '%s' from %s:%d", data, ip, port))
        --writFildDat(data,"data.lua")
    end)
    port, ip = udpSocket:getaddr()
    tmr.delay(500000)
    print(string.format("local UDP socket address / port: %s:%d", ip, port))
end

function setAp()
    wifi.setmode(wifi.SOFTAP)
    cfg={}
    cfg.ssid="chang"
    cfg.pwd="chang12345"
    wifi.ap.config(cfg)
    --print(wifi.ap.getip())
    
    local t1 = tmr.create()
    t1:alarm(200, tmr.ALARM_AUTO, function()
        if gpio.read(led)==1 then
            gpio.write(led, 0) 
        else
            gpio.write(led, 1) 
        end

    end)
    setUdp()   
end
