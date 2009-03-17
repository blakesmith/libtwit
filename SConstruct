#J vim:syntax=python
import os
import glob
import sys

cflags= ["-ggdb"]
env = Environment(CFLAGS=cflags)
conf = Configure(env)
conf.CheckLib('curl')
if conf.CheckLib('libxml2'):
	conf.env.Append(CFLAGS = ['-I/usr/include/libxml2'])
env = conf.Finish()

def do_header():
	header_dir = '/usr/local/include/libtwit'
	if not os.path.exists(header_dir):
		os.mkdir(header_dir)
	headers = glob.glob("src/*.h")
	for header in headers:
		env.Install(header_dir, header)

libtwit = env.SharedLibrary('libtwit', ['src/core.c', 'src/status.c', 'src/account.c'])
test_app = env.Program('test_app/test_app.c', LIBS=['libtwit'], LIBPATH=['.'])

if len(sys.argv) > 1:
	if sys.argv[1] == "install":
		do_header()

env.Install('/usr/local/bin', test_app)
env.Install('/usr/local/lib', libtwit)
env.Alias('install', ['/usr/local/bin', '/usr/local/lib', '/usr/local/include'])

