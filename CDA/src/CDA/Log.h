#pragma once

#include <memory>

#include "Core.h"
#include "spdlog/spdlog.h"

namespace CDA {
	class CDA_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

// Core log macros
#define CDA_CORE_TRACE(...) ::CDA::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define CDA_CORE_INFO(...)  ::CDA::Log::GetCoreLogger()->info(__VA_ARGS__)
#define CDA_CORE_WARN(...)  ::CDA::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define CDA_CORE_ERROR(...) ::CDA::Log::GetCoreLogger()->error(__VA_ARGS__)
#define CDA_CORE_FATAL(...) ::CDA::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// Client log macros
#define CDA_TRACE(...)      ::CDA::Log::GetClientLogger()->trace(__VA_ARGS__)
#define CDA_INFO(...)       ::CDA::Log::GetClientLogger()->info(__VA_ARGS__)
#define CDA_WARN(...)       ::CDA::Log::GetClientLogger()->warn(__VA_ARGS__)
#define CDA_ERROR(...)      ::CDA::Log::GetClientLogger()->error(__VA_ARGS__)
#define CDA_FATAL(...)      ::CDA::Log::GetClientLogger()->fatal(__VA_ARGS__)