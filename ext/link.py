from subprocess import *
import locale
Cprocess = None

def link_start():
    global Cprocess
    Cprocess = Popen('./main',shell=False,stdin=PIPE,stdout=PIPE)

def link_command(s):
    global Cprocess
    s=s+"\n"
    Cprocess.stdin.write(s.encode())
    Cprocess.stdin.flush()

def link_read():
    global Cprocess
    s=Cprocess.stdout.readline()
    print(s)
    s=s.decode('cp936')
    
    if(s=="Invalid"):
        raise Exception("Invalid!")
    return s
