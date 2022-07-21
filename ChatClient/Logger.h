#pragma once

#include <iostream>
#include <fstream>

class Logger final
{
public:
	Logger() = default;
	Logger(const std::string& logFilePath);

	~Logger();

	void AddLog(const std::string& log);
	friend void operator<<(Logger& logger, const std::string& log);

private:
	std::ofstream* m_logStream = nullptr;
};