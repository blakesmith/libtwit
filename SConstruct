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

cfiles = glob.glob("src/*.c")
libtwit = env.SharedLibrary('libtwit', cfiles)

if len(sys.argv) > 1:
	if sys.argv[1] == "install":
		do_header()

env.Install('/usr/local/lib', libtwit)
env.Alias('install', ['/usr/local/lib', '/usr/local/include'])

