local basename = require('path').basename;

ifNotEqual( basename( '/path/to/dir/file.ext' ), 'file.ext' );
ifNotEqual( basename( 'path.to.dir.file.ext' ), 'path.to.dir.file.ext' );

