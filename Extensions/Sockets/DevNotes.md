- May 21st 2020
  ---
  ### Time Information
  || Session 1| Session2 |
  |:---|---:|----:|
  | Start Time | 9:46 AM||
  | End Time| 11:05 AM||
  | Duration | 1 HR||

  ### Resources Discovered or Used Today:

  ### Design Decisions

  ### Notes
  - Not exactly the first day I've worked on this but the first day I have started with a plan
  - Essentially, it's been a while I did anything with C++. So I have decided to read and completely
    understand the HelloWorld sample Microsoft provided before proceeding any further.
  - To do confirm my understand, I will, comment the entire HelloWorld code and once I'm done, return
    to coding this project.
  - See the Section "Hello World Notes" in the daily diaries

  ### Hello World Notes
  - The first step is building the Hello World Notes and confirming the code works as advertised
  - Commented all the non-standard code in `SimpleIntroExtension.cpp`
  - Unable to compile sample due to `'ExtratString':  is not a member of 'Debugger:DataModel::ClientEx::Details'`
  - Commented on [GitHub](https://github.com/microsoft/WinDbg-Samples/issues/15) and awaiting a response

- May 23rd 2020
  ---
  ### Time Information
  || Session 1| Session2 |
  |:---|---:|----:|
  | Start Time | 6:00 AM| 8:00 AM|
  | End Time| 8:00 AM| 10:00 AM |
  | Duration | 2 HR| 2 HR|

  ### Resources Discovered or Used Today:

  ### Design Decisions

  ### Notes
  - Still commenting the SimpleIntroClientLibrary
  - Last time I commented all of SimpleIntroExtension.cpp
  - Last time, I was stuck at: Unable to compile sample due to `'ExtratString':  is not a member of 'Debugger:DataModel::ClientEx::Details'`
  - My question on [Github](https://github.com/microsoft/WinDbg-Samples/issues/15) was answered but sadly not a response I wanted to here
  - See the Section "Hello World Notes" in the daily diaries

  ### Hello World Notes
  - Still trying to fix project build
  - Commented all the non-standard code in `SimpleIntro.h` - Nothing magical to comment
  - The purpose of `SimpleIntro.h`
    - This file servers as the base file for all the `includes` needed in the project such that every `.cpp` may just include it.
        - There's a section for the standard windows headers required for debugger extensions
        - There's a section for the more specific custom header files defining classes and namespaces relevant for our extension
  - The purpose of `SimpleIntroExtension.cpp`
    - This file serves as the entry point of the debug extension. It initializes various global references relevant to the functionality it provides
        - There's standard code for loading and unloading the extension
        - There's code for the initialization of the class the extension will be providing
        - Firstly, the code declares some global pointers that will be initialized in the extension loading
        - Next it extends the `Debugger::DataModel::ClientEx` namespace with Getter methods for the global pointers
        - Next it extends the `Debugger::DataModel::Libraries` namespace with a constructor of the ExtensionProvider class that creates a singleton
          of the Extension in this case `HelloProvider`.
  - I was able to fix the issue with project building. Just had to move some lines in `DbgModelClientEx.h`. I posted my response to the original
    question on [GitHub](https://github.com/microsoft/WinDbg-Samples/issues/15#issuecomment-633028477)
  - There's also an issue by `@zodiacon` on [WinDbg-Libraries GitHub](https://github.com/microsoft/WinDbg-Libraries/issues/2) related to this
  - Commented all the non-standard code in `HelloProvider.cpp`
    - This file serves as
        - Theres's code to extend the `Debugger::DataModel::Libraries::Hello` and `Debugger::DataModel::ClientEx::Boxing`
        - Hello class methods declared in the header file `HelloProvider.h` are implemented
        - Implements the extension of the Process Debugger Model

- May 25th 2020
  ---
  ### Time Information
  || Session 1| Session2 |
  |:---|---:|----:|
  | Start Time | 12:30 AM|11:00 PM|
  | End Time| 2:00 AM|1:00 AM|
  | Duration | 1.5 HR| 2.0 HR|

  ### Resources Discovered or Used Today:
  - Some angel wrote a C++17 wrapper library from which I could get insights and good practice. The code is on [GitHub](https://github.com/Ybalrid/kissnet/) and a blog: [No nonsense networking for C++ : Introducing kissnet, a K.I.S.S. socket library!](https://blog.ybalrid.info/2018/12/no-nonsense-networking-for-c-introducing-kissnet-a-k-i-s-s-socket-library/)
  - [Berkeley Sockets](https://en.wikipedia.org/wiki/Berkeley_sockets])
  - 
  ### Design Decisions
  - Decided to implement the following wrapper APIs
    - Socket
    - Bind
    - Accept
    - Listen
    - Connect
    - GetHostByName
    - GetHostByAddress
    - SetSockOpt
    - Select
    - Poll
    - GetSockOpt
    - SendAll
    - RecvAll

  ### Notes
  - Understanding the hello sample is still in progress, but I have covered both C++ files and ready to start coding!
  - 

  ### Hello World Notes

- May 31st 2020
  ---
  ### Time Information
  || Session 1| Session2 |
  |:---|---:|----:|
  | Start Time | 2:00 AM|11:30 PM|
  | End Time| 3:30 AM|12:30 AM|
  | Duration | 1.5 HR| 1.0 HR|

  ### Resources Discovered or Used Today:
  ### Design Decisions
- Decided to suspend this project until I properly study C++
- I decided this is best as I have not coded in the language in a while and feel out of touch. If i'm going to do this project, I should do it right so others can find it useful.

  ### Notes
- I forget to mention earlier but [@wmessmer](https://twitter.com/wmessmer) answered and confirmed my thinking on [Twitter](https://twitter.com/wmessmer/status/1264258391976734720)
- Today, I went back to SimpleIntro.js to understand the c++ version comparisons. Since I'm not too versed with C++, it seems easier to read the JS and try to understand the equivalent
- c++ code as well as googling stuff!
- 
  ### Hello World Notes