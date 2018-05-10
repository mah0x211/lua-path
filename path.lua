--[[

  Copyright (C) 2013 Masatoshi Teruya

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.

--]]
local pathc = require('path.pathc');
local select = select;
local tostring = tostring;
local tblconcat = table.concat;
local strgsub = string.gsub;
local strgmatch = string.gmatch;

local function concat( sep, ... )
    local argv = {...};
    local argc = select( '#', ... );
    local res = {};
    local len = 0;

    for i = 1, argc do
        local v = argv[i]
        if v ~= nil then
            len = len + 1;
            res[len] = tostring( v );
        end
    end

    return tblconcat( res, sep );
end


local function normalize( ... )
    local path = concat( '/', ... );
    local res = {};
    local len = 0;

    -- remove double slash
    path = strgsub( path, '/+', '/' );
    for seg in strgmatch( path, '[^/]+' ) do
        if seg == '..' then
            if len > 0 then
                res[len] = nil;
                len = len - 1;
            end
        elseif seg ~= '.' then
            len = len + 1;
            res[len] = seg;
        end
    end

    return '/' .. tblconcat( res, '/' );
end


return {
    normalize = normalize,
    dirname = pathc.dirname,
    basename = pathc.basename,
    extname = pathc.extname,
    exists = pathc.exists,
    stat = pathc.stat,
    toReg = pathc.tofile,   -- deprecated
    toDir = pathc.todir,    -- deprecated
    isReg = pathc.isfile,   -- deprecated
    isDir = pathc.isdir,    -- deprecated
    isChr = pathc.ischr,    -- deprecated
    isBlk = pathc.isblk,    -- deprecated
    isFifo = pathc.isfifo,  -- deprecated
    isLnk = pathc.islnk,    -- deprecated
    isSock = pathc.issock,  -- deprecated
    tofile = pathc.tofile,
    todir = pathc.todir,
    isfile = pathc.isfile,
    isdir = pathc.isdir,
    ischr = pathc.ischr,
    isblk = pathc.isblk,
    isfifo = pathc.isfifo,
    islnk = pathc.islnk,
    issock = pathc.issock,
    readdir = pathc.readdir
};

