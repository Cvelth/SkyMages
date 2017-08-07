#include "PhysicsEngine.hpp"

size_t PhysicsEngine::UpdateInterval = 16667ul;

PhysicsEngine::PhysicsEngine() : m_is_initialized(false) {}

PhysicsEngine::PhysicsEngine(std::function<bool()> const& finishFlagAccess) : PhysicsEngine() {
	initialize(finishFlagAccess);
}
PhysicsEngine::~PhysicsEngine() {
	clean();
}

size_t PhysicsEngine::getUpdateInterval() {
	return UpdateInterval;
}

void PhysicsEngine::changeUpdateInterval(size_t microseconds) {
	UpdateInterval = microseconds;
}

#include "PhysicalObjectsQueue.hpp"
void PhysicsEngine::initialize(std::function<bool()> const& finishFlagAccess) {
	m_finish_flag_access = finishFlagAccess;
	m_queue = new PhysicalObjectsQueue();
	m_is_initialized = true;
}
void PhysicsEngine::initializeCollisionSystem(GameMap *map) {
	m_map = map;
}
void PhysicsEngine::addObject(AbstractGameObject * object) {
	m_queue->add(object);
}
void PhysicsEngine::removeObject(AbstractGameObject * object) {
	m_queue->remove(object);
}
void PhysicsEngine::clean() {
	if (m_is_initialized && m_queue)
		delete m_queue;
	m_is_initialized = false;
}
#include <iostream>
#include <chrono>
#include <thread>
#include "GameObjects\AbstractGameObject.hpp"
#include "Shared\GameMode.hpp"
void PhysicsEngine::loop(bool destroy_engine_after_exit) {
	GameModeController::physicsLoopIsReady(true);
	while (!m_finish_flag_access()) {
		auto next_tick = std::chrono::steady_clock::now() + std::chrono::microseconds(UpdateInterval);

		if (GameModeController::getCurrentGameMode() == GameMode::Normal)
			m_queue->for_each([this](AbstractGameObject* go) {
				processForces(go);
				processMovement(go, m_map);
			});

		std::this_thread::sleep_until(next_tick);
	}
	GameModeController::physicsLoopIsReady(false);
	clean();
	if (destroy_engine_after_exit)
		delete this;
}