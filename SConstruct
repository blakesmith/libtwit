#J vim:syntax=python

cflags= ["-ggdb"]
env = Environment(CFLAGS=cflags)
conf = Configure(env)
conf.CheckLib('curl')
if conf.CheckLib('libxml2'):
	conf.env.Append(CFLAGS = ['-I/usr/include/libxml2'])
env = conf.Finish()

libtwit = env.SharedLibrary('libtwit', ['src/core.c', 'src/status.c', 'src/account.c'])
test_app = env.Program('test_app/test_app.c', LIBS=['libtwit'], LIBPATH=['.'])

env.Install('/usr/local/bin', test_app)
env.Install('/usr/local/lib', libtwit)
env.Alias('install', ['/usr/local/bin', '/usr/local/lib'])

