local path = require('path');
local stat = ifNil( path.stat( 'stat_try.lua' ) );

ifNotEqual( stat.type, 'reg' );

stat = ifNil( path.stat( './' ) );
ifNotEqual( stat.type, 'dir' );

ifNotNil( path.stat( 'stat_try.lua_' ) );

stat = ifNil( path.stat( 'stat_try.lua' ) );
ifNotEqual( stat.type, 'reg' );
ifNotNil( stat.fd );

stat = ifNil( path.stat( 'stat_try_sym.lua' ) );
ifNotEqual( stat.type, 'reg' );


stat = ifNil( path.stat( 'stat_try_sym.lua' ) );
ifNotEqual( stat.type, 'reg' );


stat = ifNil( path.stat( 'stat_try_sym.lua', false ) );
ifNotEqual( stat.type, 'lnk' );


stat = ifNotNil( path.stat( 'stat_try_sym.lua', false, true ) );

stat = ifNil( path.stat( 'stat_try_sym.lua', true, true ) );
ifNotEqual( stat.type, 'reg' );
ifNil( stat.fd );



