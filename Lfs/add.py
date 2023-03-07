import os
import json

# compatible with proj dir
if not os.getcwd().endswith('Lfs'):
    os.chdir('Lfs')

target = tuple()
suffix = tuple()
with open('attribute.json', 'r') as f:
    root = json.load(f)
    target = tuple(root['target'])
    suffix = tuple(root['suffix'])

dirtree = dict()
filetree = {'large_file': []}
filelist = filetree['large_file']

def updateDirtree(dirtree, newDirpath):
    currNode = dirtree
    pathTree = newDirpath.split('/')
    for dirname in pathTree:
        if dirname not in currNode:
            currNode[dirname] = dict()
        currNode = currNode[dirname]

for dirpath, dirnames, filenames in os.walk(".."):
    # use linux style '/' for compability
    newDirpath = dirpath.replace('\\', '/').lstrip('../')
    if newDirpath.startswith(target):
        lfsFound = False
        for filename in filenames:
            if filename.endswith(suffix):
                lfsFound = True
                filelist.append(newDirpath + '/' + filename)
        if lfsFound:
            updateDirtree(dirtree, newDirpath)

with open('directory.json', 'w') as f:
    json.dump(dirtree, f, indent=4, sort_keys=True)

with open('large_file.json', 'w') as f:
    json.dump(filetree, f, indent=4, sort_keys=True)
