--ADCD,1234567890,1981,12e401114,
local localport=6531
local localip="255.255.255.255"
led = 4
gpio.mode(led, gpio.OUTPUT)

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
        <h1>esp8266 Config</h1>
        <form action="config" method="get">
            wifiName:<input type="text" name="name"  />
            <br>
            <br>
            wifiPwd:<input type="text" name="pwd"  />
            <br>
            <br>
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
            ID:<input type="text" name="ID"  />
            <br>
            <br>
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
            KEY:<input type="text" name="KEY"  />
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
            writFildDat(req.query.name..','..req.query.pwd..','..req.query.ID..','..req.query.KEY..',',"data.lua")
            res:send([[<h1>OK</h1>]])
            writFildDat("m1","mode.lua")
            tmr.delay(1000)
            node.restart()
        end)
        
        -- Get json
        httpServer:use('/json', function(req, res)
            res:type('application/json')
            res:send('{"doge": "smile"}')
        end)
        
        -- Redirect
        httpServer:use('/redirect', function(req, res)
            res:redirect('doge.jpg')
        end)
end

function setAp()
    wifi.setmode(wifi.SOFTAP)
    cfg={}
    cfg.ssid="chang"
    cfg.pwd="chang12345"
    wifi.ap.config(cfg)
    --print(wifi.ap.getip())
    
    local t1 = tmr.create()
    t1:alarm(100, tmr.ALARM_AUTO, function()
        if gpio.read(led)==1 then
            gpio.write(led, 0) 
        else
            gpio.write(led, 1) 
        end

    end)
    setUdp()   
end
