function split( str,reps )
    local resultStrList = {}
    string.gsub(str,'[^'..reps..']+',function ( w )
        table.insert(resultStrList,w)
    end)
    return resultStrList
end
function writFildDat(dat,filen)
    if file.open(filen, "w") then
        file.writeline(dat)
        file.close()
    end
end

function gatLocalConnectData(filen)
    local dat
    if file.open(filen, "r") then
        dat = split(file.readline(),',')
        file.close()
    end
    return dat[1],dat[2],dat[3],dat[4],dat[5],dat[6],dat[7]
end


