#pragma once
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <wrl.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <string.h>

#include <oaidl.h>
#include <wrl/client.h>
#include <wrl/implements.h>
#include <wrl/module.h>

// This is needed when using SDK version 10.0.17763.0
// For newer versions of the SDK, this is not necessary
using namespace Microsoft::WRL;

#include <dbgmodel.h>
#include "DbgModelClientEx.h"

#include "SocketsProvider.h"
#include "SocketsExtension.h"

#pragma comment(lib, "Ws2_32.lib")