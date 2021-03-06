#pragma once
#include "Objects/Objects/EquipableItem.hpp"
#include "Objects/Objects/DependedAcceleratableObjectState.hpp"
#define invert_if(inverted, value) (inverted ? +value : -value)

class EnergyStorage;
class ObjectStorage;
class MainActor;
class FlyEngine : public EquipableItem, public DependedAcceleratableObjectState {
	friend ObjectStorage;
	friend MainActor;
protected:
	float m_maximum_acceleration;
	float m_energy_usage_coefficient;

	float m_up_acceleration_percent;
	float m_down_acceleration_percent;
	float m_left_acceleration_percent;
	float m_right_acceleration_percent;

	std::shared_ptr<EnergyStorage> m_energy_source;
	mutable float m_anti_gravity_expected_mass; //0.f for turned off.
protected:
	float const calculate_acceleration_up() const;
	float const calculate_acceleration_down() const;
	float const calculate_acceleration_left() const;
	float const calculate_acceleration_right() const;

	FlyEngine() : EquipableItem(), DependedAcceleratableObjectState(0.f) { }
public:
	~FlyEngine() {}

	void connect_to_energy_source(std::shared_ptr<EnergyStorage> source) { m_energy_source = source; }

	using EquipableItem::mass;
	using EquipableItem::addMass;
	using EquipableItem::mulMass;

	virtual void accelerate_up(bool inverted = false) {
		auto acceleration = calculate_acceleration_up();
		accelerate_v(invert_if(!inverted, acceleration));
	}
	virtual void accelerate_down(bool inverted = false) {
		auto acceleration = calculate_acceleration_down();
		accelerate_v(invert_if(!inverted, acceleration));
	}
	virtual void accelerate_left(bool inverted = false) {
		auto acceleration = calculate_acceleration_left();
		accelerate_h(invert_if(!inverted, acceleration));
	}
	virtual void accelerate_right(bool inverted = false) {
		auto acceleration = calculate_acceleration_right();
		accelerate_h(invert_if(!inverted, acceleration));
	}
	void activate_anti_gravity(float expected_mass) {
		if (m_anti_gravity_expected_mass == 0.f)
			m_anti_gravity_expected_mass = expected_mass;
		else
			m_anti_gravity_expected_mass = 0.f;
	}

	virtual vector acceleration(scalar const& time_correct) const override;
	virtual vector acceleration(scalar const& mass, scalar const& time_correct) const;
	virtual vector get_acceleration() const;
	virtual void update_acceleration(vector const& acceleration);

	bool is_anti_gravity_active() const { return m_anti_gravity_expected_mass != 0.f; }
private:
	template <typename value_type>
	bool upgrade_value(std::string const& name, value_type const& value);
	template <typename value_type>
	void set_value(std::string const& name, value_type const& value);
};

template<>
inline bool FlyEngine::upgrade_value<float>(std::string const& name, float const& value) {
	if (name == "maximum_acceleration")
		m_maximum_acceleration *= value;
	else if (name == "up_acceleration_percent")
		m_up_acceleration_percent *= value;
	else if (name == "down_acceleration_percent")
		m_down_acceleration_percent *= value;
	else if (name == "left_acceleration_percent")
		m_left_acceleration_percent *= value;
	else if (name == "right_acceleration_percent")
		m_right_acceleration_percent *= value;
	else
		return false;
	return true;
}
template<typename value_type>
inline bool FlyEngine::upgrade_value(std::string const& name, value_type const& value) {
	return false;
}

template<>
inline void FlyEngine::set_value<std::string>(std::string const& name, std::string const& value) {
	if (name == "name")
		m_name += value;
	else if (name == "description")
		m_description += value;
	else
		throw Exceptions::UnsupportedValueException("Unsupported value was passed");
}
template<>
inline void FlyEngine::set_value<float>(std::string const& name, float const& value) {
	if (name == "mass") {
		EquipableItem::mulMass(value);
		EquipableItem::addMass(value);
		DependedAcceleratableObjectState::mulMass(value);
		DependedAcceleratableObjectState::addMass(value);
	} else if (name == "chance_to_take_damage")
		m_chance_to_take_damage += value; 
	else if (name == "energy_usage_coefficient")
		m_energy_usage_coefficient += value;
	else if (name == "maximum_acceleration")
		m_maximum_acceleration += value;
	else if (name == "up_acceleration_percent")
		m_up_acceleration_percent += value;
	else if (name == "down_acceleration_percent")
		m_down_acceleration_percent += value;
	else if (name == "left_acceleration_percent")
		m_left_acceleration_percent += value;
	else if (name == "right_acceleration_percent")
		m_right_acceleration_percent += value;
	else
		throw Exceptions::UnsupportedValueException("Unsupported value was passed");
}
template<typename value_type>
inline void FlyEngine::set_value(std::string const& name, value_type const& value) {
	throw Exceptions::UnsupportedValueException("Unsupported value was passed");
}