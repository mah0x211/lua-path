local cwd = require('process').getcwd();
local toReg = require('path').toReg;

ifNotEqual( toReg( 'toReg_try.lua' ), cwd .. '/toReg_try.lua' );
