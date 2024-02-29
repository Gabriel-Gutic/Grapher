#include "pch.h"
#include "Graph2D.h"

using namespace Tomato;


Graph2D::Graph2D(std::string_view name)
	:m_Name(name), m_Color(Float3(1.0f, 1.0f, 1.0f)), m_Smoothness(0.5f), isVisible(true)
{
}

const std::string& Graph2D::GetName() const
{
	return m_Name;
}

bool Graph2D::IsVisible() const
{
	return isVisible;
}

bool Graph2D::Valid(float x)
{
	return !(std::isnan(x) || std::isinf(x));
}

ExplicitGraph2D::ExplicitGraph2D(std::string_view name, const std::function<float(float x)>& func)
	:Graph2D(name), m_Func(func)
{
}

void ExplicitGraph2D::Draw(float cameraSize) const
{
	if (isVisible)
	{
		const float size = cameraSize * 0.8f;

		const float dx = size / (m_Smoothness * 10000.0f);
		const float xmax = size / 10.0f;

		Float3 last;
		for (float x = -xmax; x <= xmax - dx; x += dx)
		{
			const float f1_x = m_Func(x);
			const float f2_x = m_Func(x + dx);


			if (!Valid(f1_x) || !Valid(f2_x))
				continue;

			Float3 A = { x * 10.0f, f1_x * 10.0f, 0.0f };
			Float3 B = { (x + dx) * 10.0f, f2_x * 10.0f, 0.0f};
			if ((Math::Abs(f1_x) <= xmax)
				&& (Math::Abs(f2_x) <= xmax))
			{
				Renderer3D::Get()->DrawLine(A, B, m_Color);
			}
		}
	}
}

void ExplicitGraph2D::UI()
{
	ImGui::Checkbox("Visible", &isVisible);
	ImGui::ColorEdit3("Color", m_Color.ToPtr());
	ImGui::SliderFloat("Smoothness", &m_Smoothness, 0.3f, 1.0f);
}

ParametricGraph2D::ParametricGraph2D(std::string_view name, const std::function<Float2(float t)>& func, const Tomato::Float2& limits)
	:Graph2D(name), m_Func(func), m_Limits(limits), m_Interval(limits), m_Speed((limits.y - limits.x) / 100.0f)
{
}

void ParametricGraph2D::Draw(float cameraSize) const
{
	if (isVisible)
	{
		const float size = cameraSize * 0.8f;
		const float xmax = size / 10.0f;

		const float dt = size / (m_Smoothness * 10000.0f);

		for (float t = m_Interval.x; t <= m_Interval.y - dt; t += dt)
		{
			const auto [x1, y1] = m_Func(t).data;
			const auto [x2, y2] = m_Func(t + dt).data;

			if (!Valid(x1) || !Valid(x2) || !Valid(y1) || !Valid(y2))
				continue;

			Float3 A = { x1 * 10.0f, y1 * 10.0f, 0.0f };
			Float3 B = { x2 * 10.0f, y2 * 10.0f, 0.0f };
			if ((Math::Abs(x1) <= xmax) &&
				(Math::Abs(x2) <= xmax) &&
				(Math::Abs(y1) <= xmax) &&
				(Math::Abs(y2) <= xmax) &&
				Math::Distance(A, B) < size)
			{
				Renderer3D::Get()->DrawLine(A, B, m_Color);
			}
		}
	}
}

void ParametricGraph2D::UI()
{
	ImGui::Checkbox("Visible", &isVisible);
	ImGui::ColorEdit3("Color", m_Color.ToPtr());
	ImGui::SliderFloat("Smoothness", &m_Smoothness, 0.3f, 1.0f);
	ImGui::DragFloatRange2("t", &m_Interval.x, &m_Interval.y, m_Speed, m_Limits.x, m_Limits.y);
}
