local cwd = require('process').getcwd()
local exists = require('path').exists;

ifNotEqual( exists( './exists_try.lua' ), cwd .. '/exists_try.lua' );
ifNotNil( exists( './_exists_try.lua' ) );
