#include "Sockets.h"

namespace Debugger::DataModel::Libraries::Sockets {

	SocketProvider *SocketProvider::s_pProvider = nullptr;


	/**
		Add all the exposed Methods
	*/
	SocketObject::SocketObject() {
		AddMethod(L"SocketSend", this, &SocketObject::SocketSend);

		AddMethod(L"SocketReceive", this, &SocketObject::SocketReceive);

		AddMethod(L"SocketReceiveAll", this, &SocketObject::SocketReceiveAll);

		AddMethod(L"SocketClose", this, &SocketObject::SocketClose);

	}

	ClientSocketObject::ClientSocketObject() {
		TypedInstanceModel<Details::ClientSocket>::AddMethod(L"SocketConnect", this, &ClientSocketObject::SocketConnect);
	}

	SocketUtility::SocketUtility() {

		BindReadOnlyProperty(L"Sockets", &Details::Utility::Sockets);

		AddMethod(L"SocketInit", this, SocketUtility::SocketInit);

		AddMethod(L"ServerEndpoint", this, SocketUtility::ServerEndpoint);

		AddMethod(L"ClientEndpoint", this, SocketUtility::ClientEndpoint);
	}

	// 
	Details::Socket* SocketUtility::SocketInitHelper(
		_In_ const Object& ObjectInstance,
		_In_ Details::Utility& utilityObject,
		int family,
		int type,
		int proto)
	{
		// Create new Details::Socket
		// TODO: Check c++: we don't want dangling pointers and
		// use after frees
		Details::Socket *socketObject = new Details::Socket;

		// As seen at
		// https://docs.microsoft.com/en-us/windows/win32/winsock/initializing-winsock

		WSADATA wsaData;
		int iResult;
		// Initialize Winsock
		iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (iResult != 0) {
			// Add error to return object
			socketObject->err_code = iResult;
			socketObject->err_msg = L"WSAStartup failed";
			// add this to the list of sockets
			utilityObject.Sockets.push_back(*socketObject);
			return socketObject;
		}

		// As seen at
		// https://docs.microsoft.com/en-us/windows/win32/winsock/creating-a-socket-for-the-server

		struct addrinfo *result = NULL,
			*PTR = NULL,
			hints;

		ZeroMemory(&hints, sizeof(hints));
		// TO DO Extend module to include constants such as these
		hints.ai_family = family;
		hints.ai_socktype = type;
		hints.ai_protocol = proto;
		hints.ai_flags = AI_PASSIVE;

		SOCKET native_socket = INVALID_SOCKET;
		native_socket = socket(hints.ai_family, hints.ai_socktype, hints.ai_protocol);

		// Validation
		if (native_socket == INVALID_SOCKET) {
			WSACleanup();
			socketObject->err_code = WSAGetLastError();
			socketObject->err_msg = L"Error at socket()\n";
			// add this to the list of sockets
			utilityObject.Sockets.push_back(*socketObject);
			return socketObject;
		}

		// Success
		// TODO check C++; We don't want dangling pointer problem or double free
		socketObject->lsock = native_socket;
		socketObject->isInitialized = true;
		socketObject->lresult = result;

		// add this to the list of sockets
		utilityObject.Sockets.push_back(*socketObject);

		return socketObject;
	}

	// Implement the functions
	Object SocketUtility::ClientEndpoint(
		_In_ const Object& objectInstance,
		_In_ Details::Utility& utilityObject,
		std::string hostname,
		int port,
		int family,
		int type,
		int proto
	) 
	{
		int iResult;
		struct addrinfo hints, *ptr;
		hints.ai_family = family;
		hints.ai_socktype = type;
		hints.ai_protocol = proto;
		hints.ai_flags = AI_PASSIVE;

		Details::ServerSocket *socketObject = (Details::ServerSocket *)SocketInitHelper(objectInstance, utilityObject, family, type, proto);

		// Error check the returned object
		if (socketObject->err_code != 0) {
			return Object::Create(HostContext(), L"Socket", *socketObject);
		}

		// GetAddrInfo of remote host
		iResult = getaddrinfo(hostname.c_str(), std::to_string(port).c_str(), &hints, &socketObject->rresult);
		if (iResult != 0) {
			closesocket(socketObject->lsock);
			WSACleanup();
			socketObject->lsock = INVALID_SOCKET;
			socketObject->rresult = NULL;
			socketObject->isInitialized = false;
			socketObject->err_code = WSAGetLastError();
			socketObject->err_msg = L"getaddrinfo failed";
			return Object::Create(HostContext(), L"Socket", *socketObject);
		}

		// Attempt to connect
		ptr = socketObject->rresult;
		while (ptr) {
			// Connect to server.
			iResult = connect(socketObject->lsock, ptr->ai_addr, (int)ptr->ai_addrlen);
			if (iResult == SOCKET_ERROR) {
				ptr = socketObject->rresult->ai_next;
			}
			else {
				break;
				// update with the working pointer
				socketObject->rresult = ptr;
			}
		}

		if (!iResult) {

			freeaddrinfo(socketObject->rresult);
			WSACleanup();
			closesocket(socketObject->lsock);
			socketObject->lsock = INVALID_SOCKET;
			socketObject->rresult = NULL;
			socketObject->isInitialized = false;
			socketObject->err_code = WSAGetLastError();
			socketObject->err_msg = L"connect failed";

		}
		

		return Object::Create(HostContext(), L"Socket", *socketObject);

	}

	Object SocketUtility::SocketInit(
		_In_ const Object& ObjectInstance,
		_In_ Details::Utility& utilityObject,
		int family,
		int type,
		int proto)
	{

		return Object::Create(HostContext(), L"Socket", *(SocketInitHelper(
			ObjectInstance,
			utilityObject,
			family,
			type,
			proto
		)));
	}

	Object SocketUtility::ServerEndpoint(
		_In_ const Object& objectInstance,
		_In_ Details::Utility& utilityObject,
		std::string hostname,
		int port,
		int family,
		int type,
		int proto)
	{
		int iResult;
		struct addrinfo hints;
		hints.ai_family = family;
		hints.ai_socktype = type;
		hints.ai_protocol = proto;
		hints.ai_flags = AI_PASSIVE;
		
		Details::ServerSocket *socketObject = (Details::ServerSocket *)SocketInitHelper(objectInstance, utilityObject, family, type, proto);

		// Error check the returned object
		if (socketObject->err_code != 0) {
			return Object::Create(HostContext(), L"Socket", *socketObject);
		}

		// BIND to an address
		// Resolve the local address and port to be used by the server
		iResult = getaddrinfo(hostname.c_str(), std::to_string(port).c_str(), &hints, &socketObject->lresult);
		if (iResult != 0) {
			closesocket(socketObject->lsock);
			printf("getaddrinfo failed: %d\n", iResult);
			WSACleanup();
			socketObject->lsock = INVALID_SOCKET;
			socketObject->lresult = NULL;
			socketObject->isInitialized = false;
			socketObject->err_code = WSAGetLastError();
			socketObject->err_msg = L"getaddrinfo failed";
			return Object::Create(HostContext(), L"Socket", *socketObject);
		}

		iResult = bind(socketObject->lsock, socketObject->lresult->ai_addr, (int)socketObject->lresult->ai_addrlen);
		freeaddrinfo(socketObject->lresult);
		socketObject->lresult = NULL;
		if (iResult == SOCKET_ERROR) {
			closesocket(socketObject->lsock);
			WSACleanup();
			// update object
			socketObject->lsock = INVALID_SOCKET;
			
			socketObject->isInitialized = false;
			socketObject->err_code = WSAGetLastError();
			socketObject->err_msg = L"bind failed";
			return Object::Create(HostContext(), L"Socket", *socketObject);
		}

		// LISTEN
		if (listen(socketObject->lsock, SOMAXCONN) == SOCKET_ERROR) {
			closesocket(socketObject->lsock);
			WSACleanup();
			// update object
			socketObject->lsock = INVALID_SOCKET;
			socketObject->isInitialized = false;
			socketObject->err_code = WSAGetLastError();
			socketObject->err_msg = L"Listen failed";
			return Object::Create(HostContext(), L"Socket", *socketObject);
		}

		// ACCEPT
		// TODO: May be require a handler from the callee with
		// which requests will be handled
		// run a loop in another thread and provide a wayto kill
		return Object::Create(HostContext(), L"Socket", *socketObject);


	}
	
	std::wstring SocketUtility::GetStringConversion(_In_ const Object& objectInstance,
		_In_ Details::Utility& utility,
		_In_ const Metadata& metadata) {

		std::wstring stringConversion = L"Gateway to Sockets";
		return stringConversion;
	
	}

	std::wstring ClientSocketObject::GetStringConversion(_In_ const Object& objectInstance,
		_In_ Details::ClientSocket& csock,
		_In_ const Metadata& metadata) {

		std::wstring stringConversion = L"ClientSocket: Socket is ";

		if (csock.isInitialized) {
			stringConversion += L"Connected to : (";
			stringConversion += csock.hostname;
			stringConversion += L",";
			stringConversion += std::to_wstring(csock.port);
			stringConversion += L")\n";
		}
		else {
			stringConversion += L"Not Connected. Error msg is: ";
			stringConversion += csock.err_msg;
			stringConversion += L"\n";
		}

		
		return stringConversion;

	}

	std::wstring ServerSocketObject::GetStringConversion(_In_ const Object& objectInstance,
		_In_ Details::ServerSocket& ssock,
		_In_ const Metadata& metadata) {

		std::wstring stringConversion = L"ServerSocket: Socket is ";

		if (ssock.isInitialized) {
			stringConversion += L"Listening at : (";
			stringConversion += ssock.hostname;
			stringConversion += L",";
			stringConversion += std::to_wstring(ssock.port);
			stringConversion += L")\n";
		}
		else {
			stringConversion += L"Not Listening. Error msg is: ";
			stringConversion += ssock.err_msg;
			stringConversion += L"\n";
		}


		return stringConversion;

	}

	SocketExtension::SocketExtension() : ExtensionModel(NamedModelParent(L"Debugger.Models.Utility")) {
		AddReadOnlyProperty(L"Sockets", this, &SocketExtension::Get_Socket);
	}

	Details::Utility SocketExtension::Get_Socket(
		_In_ const Object& /*utilityInstance*/
	) {
		return Details::Utility{};
	}

	SocketProvider::SocketProvider() {
		m_spSocketObjectFactory = std::make_unique<SocketObject>();
		m_spServerSocketObjectFactory = std::make_unique<ServerSocketObject>();
		m_spClientSocketObjectFactory = std::make_unique<ClientSocketObject>();
		m_spSocketExtension = std::make_unique<SocketExtension>();
		s_pProvider = this;
	}

}