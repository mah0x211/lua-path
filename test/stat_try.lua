local path = require('path');
local stat = ifNil( path.stat( 'stat_try.lua' ) );

ifNotEqual( stat.type, 'reg' );

stat = ifNil( path.stat( './' ) );
ifNotEqual( stat.type, 'dir' );

stat = ifNil( path.stat( './', false, false ) );
ifNotNil( stat.type );

ifNotNil( path.stat( 'stat_try.lua_' ) );


