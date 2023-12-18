import matplotlib.pyplot as plt

f = open("res/pidLog.txt").readlines();
X,A,B,C,D = [],[],[],[],[]
i=0
for line in f:
    s = line.split(" ")
    A.append(float(s[0])/330_000_000.0*1_000)
    B.append(float(s[1]))
    C.append(float(s[2]))
    D.append(float(s[3]))
    X.append(i)
    i+=1

plt.plot(X,A)
plt.xlabel("time u.a")
plt.ylabel("error in ms")
plt.ylabel
plt.show()
