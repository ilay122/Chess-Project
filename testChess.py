import socket,threading

s=socket.socket()
s.connect(('127.0.0.1',42069))

def printAllRecv():
	while 1:
		datarecv=s.recv(1024)
		print datarecv

t=threading.Thread(target=printAllRecv,args=())
t.start()

while 1:
	inp=raw_input()
	s.send(inp)
