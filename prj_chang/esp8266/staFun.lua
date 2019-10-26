--ADCD,1234567890,1981,12e401114,
require("fun")
led = 4
gpio.mode(led, gpio.OUTPUT)
local wifiName,wifiPwssword,bigName,bigPwssword = gatLocalConnectData("data.lua")
local host =  "www.bigiot.net"
local port =  8181
local TCP_OBJ = nil
local cu = net.createConnection(net.TCP)
local masterID = "U1673"
function  tcp_send(data)
    ok, stoped = pcall(sjson.encode, {M="say",ID=masterID,C=data})
    cu:send( stoped.."\n" ) 
end

function creatConnectTCP() 
    cu:connect(port, host)
    cu:on("receive", function(cu, c) 
       -- print(c)
        r = sjson.decode(c) 
        if r.M == "say" then
            masterID=r.ID
            print(r.C)          
        elseif r.M == "WELCOME TO BIGIOT" then
            tmr.delay(500000)
            print("netok")         
            cu:send([[{"M":"checkout","ID":"]]..bigName..[[","K":"]]..bigPwssword..[["}]].."\n")  
            cu:send([[{"M":"checkin","ID":"]]..bigName..[[","K":"]]..bigPwssword..[["}]].."\n")         
        elseif r.M == "checkinok" then
            tmr.delay(500000)
            print("loginok")
            gpio.write(led, 0)
        end
    end)
    
    local t2 = tmr.create()
    t2:alarm(30000, tmr.ALARM_AUTO, function()
        cu:send([[{"M":"beat"}]].."\n")
    end)
    
    cu:on('disconnection',function(scu)
        node.restart()
        cu = nil        
        t2:unregister()
    end)
    
end

function setSta()
 
    wifi.setmode(wifi.STATION)
    station_cfg={}
    station_cfg.ssid=wifiName
    station_cfg.pwd=wifiPwssword
    wifi.sta.config(station_cfg)
    wifi.sta.connect()

    
    local t1 = tmr.create()
    t1:alarm(1000, tmr.ALARM_AUTO, function()
        if wifi.sta.getip() ~= nil then
            --print(wifi.sta.getip())
            tmr.delay(500000)
            print("wifiok")
            gpio.write(led, 1)
            creatConnectTCP()
            t1:unregister()
        else
            print("await get ip")  
            if gpio.read(led)==1 then
                gpio.write(led, 0) 
            else
                gpio.write(led, 1) 
            end
            --print(wifiName)
            --print(wifiPwssword)
        end
    end)
end
