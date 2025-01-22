#include "ActorFactory.h"

ActorFactory& ActorFactory::getInstance()
{
	static ActorFactory instance;
	return instance;
}

void ActorFactory::registerActor(const std::string& type, Creator creator)
{
	creators[type] = creator;
}

Actor* ActorFactory::create(const std::string& type)
{
	auto it = creators.find(type);
	if (it != creators.end()) {
		return it->second();
	}
	return nullptr; // Retourne nullptr si le type n'est pas trouvé
}

std::unordered_map<std::string, ActorFactory::Creator> ActorFactory::getCreators()
{
	return creators;
}