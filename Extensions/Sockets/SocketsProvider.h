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
		std::wstring hostname;
		int port = 0;
		ADDRINFOW *lresult;
		ADDRINFOW *rresult;
		int endpoint_type = -1; // 0 | 1 for client and server respectively
	};

	struct Utility {

		// only keep a reference to what was created,
		// indexs cannot be reassigned
		std::vector<Socket> Sockets {};

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
		const std::wstring hostname,
		int port,
		int family,
		int type,
		int proto
	);

	// ClientEndpoint
	Object ClientEndpoint(
		_In_ const Object& objectInstance,
		_In_ Details::Utility& utilityObject,
		const std::wstring hostname,
		int port,
		int family,
		int type,
		int proto
	);

	std::wstring GetStringConversion(_In_ const Object& objectInstance,
		_In_ Details::Utility& socket,
		_In_ const Metadata& metadata);

};

class SocketObject : public TypedInstanceModel<Details::Socket> {
public:

	SocketObject();

	// Bind
	Object SocketBind(_In_ const Object& ObjectInstance, _In_ const Details::Socket& ssocket);

	// Listen
	Object SocketListen(_In_ const Object& ObjectInstance, _In_ const Details::Socket& ssocket);

	// Accept
	Object SocketAccept(_In_ const Object& ObjectInstance, _In_ const Details::Socket& ssocket);

	std::wstring GetStringConversion(_In_ const Object& objectInstance,
		_In_ Details::Socket& ssocket,
		_In_ const Metadata& metadata);

	// Send
	Object SocketSend(_In_ const Object& socketObjectInstance, _In_ const Details::Socket& socket);

	// Receive
	Object SocketReceive(_In_ const Object& socketObjectInstance, _In_ const Details::Socket& socket);

	// ReceiveAll
	Object SocketReceiveAll(_In_ const Object& socketObjectInstance, _In_ const Details::Socket& socket);

	// close
	Object SocketClose(_In_ const Object& socketObjectInstance, _In_ const Details::Socket& socket);

	// Connect
	Object SocketConnect(
		_In_ const Object& ObjectInstance,
		_In_ const Details::Socket& csocket);

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

	static SocketsProvider& Get() {
		return *s_pProvider;
	}

private:

	static SocketsProvider *s_pProvider;

	std::unique_ptr<SocketObject> m_spSocketObjectFactory;

	std::unique_ptr<SocketExtension> m_spSocketExtension;
};


}

namespace Debugger::DataModel::ClientEx::Boxing
{

	using namespace Debugger::DataModel::Libraries::Sockets;
	using namespace Debugger::DataModel::Libraries::Sockets::Details;

	/*
	template<>
	struct BoxObject<Socket>
	{
		static Socket Unbox(_In_ const Object& object);
		static Object Box(_In_ const Socket& sock);
	};*/

	template<>
	struct BoxObject<Socket>
	{
		static Socket Unbox(_In_ const Object& object);
		static Object Box(_In_ const Socket& sock);
	};

	
	template<>
	struct BoxObject<Utility>
	{
		static Utility Unbox(_In_ const Object& object);
		static Object Box(_In_ const Utility& sock);
	};

};