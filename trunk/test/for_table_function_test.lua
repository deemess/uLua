local a = {}
for i = 1, 10 do
a[i] = function() return i end
end
print(a[5]())