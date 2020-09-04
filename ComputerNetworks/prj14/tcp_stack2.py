#!/usr/bin/python

import sys
import string
import socket
from time import sleep

data = string.digits + string.lowercase + string.uppercase

def server(port):
    s = socket.socket()
    s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    
    s.bind(('0.0.0.0', int(port)))
    s.listen(3)
    
    cs, addr = s.accept()
    print addr
    
    fp = open('server-output.dat', 'w')


    while True:
        data = cs.recv(1000)
        s.listen(3)
        if data:
            fp.write(data)
        else:
            break
    
    fp.close()
    s.close()
    


def client(ip, port):
    s = socket.socket()
    s.connect((ip, int(port)))
    fp = open('client-input.dat', 'r')
    data = fp.read()
    file_len = len(data)
    complete_pack_num = file_len / 1000
    data_left = file_len % 1000
    print ('debug: pack_num:%d, data_left:%d' %(complete_pack_num, data_left))

    for i in range(complete_pack_num):
        s.send(data[i*1000 : (i+1)*1000])
        #if(i%100 == 0):
            #sleep(1)
        sleep(0.01)
        print ('debug: i=%d' %(i))
    
    if data_left :
        s.send(data[(i+1)*1000 : file_len])


    fp.close()
    sleep(1)
    s.close()

if __name__ == '__main__':
    if sys.argv[1] == 'server':
        server(sys.argv[2])
    elif sys.argv[1] == 'client':
        client(sys.argv[2], sys.argv[3])
