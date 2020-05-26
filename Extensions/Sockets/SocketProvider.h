#pragma once
//**************************************************************************
//
// SocketProvider.h
// 
// Definition of a provider class which adds a "Socket" node to "Debugger.Utility.Socket"
// Heavily influenced by https://github.com/Ybalrid/kissnet/blob/master/kissnet.hpp
//**************************************************************************

namespace Debugger::DataModel::Libraries::Socket
{

  using namespace Debugger::DataModel::ClientEx;
  using namespace Debugger::DataModel::ProviderEx;

  //*************************************************
  // Socket Internals
  // definitions for protocols, Ip versions etc
  namespace Internals
  {

    // supported protocols
    enum class protocol {
      tcp,
      udp
    }; // enum protocol

    // supported Ip versions
    enum class ip_version {
      v4,
    }; // enum ip

    // address of sock -> ip and port
    struct address {

      // ip addr
      std::wstring ip_addr{};

      // port
      uint16_t port{};

      // default constructor
      address() = default;

      static bool is_valid_port(unsigned long n) {
        return n < 1 << 16;
      }

      // cosntructor for string and numeric port. Similar to python
      address(std::wstring ip, uint16_t prt) {
        if (is_valid_port(prt)) {
          port = prt;
          ip_addr = std::move(ip);
        } else {
          // TODO implement error handling
        }
      
      }
      // TODO: constructor for SOCKADDR

    }; // struct address


    // Taken as is from https://github.com/Ybalrid/kissnet/blob/616d360ff150f32475c15630e27bdcab95e16ffb/kissnet.hpp#L523
    struct socket_status
    {
      ///Enumeration of socket status, with a 1 byte footprint
      enum values : int8_t {
        errored = 0x0,
        valid = 0x1,
        cleanly_disconnected = 0x2,
        non_blocking_would_have_blocked = 0x3

        /* ... any other info on a "still valid socket" goes here ... */

      };

      ///Actual value of the socket_status.
      const values value;

      ///Use the default constructor
      socket_status() :
        value{ errored } {}

      ///Construct a "errored/valid" status for a true/false
      explicit socket_status(bool state) :
        value(values(state ? valid : errored)) {}

      socket_status(values v) :
        value(v) {}

      ///Copy socket status by default
      socket_status(const socket_status&) = default;

      ///Move socket status by default
      socket_status(socket_status&&) = default;

      ///implicitly convert this object to const bool (as the status should not change)
      operator bool() const
      {
        //See the above enum: every value <= 0 correspond to an error, and will return false. Every value > 0 returns true
        return value > 0;
      }

      int8_t get_value()
      {
        return value;
      }

      bool operator==(values v)
      {
        return v == value;
      }
    }; // struct status

  }

  //*************************************************
  // Extension Classes:
  //

  // SocketObject:
  // Wrapper around the native socket implementation in windows
  // with simplifications
  template <Internals::protocol sock_proto, Internals::ip ipver = Internals::ip_version::v4>
  class SocketObject
  {
  public:

    SocketObject();

    Object Socket();

    Object Close();

    Object Accept();

    Object Listen();

    Object Connect();

    Object Bind();

    Object GetHostByName();

    Object GetHostByAddress();

    Object Send();

    Object Recv();

    Object ErrorStatus();


    // GetStringConversion():
    //
    // The method which will be called to generate a string conversion of a Socket
    //
    std::wstring GetStringConversion(_In_ const Object& socketInstance,
      _In_ Details::Info& socket,
      _In_ const Metadata& metadata);


  };

  // HelloExtension:
  //
  // A singleton class which extends the debugger's notion of a process with a new "Hello" property.
  //
  // [JavaScript: This is equivalent to the __HelloExtension class]
  // [COM       : This is equivalent to the HelloExtension and HelloExtensionModel classes]
  //
  class SocketExtension : public ExtensionModel
  {
  public:

    SocketExtension();

    // Get_Hello():
    //
    // The property accessor for the "Hello" property which is added to a process.
    //
    Details::Socket Get_Hello(_In_ const Object& processInstance);

  };

  // SocketProvider:
  //
  // A class which provides the "hello" set of functionality.  This is a singleton instance which encapsulates
  // all of the extension classes and factories for this part of the debugger extension.
  //
  // [JavaScript: This (and ExtensionProvider) is represented by the overall script and the initializeScript method]
  // [COM       : This is equivalent to the HelloExtension class]
  //
  class HelloProvider
  {
  public:

    HelloProvider();
    ~HelloProvider();

    // GetHelloFactory():
    //
    // Gets our singeton instance of the class which makes Details::Hello visible to the data model.
    //
    HelloObject& GetHelloFactory() const { return *m_spHelloFactory.get(); }

    // Get():
    //
    // Gets the singleton instance of the HelloProvider.
    //
    static HelloProvider& Get()
    {
      return *s_pProvider;
    }

  private:

    static HelloProvider* s_pProvider;

    // Factories:
    //
    // These are the classes which bridge C++ constructs (such as the Details::Hello object) to
    // the data model.
    //
    std::unique_ptr<HelloObject> m_spHelloFactory;

    // Extensions managed by this provider
    std::unique_ptr<HelloExtension> m_spHelloExtension;
  };

} // Debugger::DataModel::Libraries::Hello

//*************************************************
// Custom Boxing and Unboxing of Internal Classes:
//

namespace Debugger::DataModel::ClientEx::Boxing
{

  using namespace Debugger::DataModel::Libraries::Hello;
  using namespace Debugger::DataModel::Libraries::Hello::Details;

  template<>
  struct BoxObject<Hello>
  {
    static Hello Unbox(_In_ const Object& object);
    static Object Box(_In_ const Hello& hello);
  };

};




