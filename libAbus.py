#!/usr/bin/env python

import socket, struct



def compute_checksum( data ):
	out = 0
	for d in data:
		out ^= ord( d )
	return out

def create_packet( recipient, flags, func_code, data ):
	out = ""
	out += chr( recipient )
	out += chr( flags )
	out += chr( func_code )
	out += chr( len( data ) )
	out += data
	out += chr( compute_checksum( out ) )
	return out

def parse_packet( packet ):
	(recipient, flags, func_code, payload_length ) = struct.unpack_from( "BBBB", packet )
	data = packet[4:4+payload_length]
	cs = compute_checksum(packet[0:4+payload_length] )
	return (recipient, flags, func_code, data ), cs == ord( packet[-1] )

class AlshainBUS( object ):
	NODE_BROADCAST = 0
	NODE_MASTER = 1

	FUNC_IDENT = 0xff
	
	FLAG_REQUEST = 0
	FLAG_RESPONSE = 1

	def __init__( self, node_id ):
		self.node_id = node_id
		self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		self.sock.connect( ('localhost', 1222) )
		self.sock.setblocking(0)
	
	def transreceive( self ):
		data = None
		try:
			data = self.sock.recv( 1024 )
		except:
			pass
		if data:
			params, is_ok = parse_packet( data )
			if is_ok:
				packet = create_packet( AlshainBUS.NODE_MASTER, AlshainBUS.FLAG_RESPONSE, AlshainBUS.FUNC_IDENT, "python" )
				self.sock.send( packet )
		





#import time
#ab = AlshainBUS( 42 )
#
#try:
#	while True:
#		ab.transreceive()
#		time.sleep( 0.1 )
#except KeyboardInterrupt:
#	pass
