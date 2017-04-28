import socket,select,threading,time,random


server=socket.socket()
server.bind(('',42069))
server.listen(5)

global listofclients,peopleinqueue,peopleingame,peoplewhoaccept,whoaccepted,whowithwho

listofclients=[]

peopleinqueue=[]

peoplewhoaccept=[]
whoaccepted={}

whowithwho={} # BOTH FOR ACCEPTING AND GAME

peopleingame=[]



def putInAcceptAndCheckIfBothAccepted(s1,s2):
	global listofclients,peopleinqueue,peopleingame,peoplewhoaccept,whoaccepted,whowithwho
	peoplewhoaccept.append(s1)
	peoplewhoaccept.append(s2)
	s1.send("Accept Or Decline")
	s2.send("Accept Or Decline")
	whoaccepted[s1]=None
	whoaccepted[s2]=None
	whowithwho[s1]=s2
	whowithwho[s2]=s1
	time.sleep(10)
	if s1 in peopleingame and s2 in peopleingame and whowithwho[s1] is s2:
		#BOTH ACCEPTED
		print "They Both Accepted in Thread"
	
	if s1 in whoaccepted and whoaccepted[s1] == None:
		del whoaccepted[s1]
		removeSocket(s1)
	if s2 in whoaccepted and whoaccepted[s2] == None:
		del whoaccepted[s2]
		removeSocket(s2)
	if s1 in peoplewhoaccept:
		s1.send("Back In Queue")
		peoplewhoaccept.remove(s1)
		peopleinqueue.append(s1)
	if s2 in peoplewhoaccept:
		s2.send("Back In Queue")
		peoplewhoaccept.remove(s2)
		peopleinqueue.append(s2)
		
	

def removeSocket(s):
	if s in peopleinqueue: #DUDE LEFT Q
		peopleinqueue.remove(s)
	elif s in peoplewhoaccept: #DUDE LEFT WHILE ACCEPTING
		peoplewhoaccept.remove(s)
	elif s in peopleingame:#DUDE LEFT IN THE MIDDLE OF A GAME
		otherdude=whowithwho[s]
		otherdude.send("Enemy Left")
		del whowithwho[s]
		del whowithwho[otherdude]
		peopleingame.remove(otherdude)
		
	if s in listofclients:
		listofclients.remove(s)
	s.close()

def handleRecvData(sock,datarecv):
	global listofclients,peopleinqueue,peopleingame,peoplewhoaccept,whoaccepted,whowithwho
	if sock in peoplewhoaccept:
		if datarecv == "Accept":
			whoaccepted[sock]=True
			print "Dude Accepted"
			if whoaccepted[whowithwho[sock]] == False:
				otherdude=whowithwho[sock]
				peopleinqueue.append(sock)
				peoplewhoaccept.remove(sock)
				del whowithwho[sock]
				del whowithwho[otherdude]
				del whoaccepted[sock]
				del whoaccepted[otherdude]
				sock.send("Back In Queue")
				return
			if whoaccepted[whowithwho[sock]]:
				peoplewhoaccept.remove(sock)
				peoplewhoaccept.remove(whowithwho[sock])
				peopleingame.append(sock)
				peopleingame.append(whowithwho[sock])
				#SEND EACH OTHER TEAM AND STUFF
				print "They both accepted -> sending colors"
				if random.random() >= 0.5:
					sock.send("WHITE")
					whowithwho[sock].send("BLACK")
				else:
					sock.send("BLACK")
					whowithwho[sock].send("WHITE")
		elif datarecv == "Decline":
			whoaccepted[sock]=False
			if whoaccepted[whowithwho[sock]] == True:
				otherdude=whowithwho[sock]
				peopleinqueue.append(otherdude)
				peoplewhoaccept.remove(otherdude)
				del whowithwho[sock]
				del whowithwho[otherdude]
				del whoaccepted[sock]
				del whoaccepted[otherdude]
				otherdude.send("Back In Queue")
				
			removeSocket(sock)
	elif sock in peopleingame:
		whowithwho[sock].send(datarecv)
		print "In game"
	"""
	"""

while 1 :
	rlist,wlist,xlist=select.select([server]+listofclients,[],[])
	for s in rlist:
		if s is server:
			client_sock,client_addr=s.accept()
			peopleinqueue.append(client_sock)
			listofclients.append(client_sock)
			print "New Dude In The Queue"
		else:
			try:
				datarecv=s.recv(1024)
				print datarecv
				if datarecv:
					handleRecvData(s,datarecv)
				else:
					removeSocket(s)
			except:
				removeSocket(s)
	if len(peopleinqueue) >= 2:
		t=threading.Thread(target=putInAcceptAndCheckIfBothAccepted,args=(peopleinqueue.pop(),peopleinqueue.pop()))
		t.daemon = True
		t.start()