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