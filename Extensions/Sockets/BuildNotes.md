Begining a Project
0. Environment VSCode 2017 v141 SDK <= 10.0.8362.0
1. Tools -> Nugget Package Manager -> Manage Nugget Package for Solution.. -> search and Install "Microsoft.Debugging.DataModel.CppLib"
2. Make sure `dbeng.lib` and `runtimeobject.lib` are linked. Go to Project Properties -> Configuration Properties -> Linker -> Input -> Additional Dependencies
3. Create Source File with minimum the following includes
   ```cpp
	#include <windows.h>
	#include <wrl.h>
	using namespace Microsoft::WRL;
	#include <dbgmodel.h>
	#include "DbgModelClientEx.h"

	using namespace Debugger::DataModel::ClientEx;
	using namespace Debugger::DataModel::ProviderEx;
   ```
4. Add the template code for DbgEng Extension Exports

   See commit with message: Starter code for SocketsExtension.cpp SocketsExtension.def and SocketsExtension.h and BuildNotes.md

5. Implement the Provider.
	The provider.h file should contains delarations for classes and properties in their namespaces.
	Add the provider.h file to the includes in the main .h file