#include "pch.h"
#include "Graph2D.h"

using namespace Tomato;

Graph2D::Graph2D()
{
	m_FrameBuffer = FrameBuffer::CreateShared();
	Renderer3D::SetFrameBuffer(m_FrameBuffer);
}

void Graph2D::OnUpdate(float dt)
{
	auto& camera = App::GetCurrentCamera()->GetComponent<CameraComponent>();
	const auto& window = App::GetWindow();

	camera.SetOrthographicProjection(-window->GetAspectRatio() * m_CameraOrthoSize, window->GetAspectRatio() * m_CameraOrthoSize, -m_CameraOrthoSize, m_CameraOrthoSize, 0.1f, 100.0f);

	DrawLines();
}

void Graph2D::OnEvent(const Event& e)
{
	if (e.GetType() == EventType::KeyPress)
	{
		float dt = App::GetDeltaTime();
		auto ev = Event::Cast<KeyPressEvent>(e);
		if (ev.GetKey() == TOMATO_KEY_LEFT)
		{
			m_CameraTheta -= m_CameraSpeed * dt;
			CameraRepositioning();
		}
		if (ev.GetKey() == TOMATO_KEY_RIGHT)
		{
			m_CameraTheta += m_CameraSpeed * dt;
			CameraRepositioning();
		}
	}

	if (e.GetType() == EventType::Wheel)
	{
		auto ev = Event::Cast<WheelEvent>(e);
		if (m_CameraOrthoSize >= 0.2f || ev.GetValue() > 0)
			m_CameraOrthoSize += ev.GetValue() / 10.0f;
	}
}

void Graph2D::OnGUI()
{
	GUI::RenderWindow(m_FrameBuffer);
}

float Graph2D::f(float x)
{
	return Math::Tan(x); 
	// TODO: Repair Pow Function
}

void Graph2D::DrawLines()
{
	float size = m_CameraOrthoSize;

	Renderer3D::Get()->DrawLine({ -size,  0.0f,  0.0f },
		{ size, 0.0f, 0.0f }, Color::Red);
	Renderer3D::Get()->DrawLine({ 0.0f, -size,  0.0f },
		{ 0.0f, size, 0.0f }, Color::Green);

	Float3 last;
	for (float x = -10.0f * size; x <= 10.0f * size; x += size / 100.0f)
	{
		float f_x = f(x) / 10.0f;

		if (std::isnan(f_x))
			continue;

		Float3 newPoint = { x / 10.0f, f_x, 0.0f };
		if ((x != -10.0f * size) && (Math::Abs(f_x) <= size) 
			&& Math::Distance(newPoint, last) < size)
		{
			Renderer3D::Get()->DrawLine(last, newPoint, Color::White);
		}
		last = newPoint;
	}
}

void Graph2D::CameraRepositioning()
{
	auto& pos = App::GetCameraPosition();

	pos.x = 3 * Math::Cos(m_CameraTheta);
	pos.z = 3 * Math::Sin(m_CameraTheta);
}
