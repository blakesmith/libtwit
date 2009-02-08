#J vim:syntax=python

cflags= ["-ggdb"]
env = Environment(CFLAGS=cflags)
conf = Configure(env)
conf.CheckLib('curl')
if conf.CheckLib('libxml2'):
	conf.env.Append(CFLAGS = ['-I/usr/include/libxml2'])
env = conf.Finish()
env.Library('libtwit', ['stack.c', 'libtwit.c'])
env.Program('test_app.c', LIBS=['libtwit', 'libxml2', 'libcurl'], LIBPATH=['.'])
