uart.setup(0, 9600, 8, 0, 1, 1 )

require("apFun")

setSta()
 
uart.on("data", "\n",function(data)
    ok=pcall(udp_send,data)
    if ok then
--        print("udp send ok")
    else 
--        print("udp send error")
    end
end, 0)

