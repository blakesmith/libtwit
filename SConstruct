#J vim:syntax=python

cflags= []
env = Environment(CFLAGS=cflags)
conf = Configure(env)
conf.CheckLib('curl')
if conf.CheckLib('libxml2'):
	conf.env.Append(CFLAGS = ['-I/usr/include/libxml2'])
env = conf.Finish()
env.Program('libtwit.c')
