print("lua.level() = " .. lua.level())

lua.eval("print('lua.level() = ' .. lua.level());");

lua.eval_file("scratchpad.lua");

print("hello, world")

print(done())

quit()
print(done())

return


local x = 0;
while true do
  x = x + 1;
  if x == 10 then
    return
  end
  print(x);
end


local x = 0;
while not lua.stop_requested() do
  x = x + 1
  print(x)
  if x > 10 then lua.request_stop() end
end

local i = 0;
while true do
  lua.eval_file("infinite_loop.lua")
  print("outer loop: " .. i)
  i = i + 1
end


test()

print(10)

