lua-path
========

path string manipulation

## Installation

```sh
luarocks install --from=http://mah0x211.github.io/rocks/ path
```


## Functions

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
2. errstr: dependent on a system.


### path = basename( path )

return a filename or directory portion of path string.

**Parameters**

- path: path string.

**Returns**

1. path: filename or directory portion of path string.
2. errstr: dependent on a system.


### ext = extname( path )

return a extension of the path string.

**Parameters**

- path: path string.

**Returns**

1. path: extension of the path string.


### path, errstr = exists( path )

returns the canonicalized absolute pathname.

**Parameters**

- path: path string.

**Returns**

1. path: returns the canonicalized absolute pathname or nil on error or not exists.
2. errstr: dependent on a system.


### info, errstr = stat( path [, followSymlink [,  openfd]] )

return a information about the file pointed to by specified path.

**Parameters**

- path: path string.
- followSymlink: follow symbolic links (default: true).
- openfd: open a file descriptor (default: false).


**Returns**

1. info: info table (same as struct stat without st_ prefix), nil on error or not exists.
2. errstr: dependent on a system.


### path = tofile( path )

returns the absolute path if it is regular file.

**Parameters**

- path: path string.

**Returns**

1. path: absolute path string or nil on error or not exists.
2. errstr: dependent on a system.


### path = todir( path )

returns the absolute path if it is directory.

**Parameters**

- path: path string.

**Returns**

1. path: absolute path string or nil on error or not exists.
2. errstr: dependent on a system.


### bool = isfile( info.mode )

returns a true if it is regular file.

### bool = isdir( info.mode )

returns a true if it is directory.

### bool = ischr( info.mode )

returns a true if it is character device.

### bool = isblk( info.mode )

returns a true if it is block device.

### bool = isfifo( info.mode )

returns a true if it is FIFO special file, or a pipe.

### bool = islnk( info.mode )

returns a true if it is symbolic link.

### bool = issock( info.mode )

returns a true if it is socket.


### entries, errstr = readdir( path )

return entries of specified directory path.

**Parameters**

- path: directory path string.

**Returns**

1. entries: entries table, nil on error or not exists.
2. errstr: dependent on a system.

