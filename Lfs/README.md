# Lfs: Large File System

Since git does not support large file well and git.lfs requires pay to increase storage and bandwidth (only 1GB/month for free), we have to store the large files on the private server. The default server is ubuntu@d14games.com, and you can change the IP address parameter of py.paramiko in pull.py and push.py to connect to another server. The connection is based on ssh, so you need to ask the administrator of the server to get the password before using.

The usage of Lfs is similar to Git, and there are only 3 operations: **add**, **pull**, **push**.

### Pull remote to local

You need to clone the project with Git firstly:

```
git clone <your-git-project-url>
```

and then change to the project directory to download large files:

```
cd <your-git-project-dir>
py Lfs/pull.py
```

### Push local to remote

You need to edit **attribute.json** to decide which large files to track:

```json
{
    "target": [
        "Dir1",
        "Dir2"
    ],
    "suffix": [
        "jpg",
        "png"
    ]
}
```

where **target** indicates where to search large files, and **suffix** is used to match specific files. Take the file above as an example, if your project directory is named *Proj*, then Lfs will search *Proj/Dir1* and *Proj/Dir2* for the files end with *jpg* and *png*.

After decide the search strategy, you need to notify Lfs to perform the actual searching:

```
py Lfs/add.py
```

and then Lfs will generate **directory.json** and **large_files.json** in place, where **directory.json** includes the recursive directory structure of the large files, and **large_file.json** includes the relative paths of the large files.

Finally you can upload all large files tracked by **large_file.json** to the private server:

```
py Lfs/push.py
```

