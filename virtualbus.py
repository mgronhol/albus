#!/usr/bin/env python

import select, socket


server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.setblocking(0)
server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

server_address = ('', 1222)

server.bind(server_address)
server.listen(5)

inputs = [ server ]
outputs = [ ]

queue = []


try:
	while inputs:
		readable, writable, exceptional = select.select(inputs, [], inputs)
		
		for sock in readable:
			if sock is server:
				connection, client_address = sock.accept()
				connection.setblocking(0)
				inputs.append( connection )
				outputs.append( connection )
			
			else:
				data = sock.recv( 1024 )
				if data:
					queue.append( (sock, data) )
				else:
					outputs.remove( sock )
					inputs.remove( sock )
					sock.close()
		
		for sock in exceptional:
			if sock in inputs:
				inputs.remove( sock )
			if sock in outputs:
				outputs.remove( sock )
				sock.close()
		
		if len( queue ) > 0:
			origin,chunk = queue.pop()
			for sock in outputs:
				if sock != origin:
					sock.send( chunk )
				
except KeyboardInterrupt:
	pass

server.close()
