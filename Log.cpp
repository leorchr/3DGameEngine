#include "Log.h"
#include <SDL_log.h>
#include <SDL_error.h>

void Log::info(const string& message)
{
	SDL_Log(message.c_str());
}

void Log::error(LogCategory category, const string& message)
{
	SDL_LogError(static_cast<int>(category), "%s | SDL: %s", message.c_str(), SDL_GetError());
}

bool Log::initialize()
{
	SDL_LogSetOutputFunction(outputLogFunction, nullptr);
	return true;
}

void outputLogFunction(void* userdata, int category, SDL_LogPriority priority, const char* message)
{
	FILE* output = (priority >= SDL_LOG_PRIORITY_WARN) ? stderr : stdout;
	fprintf(output, "[%s] %s\n", getPriorityName(priority), message);
	
	// if (priority >= SDL_LOG_PRIORITY_WARN) {
	// 	std::cerr << message << std::endl;
	// } else {
	// 	std::cout << message << std::endl;
	// }
}

const char* getPriorityName(SDL_LogPriority priority)
{
	switch (priority) {
	case SDL_LOG_PRIORITY_VERBOSE: return "VERBOSE";
	case SDL_LOG_PRIORITY_DEBUG: return "DEBUG";
	case SDL_LOG_PRIORITY_INFO: return "INFO";
	case SDL_LOG_PRIORITY_WARN: return "WARN";
	case SDL_LOG_PRIORITY_ERROR: return "ERROR";
	case SDL_LOG_PRIORITY_CRITICAL: return "CRITICAL";
	default: return "UNKNOWN";
	}
}

