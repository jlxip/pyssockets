#!/usr/bin/env python

import pyssockets
import socket

def init(ctx):
	ctx.data = bytes()
	ctx.state = 1
	return pyssockets.RET_OK

def recv(ctx):
	try:
		ctx.data += ctx.socket.recv(1024 - len(ctx.data))
	except BlockingIOError:
		return pyssockets.RET_READ
	except:
		return pyssockets.RET_ERROR

	if len(ctx.data) < 1024 and (len(ctx.data) and ctx.data[-1] != ord('\n')):
		return pyssockets.RET_READ

	ctx.state = 2
	return pyssockets.RET_OK

def send(ctx):
	try:
		ctx.socket.send(ctx.data)
	except:
		return pyssockets.RET_ERROR

	# Again
	return init(ctx)

pyssockets.addState(init)
pyssockets.addState(recv)
pyssockets.addState(send)
pyssockets.run('0.0.0.0', 4444, 0);
