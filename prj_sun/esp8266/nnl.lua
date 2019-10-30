led = 2
gpio.mode(led, gpio.OUTPUT)
 t1 = tmr.create()
t1:alarm(500, tmr.ALARM_AUTO, function()
    if gpio.read(led)==1 then
        gpio.write(led, 0) 
        print(1)
    else
        print(2)
        gpio.write(led, 1) 
    end
end)