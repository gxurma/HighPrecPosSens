import serial
import time
from matplotlib import pyplot as plt
import numpy as np
from calibrationfile import amin, amax

calibrating = False

position = 0
cutoff = 35

aold = []
k=0.995
c = []
cavg = []
cold = []
counter = 0
s = serial.Serial('COM6')  #adjust port to your needs
plt.ion()
message = "2700\n"
first = True
while(True) :
    s.write(message.encode())

  #  res = s.readline()

    # print(res)

 #   res = s.readline()

    # print(res)

    res = s.readline()

    # print(res)

    res = s.readline()

    # print(res.decode() )

    res = s.readline()
 
    t= range(len(res))
    a= list(res)
    t = t[200:-54]
    a= a[200:-54]
    if first :
        if calibrating:
            amax = a.copy()
            amin = a.copy()
        aold = a.copy()
        c = a.copy()
        cold = a.copy()
        cavg = a.copy()
        first = False
        print(first)

    # print(a)
    plt.cla()
    plt.plot(t,a)
    # xmin,xmax,ymin,ymax = plt.axis()
    # print(len(a) )
    # print(xmin,xmax,ymin,ymax)
    # print(a[174:-53])
    # b = np.convolve(a,aold)
    # plt.plot(b/100000)
    # time.sleep(0.5)

    # print(a[0:40])
    # print(amin[0:40])
    if (calibrating) :
        for i in range(len(a)):
            if (a[i] < amin[i]) :
                amin[i] = a[i]
            if (a[i] > amax[i]) :
                amax[i] = a[i]
    plt.plot(t, amin)
    plt.plot(t, amax)

    # print(amin[0:40])


    for j in range(len(a)):
        cold[j] = c[j]
        c[j] = (a[j] - amin[j])
        # cavg[j] = k*cavg[j]+(1-k)*c[j]
        #print(c[j], a[j],amin[j], end=" : ")   
    # print( c[0:40])
    # print(amin[0:40])

    plt.plot(t, c)
    # plt.plot(t, cold)
    # plt.plot(t, cavg)

    # f = np.fft.fft(c)
    # print(np.abs(f)[1:20])
    
    # plt.plot(t, np.abs(f))
    # plt.plot(t, g)
    # d = np.correlate(c,cold,mode="full")
    
    # c = c*np.bartlett(len(c))
    G_a = np.fft.fft(c)
    G_a[cutoff:-cutoff] = 0
    # plt.plot(np.fft.ifft(G_a))
    # plt.plot(c)
    
    G_b = np.fft.fft(cold)
    G_b[cutoff:-cutoff] = 0
    # plt.plot(G_b[1:200])

    G_b_conj= np.conjugate(G_b)
    r = G_a*G_b_conj
    d = np.fft.ifft(r)
    # plt.plot(d)

    delta = (int) (np.argmax(d))
    if delta > len(d)/2 :
        delta = delta -len(d)
    position = position + delta
    print("%d %8d %8d"%(counter, delta, position), end="\r")

    # aold = a
    plt.show()
    plt.pause(0.005)
    if calibrating :
        if (counter > 600):
            break
        else:
            counter = counter +1
    # time.sleep(0.005)

s.close()

print("amin = ", amin)
print("amax = ", amax)

