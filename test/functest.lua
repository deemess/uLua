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
