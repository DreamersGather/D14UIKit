import os
import time
import json
import paramiko

# compatible with proj dir
if not os.getcwd().endswith('Lfs'):
    os.chdir('Lfs')

dirtree = dict()
with open('directory.json', 'r') as f:
    dirtree = json.load(f)

filetree = dict()
with open('large_file.json', 'r') as f:
    filetree = json.load(f)
filelist = filetree['large_file']

sc = paramiko.SSHClient()
sc.set_missing_host_key_policy(paramiko.AutoAddPolicy())

password = input('ubuntu@d14games.com password: ')
try:
    sc.connect('d14games.com', 22, 'ubuntu', password)
except:
    print('Failed to connect to the remote server.')
    print('Please ask the administrator to see whether the server is working;')
    print('check your password and input it again to reconnect to the server.')
    exit(-1)

sftp = paramiko.SFTPClient.from_transport(sc.get_transport())

def printProgressBar(name, curr, total, length = 50):
    pernum = 0
    pernum_comp = 0
    if curr < total - 1:
        pernum = curr * length // total
        pernum_comp = length - pernum
    else:
        pernum = length
        pernum_comp = 0
    fnpart = '#' * pernum
    rmpart = '-' * pernum_comp
    perstr = str(curr + 1) + '/' + str(total)
    print('{}: [{}{}] {:>7}\r'.format(name, fnpart, rmpart, perstr), end='')

projname = os.path.basename(os.path.abspath('..'))
projpath = '/home/ubuntu/' + projname + '/'

diridx = 0
dirnum = 0

def get_num(currNode):
    dirnum = len(currNode)
    for key in currNode:
        dirnum = dirnum + get_num(currNode[key])
    return dirnum

dirnum = get_num(dirtree)

def make_dirs(currNode, currPath):
    global diridx
    global dirnum
    for key in currNode:
        nextPath = currPath + key + '/'
        try:
            os.mkdir(nextPath)
        except:
            pass # if local directory already exists
        printProgressBar('Directories', diridx, dirnum)
        diridx = diridx + 1
        # make all children directories recursively
        make_dirs(currNode[key], nextPath)

print('\nStart making directories...\n')
start = time.time()

make_dirs(dirtree, '../')

end = time.time()
print('\nTotal time: {:.2f}s'.format(end - start))

print('\nStart downloading large files...\n')
start = time.time()

for fileidx in range(len(filelist)):
    # download file
    filepath = filelist[fileidx]
    sftp.get(projpath + filepath, '../' + filepath)
    # progress bar
    printProgressBar('Large Files', fileidx, len(filelist))

end = time.time()
print('\nTotal time: {:.2f}s\n'.format(end - start))

sftp.close()
