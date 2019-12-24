
// SDK includes
#include <bento_memory/system_allocator.h>
#include <bento_base/log.h>
#include <orma_rest/session.h>

int main(int, char**)
{
	// Create an allocator for our application
	bento::SystemAllocator systemAllocator;

	// Create a session
	orma::TSession session(systemAllocator);

	// Init the session
	session.init();

	// Do a ping request
	bento::DynamicString message(systemAllocator);
	session.ping(message);
	
	// Log the answer
	bento::ILogger* logger = bento::default_logger();
	logger->log(bento::LogLevel::info, "PING_MESSAGE", message.c_str());

	// Terminate the session
	session.terminate();

	// We are done!
	return 0;
}