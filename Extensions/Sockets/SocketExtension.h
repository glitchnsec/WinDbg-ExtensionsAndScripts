#pragma once
//**************************************************************************
//
// SocketExtension.h
//
// A debugger extension which adds a new property "Sockets" to the debugger's
// Utility parent model.
//
//**************************************************************************

namespace Debugger::DataModel::Libraries
{

  using namespace Debugger::DataModel::ClientEx;
  using namespace Debugger::DataModel::ProviderEx;

  // ExtensionProvider():
  //
  // A singleton which 
  // A provider class which links together all the individual provider libraries into one thing the engine
  // extension can deal with.
  //
  class ExtensionProvider
  {
  public:

    // ExtensionProvider():
    //
    // Construct the provider for this extension and instantiate any individual extension classes which
    // together form the functionality of this debugger extension.
    //
    ExtensionProvider();

    Socket::SocketProvider& GetSocketProvider() const { return *m_spSocketProvider.get(); }

  private:

    std::unique_ptr<Socket::SocketProvider> m_spSocketProvider;
  };

} // Debugger::DataModel::Libraries

