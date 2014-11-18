local cwd = require('process').getcwd();
local toDir = require('path').toDir;

ifNotEqual( toDir( '.' ), cwd );
ifNotEqual( toDir( './' ), cwd );
