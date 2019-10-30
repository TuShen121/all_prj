--ADCD,1234567890,1981,12e401114,
require("fun")
led = 4
gpio.mode(led, gpio.OUTPUT)
local wifiName,wifiPwssword,bigName,bigPwssword,IDO,ID1,ID2 = gatLocalConnectData("data.lua")
local host =  "www.bigiot.net"
local port =  8181
local TCP_OBJ = nil
local cu = net.createConnection(net.TCP)
local masterID = "U1673"
function  tcp_send(data)
    ok, stoped = pcall(sjson.encode, {M="say",ID=ID1,C=data})
    cu:send( stoped.."\n" ) 
    ok, stoped = pcall(sjson.encode, {M="say",ID=ID2,C=data})
    cu:send( stoped.."\n" ) 
    cu:send([[{"M":"update","ID":"]]..bigName..[[","V":{"]]..IDO..[[":"]]..string.sub(data,7,11)..[["}}]].."\n" ) 
    --local s=[[{"M":"update","ID":"]]..bigName..[[","V":{"]]..IDO..[[":"]]..string.sub(data,7,11)..[["}}]]
    --cu:send([[{"M":"update","ID":"13943","V":{"12716":"12.5"}}]].."\n" ) 
end

function creatConnectTCP() 
    cu:connect(port, host)
    cu:on("receive", function(cu, c) 
       -- print(c)
        r = sjson.decode(c) 
        if r.M == "say" then
            masterID=r.ID
            print(r.C)      
           -- ok, stoped = pcall(sjson.encode, {M="say",ID=ID2,C="546"})
            --cu:send( [[{"M":"say","C":"2131","ID":"U1673"}]].."\n" )   
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
    ledFlag = 0
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
            if ledFlag==1 then
                gpio.write(led, 0) 
                ledFlag=0
            else
                gpio.write(led, 1) 
                ledFlag=1
            end
            --print(wifiName)
            --print(wifiPwssword)
        end
    end)
end
