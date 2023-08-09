import numpy as np

x = np.linspace(0, 2, 38)

y = (10*x+0.5*(-10)*x*x)*7.65

y = y.astype(int)

print("{", end="")
for i in range(len(y)):
    print(y[i], end="")
    if i != len(y) - 1:
        print(", ", end="")
print("}")
print(len(y))
