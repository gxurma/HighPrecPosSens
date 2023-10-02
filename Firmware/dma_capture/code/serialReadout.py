import serial
import time
from matplotlib import pyplot as plt
import numpy as np
from calibrationfile import amin, amax, b

calibrating = False
# calibrating = True
aufnahme = False
# aufnahme = True
wiedergabe = False
# wiedergabe = True
doplot = False
# doplot = True

calfaktor = 369.5/52363.0
# calfaktor=0.007

deltamin = 3000
deltamax = 0
cutoffstart = 67
cutoff = cutoffstart - 1
cutoffend = cutoffstart
print(cutoff)
position = 0
aold = []
k=0.995
c = []
cavg = []
cold = []
e = []
counter = 0
maxindexold1 = 1024 
maxindexold2 = 1024
maxindex1 = 1024
maxindex2 = 1024

if not wiedergabe:
    s = serial.Serial('COM6')  #adjust port to your needs

if doplot : plt.ion()
message = "2700\n"
first = True

while (cutoff < cutoffend):
    print()
    cutoff = cutoff + 1

    if aufnahme: 
        flog = open("Aufnahme23.txt","bw")
    if wiedergabe:
        flog = open("Aufnahme23.txt","br")

    position = 0
    delta = 0
    
    while(True) :
        
        
        # if (counter > 1353) and (counter < 1361): doplot = True
        # if (position > 12500) and (position < 14000): doplot = True
        # else : doplot = False 

        if not wiedergabe:
            s.write(message.encode())

            res = s.readline()

            # print(res)

            res = s.readline()

            # print(res)

            res = s.readline()

            # print(res)

        else : # if wiedergabe:
            res = flog.readline()
            if len(res) == 0 :  break

        if aufnahme :
            flog.write(res)
            flog.flush()
        
        counter = counter +1
        
        if counter < 10 : 
            continue

        t= range(len(res))
        a= list(res)
        # t = t[200:-54]
        # a= a[200:-54]
        t = t[200:2248]
        a= a[200:2248]
        # print (len(t), len(a))

        if first :
            if calibrating:
                amax = a.copy()
                amin = a.copy()
            aold = a.copy()
            c = a.copy()
            cold = a.copy()
            cavg = a.copy()
            e = a.copy()
            e[-1]=0
            first = False
            print(first)

        # print(a)
        if not aufnahme:
            if doplot : plt.figure(1)
            if doplot : plt.cla()
        # fig, (ax1,ax2) = if doplot : plt.subplots(2)
            if doplot : plt.plot(t,a)
        # xmin,xmax,ymin,ymax = if doplot : plt.axis()
        # print(len(a) )
        # print(xmin,xmax,ymin,ymax)
        # print(a[174:-53])
        # b = np.convolve(a,aold)
        # if doplot : plt.plot(b/100000)
        # time.sleep(0.5)

        # print(a[0:40])
        # print(amin[0:40])
        if (calibrating) :
            for i in range(len(a)):
                if (a[i] < amin[i]) :
                    amin[i] = a[i]
                if (a[i] > amax[i]) :
                    amax[i] = a[i]
        if not aufnahme:
            if doplot : plt.plot(t, amin)
            if doplot : plt.plot(t, amax)

        # print(amin[0:40])

        # Aufbereitung
        for j in range(len(a)):
            cold[j] = c[j]
            try:
                c[j] = (a[j] - amin[j])/(amax[j]-amin[j])*100 - 50
            except:
                None
            # cavg[j] = k*cavg[j]+(1-k)*c[j]
            #print(c[j], a[j],amin[j], end=" : ")   
        # print( c[0:40])
        # print(amin[0:40])

        if not aufnahme:

            if doplot : plt.plot(t, c)
            if doplot : plt.plot(t, cold)

        # for j in range(len(a)-1):
        #     e[j] = c[j]-c[j+1]
        # if doplot : plt.plot(t, e)
        # if doplot : plt.plot(t, cold)
        # if doplot : plt.plot(t, cavg)

        # f = np.fft.fft(c)
        # print(np.abs(f)[1:20])
        
        # if doplot : plt.plot(t, np.abs(f))
        # if doplot : plt.plot(t, g)
        # d = np.correlate(c,cold,mode="full")
        
        # c = c*np.bartlett(len(c))
        G_a = np.fft.fft(c)
        G_a[cutoff:-cutoff] = 0
        G_a[0] = 0  # keine DC Komponenten

        
        G_b = np.fft.fft(cold)
        # G_b = np.fft.fft(b)
        G_b[cutoff:-cutoff] = 0
        G_b[0] = 0 # keine DC komponenten
        # if doplot : plt.plot(G_b[1:200])

        # e = (np.fft.ifft(G_a) > 0)*50
        if not aufnahme:

            if doplot : plt.plot(t,np.fft.ifft(G_a))
            if doplot : plt.plot(t,np.fft.ifft(G_b))


        G_b_conj= np.conjugate(G_b)
        r = G_a*G_b_conj
        d = np.fft.ifft(r)
        # if doplot : plt.plot(r[1:100])



        if not aufnahme:
            if doplot : plt.figure(2)
            if doplot : plt.cla()

        h = list(d[int(len(d)/2):])
        # print(len(h))
        # if doplot : plt.plot(h)

        j = list(d[:int(len(d)/2)])
        # print(len(j))
        k = h+j
        # h = d[int(len(d)/2):] + d[:int(len(d)/2)]

        # if doplot : plt.plot(j)
        # if doplot : plt.plot(range(deltamin,deltamax),k[deltamin:deltamax])
        if doplot : plt.plot(k)


        # maxindexold1 = maxindex1
        # maxindexold2 = maxindex2

        # maxindex1 = (int) (np.argmax(k[:1024]))
        # maxindex2 = (int) (np.argmax(k[1024:])) + 1024

        maxindex = (int) (np.argmax(k))


        # if delta > len(d)/2 :
        #     delta = delta -len(d)
        
        # if maxindex < deltamin : deltamin = maxindex
        # if maxindex > deltamax : deltamax = maxindex

        # feininterpolation
#        polynoma = np.abs(k[maxindex-2:maxindex+3])
# #      polynomt = list(range(maxindex-2,maxindex+3))
#        p = np.polyfit(polynomt,polynoma,2)
#        x = -p[1]/p[0]/2
        # # print(polynoma, polynomt, p, x, maxindex)

#        delta = len(d)/2 - x

        # delta1 = len(d)/2 - maxindex1
        # delta2 = maxindex2 - len(d)/2

        delta = len(d)/2 - maxindex

        position = position + delta
        print("%03d %08d %8d %8d %8.5f %8.5f %8.5f"%(cutoff, counter, maxindex1, maxindex2, delta, position, position*calfaktor),end="\r")

        # aold = a

        # if doplot : plt.figure(3)
        # if doplot : plt.cla()
        # if doplot : plt.plot(abs(G_a[0:cutoff]))

        if not aufnahme:
            if doplot : plt.show()
            if doplot : plt.pause(0.005)
            

        if calibrating :
            if (counter > 550):
                break
        # time.sleep(0.005)
        #  print()
    if aufnahme or wiedergabe :
        flog.close()

if not wiedergabe :
    s.close()

if calibrating :

    calibf = open("calibrationfile.py","w")
    print("amin = ", amin, file=calibf)
    print("amax = ", amax, file=calibf)
    print("b = ",    b,    file=calibf)
    calibf.flush()
    calibf.close()
print()
