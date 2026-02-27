-- Endless loop that must aborted by signal interrupt
local time = os.clock();
while true do
  if os.clock() - time > 1 then
    time = os.clock()
    print("lua level = " .. lua.level())
  end
end
