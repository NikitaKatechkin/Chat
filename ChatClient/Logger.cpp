#include "Logger.h"

Logger::Logger(const std::string& logFilePath):
	m_logStream(new std::ofstream(logFilePath))
{
	if (m_logStream->is_open() == false)
	{
		throw std::exception("Failed to create a log file.");
	}
}

Logger::~Logger()
{
	if (m_logStream != nullptr)
	{
		if (m_logStream->is_open() == true)
		{
			m_logStream->close();
			delete m_logStream;
		}
	}
}

void Logger::AddLog(const std::string& log)
{
	std::ostream& currentStream = (m_logStream != nullptr) ? *m_logStream : std::cout;
	
	currentStream << log << std::endl;
}

void operator<<(Logger& logger, const std::string& log)
{
	logger.AddLog(log);
}
