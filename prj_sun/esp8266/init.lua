uart.setup(0, 9600, 8, 0, 1, 1 )

require("staFun")
require("fun")
require("apFun")
local mode = gatLocalConnectData("mode.lua")

if string.find(mode, "m0", 1,2) then
    print("setAp()")
    setAp()   
elseif string.find(mode, "m1", 1,2) then
    print("setSta()")
    setSta()   

end

uart.on("data", '\n',function(data)
    if string.find(mode, "m1", 1,2) then
        ok=pcall(tcp_send,data)
        if ok then
            --print("udp send ok")
        else 
            --print("udp send error")
        end
    end
    if string.find(data, "m0", 1,2) then
       writFildDat("m0","mode.lua")
       node.restart()
    elseif string.find(data, "m1", 1,2) then
        writFildDat("m1","mode.lua")
        node.restart()
    elseif string.find(data, "rm", 1,2) then
        file.remove("init.lua");
        node.restart()
    end
end, 0)
--
