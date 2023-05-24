import serial

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

    print(res.decode())

    res = s.readline()

    # print(res)

s.close()
