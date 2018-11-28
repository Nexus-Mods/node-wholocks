declare module "wholocks" {
    interface ProcessInfo {
      pid: number;
      appName: string;
    }

    function wholocks(filePath: string): Bluebird<Array<ProcessInfo>>;
  
    export default wholocks;
  }
}
