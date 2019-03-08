const { execFileSync } = require('child_process');
const path = require('path');

let winlib;
// have to load the lib asap because for some reason the require for native module
// (very rarely) fails (in electron only?) when done later.
// This may be a bug in Vortex exclusively but I have no clue what we might be doing
// to cause that.
if (process.platform === 'win32') {
  winlib = require('./build/Release/wholocks');
}

function linuxGetLocks(checkPath) {
  const { locks } = JSON.parse(execFileSync('lslocks', ['-o', 'command,pid,path', '-J']));
  return locks
    .filter(lock => lock.path === checkPath)
    .map(lock => ({ appName: lock.command, pid: lock.pid }));
}

function wholocks(checkPath) {
  if (process.platform === 'win32') {
    return winlib.wholocks(path.normalize(checkPath));
  } else if (process.platform === 'linux') {
    return linuxGetLocks(path.normalize(checkPath));
  } else {
    throw new Error('Platform not supported');
  }
}

module.exports = {
  default: wholocks,
};
