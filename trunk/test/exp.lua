explim = 100 
step = 0.000001 
  
function myexp(val) 
  local sum = 0.0 
  local fact = 1.0 
  local x = 1.0 
  local i 
  
  for i = 1, explim, 1 do 
    fact = fact * i 
    x = x * val 
    sum = sum + x/fact 
  end 
  
  return sum + 1.0 
end 
  
function integrate(min,max) 
  local sum = 0.0 
  
  while min < max do 
    sum = sum + myexp(min)*step 
    min = min + step 
  end  
  return sum 
end 
  
print ("exponent( 1.0)=" , myexp(1.0)) 
print ("integral(0..1)=" , integrate(0.0,1.0))