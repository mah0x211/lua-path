lua-path
========

path string manipulation

## Installation

```sh
luarocks install --from=http://mah0x211.github.io/rocks/ path
```

or 

```sh
git clone https://github.com/mah0x211/lua-path.git
cd lua-path
luarocks make
```


## functions

### path = normalize( str, str[, ...] )

returns a normalized path string.

**Parameters**

- path: path components.


**Returns**

1. path: normalized absolute path string.


### path = dirname( path )

return a directory portion of path string.

**Parameters**

- path: path string.


**Returns**

1. path: directory portion of path string.
2. errno: dependent on a system.


### path = basename( path )

return a filename or directory portion of path string.

**Parameters**

- path: path string.

**Returns**

1. path: filename or directory portion of path string.
2. errno: dependent on a system.


### ext = extname( path )

return a extension of the path string.

**Parameters**

- path: path string.

**Returns**

1. path: extension of the path string.


### path, errno = exists( path )

returns the canonicalized absolute pathname.

**Parameters**

- path: path string.

**Returns**

1. path: returns the canonicalized absolute pathname.

2. errno: dependent on a system.


### info, errno = stat( path )

return a information about the file pointed to by specified path.

**Parameters**

- path: path string.

**Returns**

1. info: info table. (same as struct stat without st_ prefix)

2. errno: dependent on a system.


### bool = isReg( info.mode )

returns a true if it is regular file.

### bool = isDir( info.mode )

returns a true if it is directory.

### bool = isChr( info.mode )

returns a true if it is character device.

### bool = isBlk( info.mode )

returns a true if it is block device.

### bool = isFifo( info.mode )

returns a true if it is FIFO special file, or a pipe.

### bool = isLnk( info.mode )

returns a true if it is symbolic link.

### bool = isSock( info.mode )

returns a true if it is socket.


### entries, errno = readdir( path )

return entries of specified directory path.

**Parameters**

- path: directory path string.

**Returns**

1. entries: entries table.

2. errno: dependent on a system.

