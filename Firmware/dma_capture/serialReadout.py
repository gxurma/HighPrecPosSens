import serial
import time
from matplotlib import pyplot as plt
import numpy as np

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
    t = t[174:-53]
    a= a[174:-53]
    if first :
        aold = a
        amin = a
        c = a
        first = False
  #  print(a)
    plt.cla()
    plt.plot(t,a)
    xmin,xmax,ymin,ymax = plt.axis()
    print(len(a) )
    print(xmin,xmax,ymin,ymax)
    # print(a[174:-53])
    # b = np.convolve(a,aold)
    # plt.plot(b/100000)
    # time.sleep(0.5)
    
    for i in range(len(a)):
        if (a[i] < amin[i]) :
            amin[i] = a[i]   
    plt.plot(t, amin)
    print(amin[0:20])

    for j in range(len(a)):
        c[j] = a[j] - amin[j]   
    print( c[0:20])

    # plt.plot(t, c)
    aold = a
    plt.show()
    plt.pause(1)

s.close()
