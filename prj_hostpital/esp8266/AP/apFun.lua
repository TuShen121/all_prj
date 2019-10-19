led = 4
gpio.mode(led, gpio.OUTPUT)
local remotPport=5001
local remotIP="255.255.255.255"

udpSocket = net.createUDPSocket()

function udp_send(data)
    udpSocket:send(remotPport,remotIP,data)
end
function setUdp()   
    udpSocket:listen(5000)
    udpSocket:on("receive", function(s, data, port, ip)
        print(data)
    end)
    gpio.write(led, gpio.LOW)
end

function setAp()
    wifi.setmode(wifi.SOFTAP)
    cfg={}
    cfg.ssid="hostpitalssid"
    cfg.pwd="hostpital"
    wifi.ap.config(cfg)
    --print(wifi.ap.getip())
    setUdp()   
end
