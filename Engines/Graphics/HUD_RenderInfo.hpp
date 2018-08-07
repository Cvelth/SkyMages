#pragma once
#include "RenderInfo.hpp"
namespace mgl {
	class ShaderProgram;
	class ShaderVariable;
	class Primitive;
}
class EnergyStorage;
class HUD_RenderInfo : public RenderInfo {
	mgl::Primitive *m_energy_bar;
	EnergyStorage *m_energy_source;
	float m_current_energy_percent;
public:
	HUD_RenderInfo(EnergyStorage *energy_source);
	~HUD_RenderInfo();

	virtual void recalculate();
};
