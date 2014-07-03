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
#include <sys/stat.h>
#include <lauxlib.h>
#include <lualib.h>

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

static inline const char *rlindex( const char *str, size_t len, int c )
{
    size_t pos = len;
    
    while( --pos > 0 )
    {
        if( str[pos] == c ){
            return str + pos + 1;
        }
    }
    
    if( *str == c ){
        return str + 1;
    }
    
    return str;
}

static int dirname_lua( lua_State *L )
{
    size_t len = 0;
    const char *path = luaL_checklstring( L, 1, &len );
    const char *ptr = rlindex( path, len, '/' );
    
    if( ptr == path ){
        lua_pushlstring( L, "", 0 );
    }
    else {
        len = (ptrdiff_t)ptr - (ptrdiff_t)path - 1;
        lua_pushlstring( L, path, len ? len : 1 );
    }
    
    return 1;
}

static int basename_lua( lua_State *L )
{
    size_t len = 0;
    const char *path = luaL_checklstring( L, 1, &len );
    
    if( path[len-1] == '/' ){
        len--;
        ((char*)path)[len] = 0;
    }
    
    lua_pushstring( L, rlindex( path, len, '/' ) );
    
    return 1;
}


static int extname_lua( lua_State *L )
{
    size_t len = 0;
    const char *path = luaL_checklstring( L, 1, &len );
    const char *ptr = NULL;
    const char *ext = NULL;
    
    if( path[len-1] == '/' ){
        len--;
        ((char*)path)[len] = 0;
    }
    ptr = rlindex( path, len, '/' );
    ext = rlindex( ptr, len - ( (ptrdiff_t)ptr - (ptrdiff_t)path ), '.' );
    
    lua_pushstring( L, ext != ptr ? ext - 1 : "" );
    
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
    
    // got error
    lua_pushnil(L);
    lua_pushnumber( L, errno );
    
    return 2;
}


static int stat_lua( lua_State *L )
{
    size_t len = 0;
    const char *path = luaL_checklstring( L, 1, &len );
    struct stat info = {0};
    
    if( stat( path, &info ) == 0 ){
        // set fields
        lua_newtable( L );
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
        return 1;
    }
    
    // got error
    lua_pushnil(L);
    lua_pushnumber( L, errno );
    
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
        { "isReg", isreg_lua },
        { "isDir", isdir_lua },
        { "isChr", ischr_lua },
        { "isBlk", isblk_lua },
        { "isFifo", isfifo_lua },
        { "isLnk", islnk_lua },
        { "isSock", issock_lua },
        { NULL, NULL }
    };
    int i = 0;
    
    // create protected-table
    lua_newtable( L );
    // create __metatable
    lua_newtable( L );
    // create substance
    lua_pushstring( L, "__index" );
    lua_newtable( L );
    
    // set functions
    for( i = 0; funcs[i].name; i++ ){ 
        lua_pushstring( L, funcs[i].name );
        lua_pushcfunction( L, funcs[i].func );
        lua_rawset( L, -3 );
    }
    
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

