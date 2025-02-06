#include <unordered_map>
#include <string>

class ActorFactory {
public:
	using Creator = class Actor* (*)();

	static ActorFactory& getInstance();

	void registerActor(const std::string& type, Creator creator);

	class Actor* create(const std::string& type);

	std::unordered_map<std::string, Creator> getCreators();

private:
	std::unordered_map<std::string, Creator> creators;
};