#include "pch.h"
#include "Graph2D.h"

#include "Editor.h"

using namespace Tomato;


Graph2D::Graph2D()
	:m_Interval(0.0f, 4 * Math::pi)
{
	m_FrameBuffer = FrameBuffer::CreateShared();
	Renderer3D::SetFrameBuffer(m_FrameBuffer);
}

void Graph2D::OnUpdate(float dt)
{
	Renderer3D::SetFrameBuffer(m_FrameBuffer);

	auto& camera = App::GetCurrentCamera()->GetComponent<CameraComponent>();
	const auto& window = App::GetWindow();

	camera.SetOrthographicProjection(-window->GetAspectRatio() * m_CameraOrthoSize, window->GetAspectRatio() * m_CameraOrthoSize, -m_CameraOrthoSize, m_CameraOrthoSize, 0.1f, 100.0f);

	DrawLines();
}

void Graph2D::OnEvent(const Event& e)
{
	if (e.GetType() == EventType::Wheel)
	{
		auto ev = Event::Cast<WheelEvent>(e);
		if (m_CameraOrthoSize > 2.0f || ev.GetValue() > 0)
			m_CameraOrthoSize += ev.GetValue();
	}
}

void Graph2D::OnGUI()
{
	GUI::RenderWindow(m_FrameBuffer, true);

	ImGui::Begin("Renderer 2D");
	ImGui::SetWindowFontScale(1.2f);

	ImGui::SliderFloat2("t", m_Interval.ToPtr(), 0.0f, 8 * Math::pi);

	ImGui::End();
}

void Graph2D::DrawExplicitGraph(const std::function<float(float)>& func, const Tomato::Float3& color)
{
	const float size = m_CameraOrthoSize * 0.8f;

	Float3 last;
	for (float x = -size / 10.0f; x <= size / 10.0f; x += size / 5000.0f)
	{
		const float f_x = func(x);

		if (!Valid(x))
			continue;

		Float3 newPoint = { x * 10.0f, f_x * 10.0f, 0.0f };
		if ((x != -size / 10.0f) && (Math::Abs(f_x) <= size / 10.0f)
			&& Math::Distance(newPoint, last) < size)
		{
			Renderer3D::Get()->DrawLine(last, newPoint, color);
		}
		last = newPoint;
	}
}

void Graph2D::DrawParametricGraph(const std::function<Tomato::Float2(float)>& func, const Float2& interval, const Tomato::Float3& color)
{
	const float size = m_CameraOrthoSize * 0.8f;

	const float dt = size / 5000.0f;

	for (float t = interval.x; t <= interval.y - dt; t += dt)
	{
		const auto [x1, y1] = func(t).data;
		const auto [x2, y2] = func(t + dt).data;

		if (!Valid(x1) || !Valid(x2) || !Valid(y1) || !Valid(y2))
			continue;

		Float3 A = { x1 * 10.0f, y1 * 10.0f, 0.0f };
		Float3 B = { x2 * 10.0f, y2 * 10.0f, 0.0f };
		if ((Math::Abs(x1) <= size / 10.0f) && 
			(Math::Abs(x2) <= size / 10.0f) &&
			(Math::Abs(y1) <= size / 10.0f) &&
			(Math::Abs(y2) <= size / 10.0f) && 
			Math::Distance(A, B) < size)
		{
			Renderer3D::Get()->DrawLine(A, B, color);
		}
	}
}

void Graph2D::DrawLines()
{
	const float size = m_CameraOrthoSize * 0.8f;

	Renderer3D::Get()->DrawLine({ -size * 1.1f,  0.0f,  0.0f },
		{ size * 1.1f, 0.0f, 0.0f }, Color::Red);
	Editor::GetFont().RenderText("x", { size * 1.1f, size / 20.0f, 0.0f },
		Color::Red, size
		);
	Renderer3D::Get()->DrawTriangle({ size * 1.1f, 0.0f, 0.0f },
		Color::Red, size / 20.0f, 1.0f,
		Quaternion::Rotation(0.0f, 0.0f, -90.0f).ToMat4()
	);

	float d = std::max(static_cast<int>(size / 60.0f) * 10.0f, 10.0f);
	float fontSize = size * 0.8f;

	for (float x = d; x <= size; x += d)
	{
		// Positive
		Renderer3D::Get()->DrawLine({ x, -size / 100.0f, 0.0f },
			{ x, size / 100.0f, 0.0f },
			Color::Red);
		Editor::GetFont().RenderText(Math::ToString(x / 10.0f), {x, size / 25.0f, 0.0f},
			Color::Red, fontSize
		);

		// Negative
		Renderer3D::Get()->DrawLine({ -x, -size / 100.0f, 0.0f },
			{ -x, size / 100.0f, 0.0f },
			Color::Red);
		Editor::GetFont().RenderText(Math::ToString(-x / 10.0f), { -x, size / 25.0f, 0.0f },
			Color::Red, fontSize
		);
	}

	Renderer3D::Get()->DrawLine({ 0.0f, -size * 1.1f,  0.0f },
		{ 0.0f, size * 1.1f, 0.0f }, Color::Green);
	Editor::GetFont().RenderText("y", { size / 20.0f, size * 1.1f, 0.0f },
		Color::Green, size
	);
	Renderer3D::Get()->DrawTriangle({ 0.0f, size * 1.1f, 0.0f },
		Color::Green, size / 20.0f, 1.0f
	);

	for (float y = d; y <= size; y += d)
	{
		float textWidth = 0.0f;
		// Positive
		Renderer3D::Get()->DrawLine({ -size / 100.0f, y, 0.0f },
			{ size / 100.0f, y, 0.0f },
			Color::Green);

		textWidth = Editor::GetFont().GetSize(Math::ToString(y / 10.0f), fontSize).x;
		Editor::GetFont().RenderText(Math::ToString(y / 10.0f), { -size / 25.0f - textWidth / 3.0f, y, 0.0f },
			Color::Green, fontSize
		);

		// Negative
		Renderer3D::Get()->DrawLine({ -size / 100.0f, -y, 0.0f },
			{ size / 100.0f, -y, 0.0f },
			Color::Green);
		textWidth = Editor::GetFont().GetSize(Math::ToString(-y / 10.0f), fontSize).x;
		Editor::GetFont().RenderText(Math::ToString(-y / 10.0f), { -size / 25.0f - textWidth / 3.0f, -y, 0.0f },
			Color::Green, fontSize
		);
	}

	// Draw graphs here

	//DrawExplicitGraph([](float x) {
	//	return Math::Sin(x);
	//});

	//DrawParametricGraph([](float t) {
	//	return Float2(
	//		3 * Math::Cos(t),
	//		5 * Math::Sin(t)
	//	);
	//}, m_Interval);

	DrawParametricGraph([](float t) {
		return Float2(
			t * Math::Cos(2 * t),
			t * Math::Sin(t)
		);
	}, m_Interval);
}

bool Graph2D::Valid(float x)
{
	return !(std::isnan(x) || std::isinf(x));
}
