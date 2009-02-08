#J vim:syntax=python

cflags= ["-ggdb"]
env = Environment(CFLAGS=cflags)
conf = Configure(env)
conf.CheckLib('curl')
if conf.CheckLib('libxml2'):
	conf.env.Append(CFLAGS = ['-I/usr/include/libxml2'])
env = conf.Finish()
libtwit = env.SharedLibrary('libtwit', ['src/stack.c', 'src/libtwit.c'])
env.Program('test_app.c', LIBS=['libtwit'], LIBPATH=['.'])

env.Install('/usr/local/lib', libtwit)
env.Alias('install', '/usr/local/lib')
