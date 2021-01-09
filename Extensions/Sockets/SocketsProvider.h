#pragma once
namespace Debugger::DataModel::Libraries::Sockets {

using namespace Debugger::DataModel::ClientEx;
using namespace Debugger::DataModel::ProviderEx;

namespace Details {

	struct Socket {
		// native socket instance - local to the endpoint
		SOCKET lsock = INVALID_SOCKET;
		SOCKET rsock = INVALID_SOCKET;
		bool isInitialized = false;
		int err_code = -1;
		std::wstring err_msg = L"Unitialized";
		const std::wstring hostname;
		int port;
		struct addrinfo *lresult;
		struct addrinfo *rresult;
	};

	struct ClientSocket : public Socket { };

	struct ServerSocket : public Socket { };

	struct Utility {

		// only keep a reference to what was created,
		// indexs cannot be reassigned
		std::vector<Socket &> Sockets;

		// Add CONSTANT defines here

	};

}

class SocketUtility : public TypedInstanceModel<Details::Utility> {
public:
	SocketUtility();

	// Expose 3 ways of creating a Socket, the raw way and either a tcp server or client sock

	// Init
	// Set up WSADATA and WSAStartup for the initialization
	// of Winsock
	// also map constants
	Details::Socket* SocketInitHelper(
		_In_ const Object& objectInstance,
		_In_ Details::Utility& utility,
		int family,
		int type,
		int proto
	);


	Object SocketInit(
		_In_ const Object& socketObjectInstance,
		_In_ Details::Utility& socket,
		int family,
		int type,
		int proto
	);

	// ServerEndpoint
	Object ServerEndpoint(
		_In_ const Object& objectInstance,
		_In_ Details::Utility& utilityObject,
		std::string hostname,
		int port,
		int family,
		int type,
		int proto
	);

	// ClientEndpoint
	Object ClientEndpoint(
		_In_ const Object& objectInstance,
		_In_ Details::Utility& utilityObject,
		std::string hostname,
		int port,
		int family,
		int type,
		int proto
	);

	std::wstring GetStringConversion(_In_ const Object& objectInstance,
		_In_ Details::Utility& socket,
		_In_ const Metadata& metadata);

};

class ServerSocketObject : public TypedInstanceModel<Details::ServerSocket> {
public:

	ServerSocketObject();

	// Bind
	Object ServerSocketBind(_In_ const Object& ObjectInstance, _In_ const Details::ServerSocket& ssocket);

	// Listen
	Object ServerSocketListen(_In_ const Object& ObjectInstance, _In_ const Details::ServerSocket& ssocket);

	// Accept
	Object ServerSocketAccept(_In_ const Object& ObjectInstance, _In_ const Details::ServerSocket& ssocket);

	std::wstring GetStringConversion(_In_ const Object& objectInstance,
		_In_ Details::ServerSocket& ssocket,
		_In_ const Metadata& metadata);
};

class SocketObject : public TypedInstanceModel<Details::Socket> {
public:

	SocketObject();

	// Send
	Object SocketSend(_In_ const Object& socketObjectInstance, _In_ const Details::Socket& socket);

	// Receive
	Object SocketReceive(_In_ const Object& socketObjectInstance, _In_ const Details::Socket& socket);

	// ReceiveAll
	Object SocketReceiveAll(_In_ const Object& socketObjectInstance, _In_ const Details::Socket& socket);

	// close
	Object SocketClose(_In_ const Object& socketObjectInstance, _In_ const Details::Socket& socket);

	std::wstring GetStringConversion(_In_ const Object& socketObjectInstance,
		_In_ Details::Socket& socket,
		_In_ const Metadata& metadata);
};


class ClientSocketObject : public SocketObject, public TypedInstanceModel<Details::ClientSocket> {
public:

	ClientSocketObject();

	// Connect
	Object SocketConnect(
		_In_ const Object& ObjectInstance,
		_In_ const Details::ClientSocket& csocket);

	std::wstring GetStringConversion(_In_ const Object& socketObjectInstance,
		_In_ Details::ClientSocket& utility,
		_In_ const Metadata& metadata);
};


class SocketExtension : public ExtensionModel {
public:
	SocketExtension();

	Details::Utility Get_Socket(_In_ const Object& utilityInstance);
};

class SocketsProvider {
public:
	SocketsProvider();
	~SocketsProvider();

	SocketObject& GetSocketObjectFactory() const {
		return *m_spSocketObjectFactory.get();
	}

	ServerSocketObject& GetServerSocketObjectFactory() const {
		return *m_spServerSocketObjectFactory.get();
	}

	ClientSocketObject& GetClientSocketObjectFactory() const {
		return *m_spClientSocketObjectFactory.get();
	}

	static SocketsProvider& Get() {
		return *s_pProvider;
	}

private:

	static SocketsProvider *s_pProvider;

	std::unique_ptr<SocketObject> m_spSocketObjectFactory;

	std::unique_ptr<ServerSocketObject> m_spServerSocketObjectFactory;

	std::unique_ptr<ClientSocketObject> m_spClientSocketObjectFactory;

	std::unique_ptr<SocketExtension> m_spSocketExtension;
};


}

namespace Debugger::DataModel::ClientEx::Boxing
{

	using namespace Debugger::DataModel::Libraries::Sockets;

	template<>
	struct BoxObject<SOCKET>
	{
		static SOCKET Unbox(_In_ const Object& object);
		static Object Box(_In_ const SOCKET& hello);
	};

};