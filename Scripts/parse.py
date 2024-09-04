import os
import matplotlib.pyplot as plt
lamb = 25000
lambbda = [i for i in range(25000,50001,500)]
for i in range(25000,50001,500):
    print("lambda= "+str(i))
    os.system("./ns3 run 'dynamicRLCqueue.cc --lambda="+str(i)+" --simTag=staticRlc'")

for i in range(25000,50001,500):
    print("lambda= "+str(i))
    os.system("./ns3 run 'dynamicRLCqueue.cc --useDynRlc=true --lambda="+str(i)+" --simTag=dynamicRlc'")
thputst = []
errst = []
delayst = []

thputdy = []
errdy = []
delaydy = []
f = open("staticRlc","r+")
g = open("dynamicRlc","r+")
count = 0
tx = 0
rx = 0
for i in f:
    count+=1
    if(count%22 == 2):
        tx = int(i.split()[2])
        #print(tx)
    elif(count%22 == 6):
        thputst.append(float(i.split()[1]))
    elif(count%22 == 7):
        delayst.append(float(i.split()[2]))
    elif(count%22 == 9):
        rx = int(i.split()[2])
    elif(count%22 == 21):
        errst.append(float(1.0 - (rx/tx)))
    else:
        continue

count = 0
tx = 0
rx = 0
for i in g:
    count+=1
    if(count%22 == 2):
        tx = int(i.split()[2])
    elif(count%22 == 6):
        thputdy.append(float(i.split()[1]))
    elif(count%22 == 7):
        delaydy.append(float(i.split()[2]))
    elif(count%22 == 9):
        rx = int(i.split()[2])
    elif(count%22 == 21):
        errdy.append(float(1.0 - (rx/tx)))

print(len(thputst))

plt.figure(1)
plt.plot(lambbda, thputst, label ="Static RLC")
plt.plot(lambbda, thputdy, label ="Dynamic RLC")
plt.title("Throughtput vs lambda")
plt.ylabel("Throughput")
plt.ylabel("lambdda")
plt.legend()

plt.figure(2)
plt.plot(lambbda, delayst, label ="Static RLC")
plt.plot(lambbda, delaydy, label ="Dynamic RLC")
plt.title("Delay vs lambda")
plt.ylabel("Delay")
plt.ylabel("lambdda")
plt.legend()

plt.figure(3)
plt.plot(lambbda, errst, label ="Static RLC")
plt.plot(lambbda, errdy, label ="Dynamic RLC")
plt.title("Error ratio vs lambda")
plt.ylabel("Error ratio")
plt.ylabel("lambdda")
plt.legend()
plt.show()
