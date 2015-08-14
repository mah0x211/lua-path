local path = require('path');
local stat = ifNil( path.stat( 'stat_try.lua' ) );

ifNotEqual( stat.type, 'reg' );

stat = ifNil( path.stat( './' ) );
ifNotEqual( stat.type, 'dir' );
ifNotNil( stat.fd );

stat = ifNil( path.stat( './', false, false ) );
ifNotNil( stat.type );
ifNotNil( stat.fd );

stat = ifNil( path.stat( './', false, nil ) );
ifNil( stat.type );
ifNotNil( stat.fd );

stat = ifNil( path.stat( 'stat_try.lua', false, true, true ) );
ifNil( stat.type );
ifNil( stat.fd );

ifNotNil( path.stat( 'stat_try.lua_' ) );


