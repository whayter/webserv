#ifndef SINGLETON_FIXTURE
#define SINGLETON_FIXTURE

#include "ServerConfig.hpp"

class SingletonFixture
{
	public:
		SingletonFixture() {}
		~SingletonFixture()
		{
			ServerConfig::__delete_singleton_instance();
		}
		ServerConfig& SetUpFile(const std::string& filename)
		{
			ServerConfig::__delete_singleton_instance();
			return ServerConfig::getInstance(filename);
		}
		ServerConfig& SetUpFile(const std::string& filename, const std::string& mimeFile)
		{
			ServerConfig::__delete_singleton_instance();
			return ServerConfig::getInstance(filename, mimeFile);
		}
};

#endif /* SINGLETON_FIXTURE */