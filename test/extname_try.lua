local extname = require('path').extname;

ifNotEqual( extname( '/path/to/dir/file.ext' ), '.ext' );
ifNotNil( extname( '/path/to/dir/file_ext' ) );
