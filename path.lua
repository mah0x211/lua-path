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


local function concat( sep, ... )
    local argv = {...};
    local i, v = next( argv );
    local res = {};
    local len = 0;
    
    while i do
        if v ~= nil then
            len = len + 1;
            res[len] = tostring( v );
        end
        i, v = next( argv, i );
    end
    
    return table.concat( res, sep );
end


local function normalize( ... )
    local path = concat( '/', ... );
    local res = {};
    local len = 0;
    
    -- remove double slash
    path = path:gsub( '/+', '/' );
    for seg in string.gmatch( path, '[^/]+' ) do
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
    
    return '/' .. table.concat( res, '/' );
end


return {
    normalize = normalize,
    dirname = pathc.dirname,
    basename = pathc.basename,
    extname = pathc.extname,
    exists = pathc.exists,
    stat = pathc.stat,
    toReg = pathc.toReg,
    toDir = pathc.toDir,
    isReg = pathc.isReg,
    isDir = pathc.isDir,
    isChr = pathc.isChr,
    isBlk = pathc.isBlk,
    isFifo = pathc.isFifo,
    isLnk = pathc.isLnk,
    isSock = pathc.isSock,
    readdir = pathc.readdir
};

