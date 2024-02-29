#include "pch.h"
#include "Graph3D.h"

using namespace Tomato;


Graph3D::Graph3D(std::string_view name)
	:m_Name(name), m_Color(Float4(1.0f, 1.0f, 1.0f, 1.0f)), m_Smoothness(1.0f), isVisible(true)
{
}

const std::string& Graph3D::GetName() const
{
	return m_Name;
}

bool Graph3D::IsVisible() const
{
	return isVisible;
}

ExplicitGraph3D::ExplicitGraph3D(std::string_view name, const std::function<float(float x, float z)>& func)
	:Graph3D(name), m_Func(func)
{
}

void ExplicitGraph3D::Draw(float cameraRadius) const
{
	if (!isVisible)
		return;
	float dx, dz;
	dx = dz = cameraRadius / (100.0f * m_Smoothness);

	float limit = cameraRadius / 3.0f;

	for (float x = -limit; x <= limit - dx; x += dx)
	{
		for (float z = -limit; z <= limit - dz; z += dz)
		{
			//float x_color = (x + limit) / (2.0f * limit);
			//float z_color = (z + limit) / (2.0f * limit);
			//float color = std::max((x_color + z_color) / 2.0f, 0.3f);
			Renderer3D::Get()->DrawQuad
			(
				Float3(x, m_Func(x, z), z),
				Float3(x, m_Func(x, z + dz), z + dz),
				Float3(x + dx, m_Func(x + dx, z + dz), z + dz),
				Float3(x + dx, m_Func(x + dx, z), z),
				m_Color.xyz, // * Float3(color, color, color),
				m_Color.w
			);
		}
	}
}

void ExplicitGraph3D::UI()
{
	ImGui::Checkbox("Visible", &isVisible);
	ImGui::ColorEdit4("Color", m_Color.ToPtr());
	ImGui::SliderFloat("Smoothness", &m_Smoothness, 0.3f, 2.0f);
}

ParametricGraph3D::ParametricGraph3D(std::string_view name, const std::function<Float3(float t, float s)>& func,
	const Tomato::Float2& tLimits, const Tomato::Float2& sLimits)
	:Graph3D(name), m_Func(func), 
	m_TLimits(tLimits), m_TValues(tLimits), 
	m_SLimits(sLimits), m_SValues(sLimits),
	m_TSpeed((tLimits.y - tLimits.x) / 100.0f), m_SSpeed((sLimits.y - sLimits.x) / 100.0f)
{
}

void ParametricGraph3D::Draw(float cameraRadius) const
{
	if (!isVisible)
		return;
	if (m_TValues.y == m_TValues.x || m_SValues.y == m_SValues.x)
		return;
	float dt, ds;
	dt = (m_TValues.y - m_TValues.x) / (100.0f * m_Smoothness);
	ds = (m_SValues.y - m_SValues.x) / (100.0f * m_Smoothness);

	for (float t = m_TValues.x; t <= m_TValues.y - dt; t += dt)
	{
		for (float s = m_SValues.x; s <= m_SValues.y - ds; s += ds)
		{
			//float t_color = (t - m_TValues.x) / (m_TValues.y - m_TValues.x);
			//float s_color = (s + m_SValues.x) / (m_SValues.y - m_SValues.x);
			//float color = std::max((t_color + s_color) / 2.0f, 0.3f);
			Renderer3D::Get()->DrawQuad(
				m_Func(t, s),
				m_Func(t, s + ds),
				m_Func(t + dt, s + ds),
				m_Func(t + dt, s),
				m_Color.xyz,// * Float3(color, color, color),
				m_Color.w
			);
		}
	}
}

void ParametricGraph3D::UI()
{
	ImGui::Checkbox("Visible", &isVisible);
	ImGui::ColorEdit4("Color", m_Color.ToPtr());
	ImGui::SliderFloat("Smoothness", &m_Smoothness, 0.3f, 2.0f);
	ImGui::DragFloatRange2("t", &m_TValues.x, &m_TValues.y, m_TSpeed, m_TLimits.x, m_TLimits.y);
	ImGui::DragFloatRange2("s", &m_SValues.x, &m_SValues.y, m_SSpeed, m_SLimits.x, m_SLimits.y);
}
