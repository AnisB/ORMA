// SDK includes
#include <bento_memory/system_allocator.h>
#include <bento_base/log.h>
#include <orma_rest/session.h>

int main(int argc, char** argv)
{
	// Check that the id is there
	if (argc != 2)
		return -1;

	// Create an allocator for our application
	bento::SystemAllocator systemAllocator;

	// Create a session
	orma::TSession session(systemAllocator);

	// Init the session
	session.init();

	// Declare our credentials
	orma::TCredentials credentials(systemAllocator);
	credentials.token = argv[1];

	// Do a ping request
	bento::DynamicString message(systemAllocator);
	session.market_list(credentials, message);
	
	// Log the answer
	bento::ILogger* logger = bento::default_logger();
	logger->log(bento::LogLevel::info, "MARKET_CONTENT", message.c_str());

	// Terminate the session
	session.terminate();

	// We are done!
	return 0;
}