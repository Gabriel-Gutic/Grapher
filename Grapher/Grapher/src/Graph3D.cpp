#include "pch.h"
#include "Graph3D.h"

using namespace Tomato;


Graph3D::Graph3D()
{
	m_FrameBuffer = FrameBuffer::CreateShared();

	App::GetScene("3D Scene")->GetCamera()->GetComponent<TransformComponent>().Position.y = -1.0f;
}

void Graph3D::OnUpdate(float dt)
{
	auto& camera = App::GetCurrentCamera()->GetComponent<CameraComponent>();
	const auto& window = App::GetWindow();

	camera.SetPerspectiveProjection(45.0f, window->GetAspectRatio(), 0.1f, 100.0f);
	
	DrawLines();
}

void Graph3D::OnEvent(const Event& e)
{

}

void Graph3D::OnGUI()
{
	GUI::RenderWindow(m_FrameBuffer);
}

void Graph3D::DrawLines()
{
	Renderer3D::Get()->DrawLine({ -1.0f,  0.0f,  0.0f }, { 1.0f, 0.0f, 0.0f }, Color::Red);
	Renderer3D::Get()->DrawLine({ 0.0f, -1.0f,  0.0f }, { 0.0f, 1.0f, 0.0f }, Color::Green);
	Renderer3D::Get()->DrawLine({ 0.0f,  0.0f, -1.0f }, { 0.0f, 0.0f, 1.0f }, Color::Blue);

	Float3 last;
	for (float x = -10.0f; x <= 10.0f; x += 0.1f)
	{
		Float3 newPoint = { x / 10.0f, x / 10.0f, 0.0f };
		if (x != -10.0f)
		{
			Renderer3D::Get()->DrawLine(last, newPoint, Color::White);
		}
		last = newPoint;
	}
}
