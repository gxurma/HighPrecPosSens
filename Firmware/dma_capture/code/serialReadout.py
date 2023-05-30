import serial
import time
from matplotlib import pyplot as plt
import numpy as np
from calibrationfile import amin, amax

aold = []

c = []
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

    print(res.decode() )

    res = s.readline()
 
    t= range(len(res))
    a= list(res)
    t = t[180:-53]
    a= a[180:-53]
    if first :
        amax = a.copy()
        aold = a.copy()
        amin = a.copy()
        c = a.copy()
        first = False
        print(first)

    # print(a)
    plt.cla()
    plt.plot(t,a)
    xmin,xmax,ymin,ymax = plt.axis()
    print(len(a) )
    print(xmin,xmax,ymin,ymax)
    # print(a[174:-53])
    # b = np.convolve(a,aold)
    # plt.plot(b/100000)
    # time.sleep(0.5)

    # print(a[0:40])
    # print(amin[0:40])
    
    for i in range(len(a)):
        if (a[i] < amin[i]) :
            amin[i] = a[i]
        if (a[i] > amax[i]) :
            amax[i] = a[i]
    plt.plot(t, amin)
    plt.plot(t, amax)

    # print(amin[0:40])

    for j in range(len(a)):
        c[j] = (a[j] - amin[j])
        #print(c[j], a[j],amin[j], end=" : ")   
    # print( c[0:40])
    # print(amin[0:40])

    plt.plot(t, c)

    # f = np.fft.fft(c)
    # print(np.abs(f)[1:20])
    
    # plt.plot(t, np.abs(f))
    # plt.plot(t, g)


    aold = a
    plt.show()
    plt.pause(0.01)

    if (counter > 600):
      break
    else:
        counter = counter +1

s.close()

print("amin = ", amin)
print("amax = ", amax)

