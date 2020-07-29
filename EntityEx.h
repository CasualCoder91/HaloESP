#pragma once

#include "Entity.h"

class EntityEx {
public:
	Entity* entity;
	__int16 typeID;

	EntityEx(Entity* entity, __int16 typeID) {
		this->entity = entity;
		this->typeID = typeID;
	}

};