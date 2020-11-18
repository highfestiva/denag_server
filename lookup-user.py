#!/usr/bin/env python3

import socket

host = input('host> ')
port = 3384
key = input('user> ')+'_gametime'
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((host,port))
key = key.encode() + b'\0\0'
print('>>>', key)
s.send(key)
print('<<<', s.recv(1024))
