import serial
import time
from matplotlib import pyplot as plt

s = serial.Serial('COM6')  #adjust port to your needs

message = "s\n"
while(True) :
    s.write(message.encode())

    res = s.readline()

    # print(res)

    res = s.readline()

    # print(res)

    res = s.readline()

    # print(res)

    res = s.readline()

    print(res.decode() )

    res = s.readline()
 
    t= range(len(res))
    a= list(res)

    plt.plot(t[11:-10],a[11:-10]);
    print(a)
    plt.show()
    # print(res.length() )


    time.sleep(0.1)

s.close()
