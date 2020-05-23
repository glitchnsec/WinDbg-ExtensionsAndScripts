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