#include "PhysicsEngine.hpp"
#include "shared/PhysicsConstants.hpp"
#define time_correction Constants::time_correction_coefficient * float(UpdateInterval)

vector const PhysicsEngine::calculateGravityForce() {
	return vector(0.f, Constants::g);
}
scalar const PhysicsEngine::calculateDimentionalDragForce(scalar const& speed, scalar const& area) {
	if (speed > 0.f)
		return -0.5f * Constants::air_density * speed * speed * Constants::cube_drag_coefficient * area;
	else if (speed < 0.f)
		return +0.5f * Constants::air_density * speed * speed * Constants::cube_drag_coefficient * area;
	else
		return 0.f;
}

vector const PhysicsEngine::calculateDragForce(vector const& speed, vector const& size) {
	return vector(calculateDimentionalDragForce(speed[0], size[1] * size[1]), 
				  calculateDimentionalDragForce(speed[1], size[0] * size[0]));
}

#include "Objects/Objects/IndependentObject.hpp"
void PhysicsEngine::processForces(std::shared_ptr<IndependentObjectState> os) {
	auto net_force = calculateGravityForce() + calculateDragForce(os->speed(), os->size());
	auto acceleration = net_force * os->mass();

	os->accelerate(acceleration);
	os->speed_up(os->acceleration(time_correction) * time_correction);
	os->accelerate(-acceleration);
}

#include "Objects/Map/Map.hpp"
#define speed_test(test, result) (test > 0.f ? result : -result)
bool PhysicsEngine::processMovement(std::shared_ptr<IndependentObjectState> os, std::shared_ptr<Map> map) {
	if (map) {
		auto future_position = os->position() + os->speed() * time_correction;
		auto half_size = os->size() * 0.5f;
		float speed_multiplier;

		speed_multiplier = map->getSpeedMultiplier(size_t(future_position[0]), size_t(future_position[1] + speed_test(os->speed()[1], half_size[1])));
		os->speed_mul(vector(1.f, speed_multiplier));
		if (speed_multiplier == 0.f) {
			os->stopAcceleration_v();
			if (os->speed()[1] > 0.f)
				os->move_to_v(size_t(future_position[1] + half_size[1]) - half_size[1]);
			else if(os->speed()[1] < 0.f)
				os->move_to_v(size_t(future_position[1] - half_size[1]) + half_size[1] + 1);
		}

		speed_multiplier = map->getSpeedMultiplier(size_t(future_position[0] + speed_test(os->speed()[0], half_size[0])), size_t(future_position[1]));
		os->speed_mul(vector(speed_multiplier, 1.f));
		if (speed_multiplier == 0.f) {
			os->stopAcceleration_h();
			if (os->speed()[0] > 0.f)
				os->move_to_h(size_t(future_position[0] + half_size[0]) - half_size[0]);
			else if(os->speed()[0] < 0.f)
				os->move_to_h(size_t(future_position[0] - half_size[0]) + half_size[0] + 1);
		}
	}
	os->move(vector(os->speed() * time_correction));
	return os->speed().isNull();
}

#include "Objects/Actors/MainActor.hpp"
#include "Objects/EquipableItems/Weapon.hpp"
#include "Objects/Objects/ObjectQueue.hpp"
void PhysicsEngine::processWeaponry(std::shared_ptr<MainActor> ma, DoubleProjectileQueue &projectile_queue, size_t id) {
	for (auto projectile : ma->shootingProcess(id))
		if (projectile)
			projectile_queue->add(projectile);
}

#include "Objects/Objects/ShootableObject.hpp"
#include <iostream>
bool PhysicsEngine::processTargeting(std::shared_ptr<ShootableObject> so, MainActorQueue &actors) {
	bool ret = false;
	actors.for_each([&so, &ret, &actors](std::shared_ptr<MainActor> a, size_t i) {
		if (a->is_alive()) {
			if (actors.at(so->shooter_id()) != a) {
				auto distance = a->position() - so->position();
				if (fabs(distance.at(0)) < (so->size().at(0) + a->size().at(0)) / 2 && fabs(distance.at(1)) < (so->size().at(1) + a->size().at(1)) / 2) {
					if (true/*second check is to be implemented here*/) {
						std::cout << "\rPlayer #" << so->shooter_id() << " has hit player #" << i << " causing him to loose ";
						a->was_hit(so);
						ret = true;
					}
				}
			}
		}
	});
	return ret;
}