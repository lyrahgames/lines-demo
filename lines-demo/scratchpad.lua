print("lua.level() = " .. lua.level())

lua.eval("print('lua.level() = ' .. lua.level());");

lua.eval_file("scratchpad.lua");

print("hello, world")

print(done())

quit()
print(done())
