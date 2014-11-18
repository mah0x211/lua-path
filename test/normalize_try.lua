local normalize = require('path').normalize;

ifNotEqual( normalize(), '/' );
ifNotEqual( normalize( nil, '/', 'test', nil, 'test' ), '/test/test' );

ifNotEqual( normalize('/path/to/dir/file'), '/path/to/dir/file' );
ifNotEqual( normalize( 'path', 'to', 'dir', 'file' ), '/path/to/dir/file' );

ifNotEqual( normalize( '..//path/to/dir/file' ), '/path/to/dir/file' );
ifNotEqual( normalize( '..', '//path/to/dir/file' ), '/path/to/dir/file' );

ifNotEqual( normalize( '/path/to/dir/file/../../' ), '/path/to' );
ifNotEqual( normalize( '/path/to/dir/file/', '..', '..' ), '/path/to' );

ifNotEqual( normalize( '/path/../to/dir/file' ), '/to/dir/file' );
ifNotEqual( normalize( 'path', '../to', '/dir/file' ), '/to/dir/file' );

ifNotEqual( normalize( '/path/../to/dir/../../../file'), '/file' );
ifNotEqual( normalize( 'path', '..', 'to', 'dir', '..', '..', '..', 'file'), '/file' );
