function wholocks(checkPath) {
    if (process.platform === 'win32') {
      let lib = require('./build/Release/wholocks');
      return lib.wholocks(checkPath);
    } // not implemented on other platforms yet
  }
  
  module.exports = {
    default: wholocks,
  }
  