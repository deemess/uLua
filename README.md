uLua
====

Lua compiler and iterpreter to run on microcontroller (AVR 8 bit)

About

uLua aimed to run Lua scripts on microcontrollers with very limited resources, like RAM. uLua should work on microcontrollers with RAM >= 1k. Second thing which uLua should achieve is fast code interpreter. Goal is 1 Million Lua instructions on 10 MIPS (10 MHz for AVR).
Usage

luac.exe -s -o alltest.luc alltest.lua
ChunkSpy.lua alltest.luc alltest.lua -o alltest.lst
uLuaPC.exe alltest.luc

ChunkSpy.lua - optional. This script used to generate listing file from compiled luc file.
Features

Currenly support:

function calls
global consts
global and local variables
math operations
for loops
native functions (like print())
closures with upvalues
garbage collector 

Following code could be executed by uLua.

```Lua
--load nil test
function loadtest()
        local a,b,c,d,e = nil,nil,0
        local aa,bb = true,false
        print(a,b,c)
        print(aa,bb)
end
loadtest()

-- for loop test
for i = 1, 3, 1 do
        print(i)
end 

for i = 3, 1, -1 do
        print(i)
end 

-- upvalue test
function newCounter ()
        local i = 0
        return function ()   -- anonymous function
                        i = i + 1
                        return i
                end
end
c1 = newCounter()
print(c1())  --> 1
print(c1())  --> 2
print(c1())  --> 3


-- function call test
function func1(par1,par2)
        local local1 = par1 + par2
        return local1
end

function func2(par1,par2,par3)
        local local1 = par1 + par2
        local local2 = par3 * par3
        local1 = local1 + local2
        return local1
end

a=1
b=3
c=3.5

result1 = func1(a,b)
result2 = func2(a,b,c)
print("Result1 = ", result1)
print("Result2 = ", result2)

--math test
local a,b = 2,4; a = a + 4 * b - a / 2 ^ b % 3
print("a + 4 * b - a / 2 ^ b % 3 = ", a)

--conditional jumps test
if a ~= b 
        then print("a != b")
end
if a > b 
        then print("a > b")
end
if a >= b 
        then print("a >= b")
end
if a < b 
        then print("a < b")
end
if a <= b 
        then print("a <= b")
end
if a == b 
        then print("a == b")
end
```

Upcoming features:

GC - garbage collector. Currenly implemented GC heap standard functions, like:

```c
    //initialize garbage collector and memory management
    void gcInit();
    //create new variable and return its number
    gcvarpt* gcNew(vartype type);
    //create new variable with given size and return its number
    gcvarpt* gcNew(vartype type, u08 size);
    //delete variable
    void gcDelete(gcvarpt* variable);
```

    Tables - widly used in lua variable type. Will be implemented as hash table. GC should be implemented first. 

Supported Operands

|OP_CODE|NAME|Description|
|-------|----|-----------|
|0	|MOVE	|Copy a value between registers|
|1	|LOADK	|Load a constant into a register|
|2	|LOADBOOL	|Load a boolean into a register|
|3	|LOADNIL	|Load nil values into a range of registers|
|4	|GETUPVAL	|Read an upvalue into a register|
|5	|GETGLOBAL	|Read a global variable into a register|
|7	|SETGLOBAL	|Write a register value into a global variable|
|8	|SETUPVAL	|Write a register value into an upvalue|
|12	|ADD	|Addition operator|
|13	|SUB	|Subtraction operator|
|14	|MUL	|Multiplication operator|
|15	|DIV	|Division operator|
|16	|MOD	|Modulus (remainder) operator|
|17	|POW	|Exponentiation operator|
|22	|JMP	|Unconditional jump|
|23	|EQ	|Equality test|
|24	|LT	|Less than test|
|25	|LE	|Less than or equal to test|
|28	|CALL	|Call a closure|
|36	|CLOSURE	|Create a closure of a function prototype|
|30	|RETURN	|Return from function call|
|31	|FORLOOP	|Iterate a numeric for loop|
|32	|FORPREP	|Initialization for a numeric for loop|

Not yet supported

|OP_CODE|NAME|Description|
|-------|----|-----------|
|6	|GETTABLE	|Read a table element into a register|
|9	|SETTABLE	|Write a register value into a table element|
|10	|NEWTABLE	|Create a new table|
|11	|SELF	|Prepare an object method for calling|
|18	|UNM	|Unary minus operator|
|19	|NOT	|Logical NOT operator|
|20	|LEN	|Length operator|
|21	|CONCAT	|Concatenate a range of registers|
|26	|TEST	|Boolean test, with conditional jump|
|27	|TESTSET	|Boolean test, with conditional jump and assignment|
|29	|TAILCALL	|Perform a tail call|
|33	|TFORLOOP	|Iterate a generic for loop|
|34	|SETLIST	|Set a range of array elements for a table|
|35	|CLOSE	|Close a range of locals being used as upvalues|
|37	|VARARG	|Assign vararg function arguments to registers|

Known Issues

GC could leak, implemented simple reference counting logic. Reference cycling is not supported. 
