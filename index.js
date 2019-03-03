const { execFileSync } = require('child_process');
const path = require('path');

function linuxGetLocks(checkPath) {
  const { locks } = JSON.parse(execFileSync('lslocks', ['-o', 'command,pid,path', '-J']));
  return locks
    .filter(lock => lock.path === checkPath)
    .map(lock => ({ appName: lock.command, pid: lock.pid }));
}

function wholocks(checkPath) {
  if (process.platform === 'win32') {
    const lib = require('./build/Release/wholocks');
    return lib.wholocks(path.normalize(checkPath));
  } else if (process.platform === 'linux') {
    return linuxGetLocks(path.normalize(checkPath));
  } else {
    throw new Error('Platform not supported');
  }
}

module.exports = {
  default: wholocks,
};
