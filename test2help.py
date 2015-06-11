a = 1
min = 24
max = 28
b = 0

if a == min:
    min = a
elif a > max:
    max = a
else:
    b += 1
    
print(min)
print(max)
print(b)

i = 4
while i < 9:
    print("y")
    if i > 6:
        print("z")
    i += 1