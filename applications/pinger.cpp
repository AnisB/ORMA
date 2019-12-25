
// SDK includes
#include <bento_memory/system_allocator.h>
#include <bento_base/log.h>
#include <orma_rest/session.h>

// External includes
#include <string>

int main(int, char**)
{
	// Create an allocator for our application
	bento::SystemAllocator systemAllocator;

	// Create a session
	orma::TSession session(systemAllocator);

	// Init the session
	session.init();

	// Do a ping request
	orma::TPingResponse pingReponse(systemAllocator);
	session.ping(pingReponse);
	
	// Log the answer
	bento::ILogger* logger = bento::default_logger();
	if (pingReponse.validity)
	{
		logger->log(bento::LogLevel::info, "STATUS", "success");
		logger->log(bento::LogLevel::info, "API VERSION", std::to_string(pingReponse.apiVersion).c_str());
		logger->log(bento::LogLevel::info, "TIMESTAMP", pingReponse.timeStamp.c_str());
	}
	else
	{
		logger->log(bento::LogLevel::info, "ERROR", "Ping failed");
	}

	// Terminate the session
	session.terminate();

	// We are done!
	return 0;
}