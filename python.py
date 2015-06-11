min = 5
max = 10
b = 0
a = 5
if a < min:
    min = a
elif a > max:
    max = a
else:
    b += 1
    
print(min)
print(max)
print(b)