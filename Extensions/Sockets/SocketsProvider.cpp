#include "Sockets.h"

namespace Debugger::DataModel::Libraries::Sockets {

	SocketsProvider *SocketsProvider::s_pProvider = nullptr;
	//SocketUtility *g_pSocketUtility = nullptr;


	/**
		Add all the exposed Methods
	*/
	SocketObject::SocketObject() {
		//AddMethod(L"SocketSend", this, &SocketObject::SocketSend);

		//AddMethod(L"SocketReceive", this, &SocketObject::SocketReceive);

		//AddMethod(L"SocketReceiveAll", this, &SocketObject::SocketReceiveAll);

		//AddMethod(L"SocketClose", this, &SocketObject::SocketClose);

		AddStringDisplayableFunction(this, &SocketObject::GetStringConversion);

	}


	SocketUtility::SocketUtility() {

		BindReadOnlyProperty(L"Sockets", &Details::Utility::Sockets);

		AddMethod(L"SocketInit", this, &SocketUtility::SocketInit);

		AddMethod(L"ServerEndpoint", this, &SocketUtility::ServerEndpoint);

		AddMethod(L"ClientEndpoint", this, &SocketUtility::ClientEndpoint);

		AddStringDisplayableFunction(this, &SocketUtility::GetStringConversion);

		//g_pSocketUtility = this;
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

		Details::Socket *socketObject = SocketInitHelper(objectInstance, utilityObject, family, type, proto);

		// this is a server endpoint
		socketObject->endpoint_type = 1;

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
		
		Details::Socket *socketObject = SocketInitHelper(objectInstance, utilityObject, family, type, proto);

		// this is a server endpoint
		socketObject->endpoint_type = 0;

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

	std::wstring SocketObject::GetStringConversion(_In_ const Object& objectInstance,
		_In_ Details::Socket& sock,
		_In_ const Metadata& metadata) {

		std::wstring stringConversion = L"";

		if (sock.isInitialized) {
			if (sock.endpoint_type == 1) {
				// client
				stringConversion = L"ClientSocket: Socket is ";
				stringConversion += L"Connected to : (";
				stringConversion += sock.hostname;
				stringConversion += L",";
				stringConversion += std::to_wstring(sock.port);
				stringConversion += L")\n";

			}
			else if (sock.endpoint_type == 0) {
				// server
				stringConversion = L"ServerSocket: Socket is ";
				stringConversion += L"Listening at : (";
				stringConversion += sock.hostname;
				stringConversion += L",";
				stringConversion += std::to_wstring(sock.port);
				stringConversion += L")\n";
			}
			else {
				// haven't seen this before 
			}
		}
		else {
			stringConversion += L"Not Listening. Error msg is: ";
			stringConversion += sock.err_msg;
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

	SocketsProvider::SocketsProvider() {
		m_spSocketObjectFactory = std::make_unique<SocketObject>();
		m_spSocketExtension = std::make_unique<SocketExtension>();
		s_pProvider = this;
	}

	SocketsProvider::~SocketsProvider() {
		s_pProvider = nullptr;
	}

}

namespace Debugger::DataModel::ClientEx::Boxing
{

	using namespace Debugger::DataModel::Libraries::Sockets;
	using namespace Debugger::DataModel::Libraries::Sockets::Details;

	


	Socket BoxObject<Socket>::Unbox(_In_ const Object& object)
	{
		//
		// Check whether the object is an instance of our Details::Hello.  If not, throw; otherwise,
		// convert.
		//
		auto& factory = SocketsProvider::Get().GetSocketObjectFactory();
		if (!factory.IsObjectInstance(object))
		{
			throw std::invalid_argument("Illegal object type.  Cannot convert to Socket");
		}

		return factory.GetStoredInstance(object);
	}

	// BoxObject<Hello>::Box():
	//
	// Provides a custom boxing (conversion to a generic object instance) implementation for
	// instances of Details::Hello.
	//
	Object BoxObject<Socket>::Box(_In_ const Socket& sock)
	{
		return SocketsProvider::Get().GetSocketObjectFactory().CreateInstance(sock);
	}

	
	Utility BoxObject<Utility>::Unbox(_In_ const Object& object)
	{
		//
		// convert.
		//
		SocketUtility *g_pSocketUtility = new SocketUtility;
		if (!g_pSocketUtility->IsObjectInstance(object))
		{
			throw std::invalid_argument("Illegal object type.  Cannot convert to Utility");
		}

		auto inst = g_pSocketUtility->GetStoredInstance(object);
		//delete g_pSocketUtility;
		return inst;
	}


	
	Object BoxObject<Utility>::Box(_In_ const Utility& util)
	{
		SocketUtility *g_pSocketUtility = new SocketUtility;
		auto inst = g_pSocketUtility->CreateInstance(util);
		//delete g_pSocketUtility;
		return inst;
	}


} // Debugger::DataModel::ClientEx::Boxing
