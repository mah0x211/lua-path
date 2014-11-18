local readdir = require('path').readdir;

ifNil( readdir( '.' ) );
ifNil( readdir( './' ) );
ifNotNil( readdir( './no_exists/dir' ) );

