import serial
import time
from matplotlib import pyplot as plt

s = serial.Serial('COM6')  #adjust port to your needs
plt.ion()
message = "2700\n"
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

  #  print(a)
    plt.cla()
    plt.plot(t[11:-10],a[11:-10],visible=True)
    xmin,xmax,ymin,ymax = plt.axis()
    plt.show()
    plt.pause(0.1)
    print(len(a) )
    print(xmin,xmax,ymin,ymax)

    # time.sleep(0.5)

s.close()
