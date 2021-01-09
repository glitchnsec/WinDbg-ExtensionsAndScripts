namespace Debugger::DataModel::Libraries
{

	using namespace Debugger::DataModel::ClientEx;
	using namespace Debugger::DataModel::ProviderEx;

	class ExtensionProvider
	{
	public:

		// ExtensionProvider():
		//
		// Construct the provider for this extension and instantiate any individual extension classes which
		// together form the functionality of this debugger extension.
		//

		// For this feature we need to be able to 
		// Create 
		ExtensionProvider();
		Sockets::SocketsProvider& GetSocketsProvider() const { return *m_spSocketsProvider.get(); }

	private:
		std::unique_ptr<Sockets::SocketsProvider> m_spSocketsProvider;

	};

} // Debugger::DataModel::Libraries