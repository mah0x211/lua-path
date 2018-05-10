/*
 *  pathc.c
 *  Created by Masatoshi Teruya on 13/11/25.
 *
 *  Copyright (C) 2013 Masatoshi Teruya
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *  THE SOFTWARE.
 */

#include <stdlib.h>
#include <stddef.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <libgen.h>
#include <lauxlib.h>
#include <lualib.h>

#define lstate_fn2tbl(L,k,v) do{ \
    lua_pushstring(L,k); \
    lua_pushcfunction(L,v); \
    lua_rawset(L,-3); \
}while(0)

#define lstate_num2tbl(L,k,v) do{ \
    lua_pushstring(L,k); \
    lua_pushnumber(L,v); \
    lua_rawset(L,-3); \
}while(0)

#define lstate_bool2tbl(L,k,v) do{ \
    lua_pushstring(L,k); \
    lua_pushboolean(L,v); \
    lua_rawset(L,-3); \
}while(0)

#define lstate_str2tbl(L,k,v) do{ \
    lua_pushstring(L,k); \
    lua_pushstring(L,v); \
    lua_rawset(L,-3); \
}while(0)

#define lstate_str2arr(L,v,i) do{ \
    lua_pushstring(L,v); \
    lua_rawseti(L,-2,i); \
}while(0)


static int dirname_lua( lua_State *L )
{
    size_t len = 0;
    const char *path = luaL_checklstring( L, 1, &len );
    char *buf = strndup( path, len );

    if( buf ){
        lua_pushstring( L, dirname( buf ) );
        free( buf );
        return 1;
    }

    // got error
    lua_pushnil( L );
    lua_pushstring( L, strerror( errno ) );

    return 2;
}


static int basename_lua( lua_State *L )
{
    size_t len = 0;
    const char *path = luaL_checklstring( L, 1, &len );
    char *buf = strndup( path, len );

    if( buf ){
        lua_pushstring( L, basename( buf ) );
        free( buf );
        return 1;
    }

    // got error
    lua_pushnil( L );
    lua_pushstring( L, strerror( errno ) );

    return 2;
}


static int extname_lua( lua_State *L )
{
    size_t len = 0;
    const char *path = luaL_checklstring( L, 1, &len );
    const char *ext = NULL;

    if( !len || !( ext = rindex( path, '.' ) ) || index( ext, '/' ) ){
        lua_pushnil( L );
    }
    else {
        lua_pushlstring( L, ext, len - ( ext - path ) );
    }

    return 1;
}


static int exists_lua( lua_State *L )
{
    size_t len = 0;
    const char *path = luaL_checklstring( L, 1, &len );
    char *rpath = realpath( path, NULL );

    if( rpath ){
        lua_pushstring( L, rpath );
        free( rpath );
        return 1;
    }
    // not exists
    else if( errno == ENOENT ){
        return 0;
    }

    // got error
    lua_pushnil(L);
    lua_pushstring( L, strerror( errno ) );

    return 2;
}


static inline int pathof_lua( lua_State *L, mode_t m )
{
    size_t len = 0;
    const char *path = luaL_checklstring( L, 1, &len );
    char *rpath = realpath( path, NULL );
    int rc = -1;

    if( rpath )
    {
        struct stat info = {0};

        if( stat( path, &info ) == 0 )
        {
            if( ( info.st_mode & S_IFMT ) == m ){
                lua_pushstring( L, rpath );
                rc = 1;
            }
            else {
                rc = 0;
            }
        }

        free( rpath );
    }

    switch( rc )
    {
        case 0:
            lua_pushnil( L );
        case 1:
            return 1;
        // got error
        default:
            // not exists
            if( errno == ENOENT ){
                return 0;
            }

            lua_pushnil(L);
            lua_pushstring( L, strerror( errno ) );
            return 2;
    }
}

static int toreg_lua( lua_State *L )
{
    return pathof_lua( L, S_IFREG );
}

static int todir_lua( lua_State *L )
{
    return pathof_lua( L, S_IFDIR );
}


typedef int (*statfn_t)(const char *, struct stat *);

static int stat_lua( lua_State *L )
{
    int argc = lua_gettop( L );
    size_t len = 0;
    const char *path = luaL_checklstring( L, 1, &len );
    struct stat info = {0};
    statfn_t statfn = stat;
    int flgs = O_RDONLY|O_CLOEXEC;
    int openfd = 0;

    // check argument
    if( argc > 3 ){
        argc = 3;
    }
    switch( argc )
    {
        // fd option
        case 3:
            if( !lua_isnoneornil( L, 3 ) ){
                luaL_checktype( L, 3, LUA_TBOOLEAN );
                openfd = lua_toboolean( L, 3 );
            }

        // follow symlinks option: default true
        case 2:
            if( !lua_isnoneornil( L, 2 ) )
            {
                luaL_checktype( L, 2, LUA_TBOOLEAN );
                // false then not follow symlinks
                if( !lua_toboolean( L, 2 ) ){
                    statfn = lstat;
                    flgs |= O_NOFOLLOW;
                }
            }
        break;
    }

    if( openfd )
    {
        if( ( openfd = open( path, flgs, 0000644 ) ) == -1 ){
            goto STAT_FAILURE;
        }
        else if( fstat( openfd, &info ) == -1 ){
            close( openfd );
            goto STAT_FAILURE;
        }
        goto STAT_SUCCESS;
    }
    else if( statfn( path, &info ) == 0 )
    {
STAT_SUCCESS:
        // set fields
        lua_newtable( L );
        // add descriptor
        if( openfd ){
            lstate_num2tbl( L, "fd", openfd );
        }
        lstate_num2tbl( L, "dev", info.st_dev );
        lstate_num2tbl( L, "ino", info.st_ino );
        lstate_num2tbl( L, "mode", info.st_mode );
        lstate_num2tbl( L, "nlink", info.st_nlink );
        lstate_num2tbl( L, "uid", info.st_uid );
        lstate_num2tbl( L, "gid", info.st_gid );
        lstate_num2tbl( L, "rdev", info.st_rdev );
        lstate_num2tbl( L, "size", info.st_size );
        lstate_num2tbl( L, "blksize", info.st_blksize );
        lstate_num2tbl( L, "blocks", info.st_blocks );
        lstate_num2tbl( L, "atime", info.st_atime );
        lstate_num2tbl( L, "mtime", info.st_mtime );
        lstate_num2tbl( L, "ctime", info.st_ctime );
        switch( info.st_mode & S_IFMT ){
            // regular file
            case S_IFREG:
                lstate_str2tbl( L, "type", "reg" );
            break;
            // directory
            case S_IFDIR:
                lstate_str2tbl( L, "type", "dir" );
            break;
            // symbolic link
            case S_IFLNK:
                lstate_str2tbl( L, "type", "lnk" );
            break;
            // char special
            case S_IFCHR:
                lstate_str2tbl( L, "type", "chr" );
            break;
            // block special
            case S_IFBLK:
                lstate_str2tbl( L, "type", "blk" );
            break;
            // socket
            case S_IFSOCK:
                lstate_str2tbl( L, "type", "sock" );
            break;
            // fifo or socket
            case S_IFIFO:
                lstate_str2tbl( L, "type", "fifo" );
            break;
        }

        return 1;
    }

STAT_FAILURE:
    // got error
    if( errno == ENOENT ){
        return 0;
    }

    lua_pushnil(L);
    lua_pushstring( L, strerror( errno ) );

    return 2;
}


#define push_modetype(L,t,v)    lua_pushboolean(L,t(v))

static int isreg_lua( lua_State *L ){
    push_modetype( L, S_ISREG, luaL_checkinteger( L, 1 ) );
    return 1;
}
static int isdir_lua( lua_State *L ){
    push_modetype( L, S_ISDIR, luaL_checkinteger( L, 1 ) );
    return 1;
}
static int ischr_lua( lua_State *L ){
    push_modetype( L, S_ISCHR, luaL_checkinteger( L, 1 ) );
    return 1;
}
static int isblk_lua( lua_State *L ){
    push_modetype( L, S_ISBLK, luaL_checkinteger( L, 1 ) );
    return 1;
}
static int isfifo_lua( lua_State *L ){
    push_modetype( L, S_ISFIFO, luaL_checkinteger( L, 1 ) );
    return 1;
}
static int islnk_lua( lua_State *L ){
    push_modetype( L, S_ISLNK, luaL_checkinteger( L, 1 ) );
    return 1;
}
static int issock_lua( lua_State *L ){
    push_modetype( L, S_ISSOCK, luaL_checkinteger( L, 1 ) );
    return 1;
}


static int readdir_lua( lua_State *L )
{
    size_t len = 0;
    const char *path = luaL_checklstring( L, 1, &len );
    DIR *dir = opendir( path );

    if( dir )
    {
        struct dirent *entry = NULL;
        int i = 0;

        lua_newtable( L );
        errno = 0;
        while( ( entry = readdir( dir ) ) ){
            lstate_str2arr( L, entry->d_name, ++i );
        }

        closedir( dir );
        if( errno ){
            lua_pop( L, 1 );
        }
        else {
            return 1;
        }
    }

    // got error
    if( errno == ENOENT ){
        return 0;
    }
    lua_pushnil(L);
    lua_pushstring( L, strerror( errno ) );

    return 2;
}


// make error
static int const_newindex( lua_State *L ){
    return luaL_error( L, "attempting to change protected module" );
}

LUALIB_API int luaopen_path_pathc( lua_State *L )
{
    struct luaL_Reg funcs[] = {
        { "dirname", dirname_lua },
        { "basename", basename_lua },
        { "extname", extname_lua },
        { "exists", exists_lua },
        { "stat", stat_lua },
        { "tofile", toreg_lua },
        { "todir", todir_lua },
        { "isfile", isreg_lua },
        { "isdir", isdir_lua },
        { "ischr", ischr_lua },
        { "isblk", isblk_lua },
        { "isfifo", isfifo_lua },
        { "islnk", islnk_lua },
        { "issock", issock_lua },
        { "readdir", readdir_lua },
        { NULL, NULL }
    };
    struct luaL_Reg *ptr = funcs;

    // create protected-table
    lua_newtable( L );
    // create __metatable
    lua_newtable( L );
    // create substance
    lua_pushstring( L, "__index" );
    lua_newtable( L );

    // set functions
    do {
        lstate_fn2tbl( L, ptr->name, ptr->func );
        ptr++;
    } while( ptr->name );

    // set substance to __metable.__index field
    lua_rawset( L, -3 );
    // set __newindex function to __metable.__newindex filed
    lua_pushstring( L, "__newindex" );
    lua_pushcfunction( L, const_newindex );
    lua_rawset( L, -3 );
    // convert protected-table to metatable
    lua_setmetatable( L, -2 );

    return 1;
}

