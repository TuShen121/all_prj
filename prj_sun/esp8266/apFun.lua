--ADCD,1234567890,1981,12e401114,
require("fun")
local localport=6531
local localip="255.255.255.255"
led = 4
gpio.mode(led, gpio.OUTPUT)
local wifiName,wifiPwssword,bigName,bigPwssword,IDO,ID1,ID2 = gatLocalConnectData("data.lua")
function setUdp()   
   -- Serving static files
        dofile('httpServer.lua')
        httpServer:listen(80)
        
        -- Custom API
        -- Get text/html
        httpServer:use('/', function(req, res)
         res:send([[<!doctype html>
<html>
<head>
<meta charset="utf-8">
<title></title>
<meta name="viewport" content="width=device-width,initial-scale=1,minimum-scale=1,maximum-scale=1,user-scalable=no" />
</head>
<body>
<h1>ESP8266 config for bigIot</h1>
<form action="config" method="get">
wifiName:<input type="text" name="name"  value="]]..wifiName..[["/>
<br>
<br>
&nbsp;
wifiPwd:<input type="text" name="pwd"  value="]]..wifiPwssword..[["/>
<br>
<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
ID:<input type="text" name="ID"  value="]]..bigName..[["/>
<br>
<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
KEY:<input type="text" name="KEY"  value="]]..bigPwssword..[["/>
<br>
<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
IDO:<input type="text" name="IDO" value="]]..IDO..[[" />
<br>
<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
ID1:<input type="text" name="ID1" value="]]..ID1..[[" />
<br>
<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
ID2:<input type="text" name="ID2"  value="]]..ID2..[["/>
<br>
<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
<input type="submit" value="ok"/>
</form>
</body>
</html>
]]) -- /welcome?name=doge
        end)
        
        -- Get file
        httpServer:use('/config', function(req, res)
--            print(req.query.name)
--            print(req.query.pwd)
--            print(req.query.ID)
--            print(req.query.KEY)
--            print(req.query.ID1)
--            print(req.query.ID2)
--            print(req.query.IDO)
            writFildDat(req.query.name..','
                        ..req.query.pwd..','
                        ..req.query.ID..','
                        ..req.query.KEY..','
                        ..req.query.IDO..','
                        ..req.query.ID1..','
                        ..req.query.ID2..','                   
                        ,"data.lua")
            res:send([[<h1>OK</h1>]])
            writFildDat("m1","mode.lua")
            tmr.delay(1000)
            node.restart()
        end)
        
        -- Get json
--        httpServer:use('/json', function(req, res)
--            res:type('application/json')
--            res:send('{"doge": "smile"}')
--        end)
        
        -- Redirect
--        httpServer:use('/redirect', function(req, res)
--            res:redirect('doge.jpg')
--        end)
end

function setAp()
    wifi.setmode(wifi.SOFTAP)
    cfg={}
    cfg.ssid="ning"
    cfg.pwd="ningzi123"
    wifi.ap.config(cfg)
    --print(wifi.ap.getip())
     ledFlag = 0
    local t1 = tmr.create()
    t1:alarm(200, tmr.ALARM_AUTO, function()
       if ledFlag==1 then
            gpio.write(led, 0) 
            ledFlag=0
        else
            ledFlag=1
            gpio.write(led, 1) 
        end
    end)
    setUdp()   
end
