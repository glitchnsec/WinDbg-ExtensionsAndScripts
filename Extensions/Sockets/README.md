Debugger.Utility.Socket
----

```
This interface is under active development and will change.
```

- Problem Statement
  ---
  - Currently, only scripts/extensions written with C/C++ and PyKd have access to native sockets for network interaction during a debug session.
  
  - JavaScript is the most fun method of scripting and extending WinDbg debug data model

- Solution
  ---
  - Implement a Socket Namespace similar to the FileSystem NameSpace under `Debugger.Utility`
  
  - ```ini
    0:000> dx Debugger
    Debugger                
        Sessions        
        Settings        
        State           
        Utility         
    0:000> dx -r1 Debugger.Utility
    Debugger.Utility                
        Collections     
        Control         
        Objects         
        DbgDocumentation
        FileSystem      
        Code            
        Analysis  
    ```
  - It will be nice to have

    ```ini
    0:000> dx Debugger
    Debugger                
        Sessions        
        Settings        
        State           
        Utility         
    0:000> dx -r1 Debugger.Utility
    Debugger.Utility                
        Collections     
        Control         
        Objects         
        DbgDocumentation
        FileSystem      
        Code            
        Analysis  
        Socket
    0:000> dx -r1 Debugger.Utility.Socket
    Debugger.Utility.Socket                
        Socket       [Socket(family, type) - Creates a Socket.]
        ...
    ```

  - This repository seeks to provide that

  - Chances are the WinDbg debug team at Microsoft may finish this before I do, but the race is on!