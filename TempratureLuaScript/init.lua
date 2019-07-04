--init.lua
if srv~=nil then
   srv:close()
end 

local moduleName = "dht11"
local M = {}
_G[moduleName] = M
 
local humidity
local temperature
local moisture

print("Setting up WIFI...")
wifi.setmode(wifi.STATION)
--modify according your wireless router settings
-- wifi.sta.config("Redmi 4A","12345678")
wifi.sta.config("Redmi 5","CB3B0ED6")
wifi.sta.connect()
tmr.alarm(1, 1000, 1, function() 
if wifi.sta.getip()== nil then 
print("IP unavaiable, Waiting...") 
else 
tmr.stop(1)
print("Config done, IP is "..wifi.sta.getip())
-- Measure temperature and post data to E-controller server

sensor_pin = 0;  --Connect Soil moisture analog sensor pin to A0 of NodeMCU

function M.read(pin)
  local checksum
  local checksumTest
  humidity = 0
  temperature = 0
  checksum = 0
 
  -- Use Markus Gritsch trick to speed up read/write on GPIO
  local gpio_read = gpio.read
 
  local bitStream = {}
   for j = 1, 40, 1 do
    bitStream[j] = 0
  end
  local bitlength = 0
 
  -- Step 1:  send out start signal to DHT22
  gpio.mode(pin, gpio.OUTPUT)
  gpio.write(pin, gpio.HIGH)
  tmr.delay(100)
  gpio.write(pin, gpio.LOW)
  tmr.delay(20000)
  gpio.write(pin, gpio.HIGH)
  gpio.mode(pin, gpio.INPUT)
 
  -- Step 2:  DHT22 send response signal
  -- bus will always let up eventually, don't bother with timeout
  while (gpio_read(pin) == 0 ) do end
  local c=0
  while (gpio_read(pin) == 1 and c < 500) do c = c + 1 end
  -- bus will always let up eventually, don't bother with timeout
  while (gpio_read(pin) == 0 ) do end
  c=0
  while (gpio_read(pin) == 1 and c < 500) do c = c + 1 end
 
  -- Step 3: DHT22 send data
  for j = 1, 40, 1 do
    while (gpio_read(pin) == 1 and bitlength < 10 ) do
      bitlength = bitlength + 1
    end
    bitStream[j] = bitlength
    bitlength = 0
    -- bus will always let up eventually, don't bother with timeout
    while (gpio_read(pin) == 0) do end
  end
   hum=" "
   tem=" "
   chk=" "
  --DHT data acquired, process.
  for i = 1, 16, 1 do
  hum=hum.." "..tostring(bitStream[i])
  tem=tem.." "..tostring(bitStream[i+16])
    if (bitStream[i] > 3) then
      humidity = humidity + 2 ^ (16 - i)
    end
    if (bitStream[i + 16] > 3) then
      temperature = temperature + 2 ^ (16 - i)
    end
  end
  for i = 1, 8, 1 do
  chk=chk.." "..tostring(bitStream[i+32])
    if (bitStream[i + 32] > 3) then
      checksum = checksum + 2 ^ (8 - i)
    end
  end
    checksumTest = (bit.band(humidity, 0xFF) + bit.rshift(humidity, 8) + bit.band(temperature, 0xFF) + bit.rshift(temperature, 8))
  checksumTest = bit.band(checksumTest, 0xFF)
 
  if temperature > 0x8000 then
    -- convert to negative format
    temperature = -(temperature - 0x8000)
  end
 
  -- conditions compatible con float point and integer
  if (checksumTest - checksum >= 1) or (checksum - checksumTest >= 1) then
    humidity = nil
  end
  --print("checksum ", checksum)
  --print("checksumTest ", checksumTest)
  --print("humidity - timing of bits ",hum)
  --print("temperat - timing of bits ",tem)
  --print("checksum - timing of bits ",chk)
end
 
function M.getTemperature()
  return temperature
end
 
function M.getHumidity()
  return humidity
end

--function M.getMoisture()
  --    local moisture_percentage = ( 100.00 - ( (adc.read(sensor_pin)/1023.00) * 100.00 ) )
    --  return moisture_percentage
--end

sensorType="dht11"          -- set sensor type dht11 or dht22


    PIN = 3 --  data pin, GPIO0
    humi=0
    temp=0
--- Get temp and send data to E-controller server
function sendData()
dht=M
    dht.read(PIN)
    chck=1
    h=dht.getHumidity()
    t=dht.getTemperature()
    --moist=dht.Moisture()
    moist = 44
    if h==nil then h=0 chck=0 end
    if sensorType=="dht11"then
        humi=h/256
        temp=t/256
    else
        humi=h/10
        temp=t/10
    end
    fare=(temp*9/5+32)
--print("Temp:"..temp .. "."..string.format("%04d", temp).."Hum"..humi .. "."..string.format("%04d", humi)" C\n")
-- conection to E-controller
print("Sending data to irrigation server")
conn=net.createConnection(net.TCP, 0) 
conn:on("receive", function(conn, payload) print(payload) end)
-- E-contoller IP 192.168.43.80
--conn:connect(80,'111.118.215.253')
conn:connect(4111,'192.168.43.217') 
--conn:send("GET /temperatureDemo/test.php?key=temprature&temp="..temp.."."..string.format("%04d", temp).." HTTP/1.1\r\n")
conn:send("GET /api/temp?temp="..temp.."&hum="..humi.."&moist="..moist.." HTTP/1.1\r\n")  
conn:send("Host: localhost.com\r\n") 
conn:send("Accept: */*\r\n") 
conn:send("User-Agent: Mozilla/4.0 (compatible; esp8266 Lua; Windows NT 5.1)\r\n")
conn:send("\r\n")
conn:on("sent",function(conn)
                      print("data sent complete")
                      print("Closing connection")
                      conn:close()
                  end)
conn:on("disconnection", function(conn)
                                print("Got disconnection...")
  end)
end

-- send data every X ms to thing speak
tmr.alarm(0, 30000, 1, function() sendData() end )

-- send data every X ms to thing speak
-- tmr.alarm(2, 60000, 1, function() sendData() end )
end 
end)
