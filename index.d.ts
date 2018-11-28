declare module "wholocks" {
    export interface ProcessInfo {
      pid: number;
      appName: string;
    }

    function wholocks(filePath: string): ProcessInfo[];
  
    export default wholocks;
}
