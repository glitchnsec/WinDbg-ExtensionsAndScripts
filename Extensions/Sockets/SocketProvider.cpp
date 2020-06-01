//**************************************************************************
//
// SocketProvider.cpp
// 
// A debugger extension which adds a new namespace "Socket" to the debugger's
// Utility.
// 
// The Socket namespace provides the properties and methods for manipulating
// sockets. This can be used from JavaScript for the creating, reading and
// writinf sockets that are needed to support a debugger extension
//
//**************************************************************************

#include "DbgExt.h"

namespace Debugger::DataModel::Libraries::Socket
{

  //SocketProvider* SocketProvider::s_pProvider = nullptr;

  // HelloObject():
  //
  // Constructor for the singleton instance of the class which binds Details::Hello to the data model.
  //
  SocketObject::SocketObject()
  {

    //
    // Add a custom string conversion function for any instance of Details::Hello.
    //
    //AddStringDisplayableFunction(this, &HelloObject::GetStringConversion);
  }

  // Get_Test():
  //
  // The property accessor the the "Test" property this extension adds to the projection of the
  // 'Details::Hello' object.
  //
  //Object HelloObject::Get_Test(_In_ const Object& /*helloInstance*/,                     // Boxed representation
  //  _In_ const Details::Hello& /*hello*/)                     // Implementation class
  //{
  //  //
  //  // Create and return a new object with two keys: A with the integer value 42, and B with the string
  //  // value "Hello World".
  //  //
  //  return Object::Create(HostContext(),
  //    L"A", 42,
  //    L"B", L"Hello World");
  //}

  // GetStringConversion():
  //
  // Gets the display string conversion for any instance of Details::Hello so that the debugger can display
  // the object's value on console or in UI.  
  //
  //std::wstring HelloObject::GetStringConversion(_In_ const Object& /*helloInstance*/,    // Boxed representation
  //  _In_ Details::Hello& hello,              // Implementation class
  //  _In_ const Metadata& /*metadata*/)       // Conversion metadata
  //{
  //  std::wstring stringConversion = L"C++ Object: ";
  //  stringConversion += hello.Text;
  //  return stringConversion;
  //}

  // SocketExtension()
  // Create the Extension Node
  SocketExtension::SocketExtension() :
    ExtensionModel(NamespacePropertyParent(L"Debugger.Models.Utility", L"Debugger.Models.Utility.Socket", "Socket"))
  {
    //
    // Add a new read-only property named "Hello" whose value is acquired through calling the Get_Hello
    // accessor method.
    //
    //AddReadOnlyProperty(L"Hello", this, &HelloExtension::Get_Hello);
  }

  // Get_Hello():
  //
  // The property accessor for the "Hello" property this extension adds to processes.  This can simply return
  // a Details::Hello directly because we define a specialization of 
  // Debugger::DataModel::ClientEx::Boxing::BoxObject<Details::Hello> which knows how to box and unbox instances 
  // of Details::Hello.
  //
  //Details::Hello HelloExtension::Get_Hello(_In_ const Object& /*processInstance*/)
  //{
  //  return Details::Hello{ L"Hello World" };
  //}

  // HelloProvider():
  //
  // The constructor for the hello provider which instantiates the necessary classes to extend the debugger
  // through the data model.
  //
  HelloProvider::HelloProvider()
  {
    m_spHelloFactory = std::make_unique<HelloObject>();
    m_spHelloExtension = std::make_unique<HelloExtension>();
    s_pProvider = this;
  }

  HelloProvider::~HelloProvider()
  {
    s_pProvider = nullptr;
  }

} // Debugger::DataModel::Libraries::Hello

namespace Debugger::DataModel::ClientEx::Boxing
{

  //using namespace Debugger::DataModel::Libraries::Hello;
  //using namespace Debugger::DataModel::Libraries::Hello::Details;

  //// BoxObject<Hello>::Unbox():
  ////
  //// Provides a custom unboxing (conversion from a generic object instance) implementation for
  //// instances of Details::Hello.
  ////
  //Hello BoxObject<Hello>::Unbox(_In_ const Object& object)
  //{
  //  //
  //  // Check whether the object is an instance of our Details::Hello.  If not, throw; otherwise,
  //  // convert.
  //  //
  //  auto& factory = HelloProvider::Get().GetHelloFactory();
  //  if (!factory.IsObjectInstance(object))
  //  {
  //    throw std::invalid_argument("Illegal object type.  Cannot convert to Hello");
  //  }

  //  return factory.GetStoredInstance(object);
  //}

  //// BoxObject<Hello>::Box():
  ////
  //// Provides a custom boxing (conversion to a generic object instance) implementation for
  //// instances of Details::Hello.
  ////
  //Object BoxObject<Hello>::Box(_In_ const Hello& hello)
  //{
  //  return HelloProvider::Get().GetHelloFactory().CreateInstance(hello);
  //}

} // Debugger::DataModel::ClientEx::Boxing
