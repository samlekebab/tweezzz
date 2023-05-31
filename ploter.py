###
import matplotlib.pyplot as plt
###
f = open("res/cpp.txt")
X = []
for line in f:
    X.append(int(line))
plt.plot(X)
plt.show()
###
