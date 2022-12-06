#include "pch.h"
#include "Graph2D.h"

#include "Editor.h"

using namespace Tomato;


Graph2D::Graph2D()
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
}

float Graph2D::f(float x)
{
	return (1 - x) / Math::Sqrt(1 + x * x);
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
	//int intSize = static_cast<int>(size);
	//int dx = static_cast<int>(2 * size / 9.0f);
	//for (int x = -intSize; x <= intSize; x += dx)
	//{
	//	float fx = static_cast<float>(x);
	//	Renderer3D::Get()->DrawLine({ fx, -size / 100.0f, 0.0f },
	//		{ fx, size / 100.0f, 0.0f },
	//		Color::Red);
	//	std::stringstream ss;
	//	ss << std::fixed << std::setprecision(0) << round(fx / 10.0f);
	//	PRINT(ss.str());
	//	Editor::GetFont().RenderText(ss.str(), {fx, size / 25.0f, 0.0f},
	//		Color::Red, size
	//	);
	//}
	Renderer3D::Get()->DrawLine({ 0.0f, -size * 1.1f,  0.0f },
		{ 0.0f, size * 1.1f, 0.0f }, Color::Green);
	Editor::GetFont().RenderText("y", { size / 20.0f, size * 1.1f, 0.0f },
		Color::Green, size
	);
	Renderer3D::Get()->DrawTriangle({ 0.0f, size * 1.1f, 0.0f },
		Color::Green, size / 20.0f, 1.0f
	);

	Float3 last;
	for (float x = -size / 10.0f; x <= size / 10.0f; x += size / 5000.0f)
	{
		const float f_x = f(x);

		if (std::isnan(f_x) || std::isinf(f_x))
			continue;

		Float3 newPoint = { x * 10.0f, f_x * 10.0f, 0.0f };
		if ((x != -size / 10.0f) && (Math::Abs(f_x) <= size / 10.0f)
			&& Math::Distance(newPoint, last) < size)
		{
			Renderer3D::Get()->DrawLine(last, newPoint, Color::White);
		}
		last = newPoint;
	}
}
