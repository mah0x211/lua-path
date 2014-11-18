local dirname = require('path').dirname;

ifNotEqual( dirname( '/path/to/dir/file.ext' ), '/path/to/dir' );
ifNotEqual( dirname( 'dir/..' ), 'dir' );
ifNotEqual( dirname( '../file.ext' ), '..' );
