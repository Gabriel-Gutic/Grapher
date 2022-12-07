#include "pch.h"
#include "Graph3D.h"

using namespace Tomato;


Graph3D::Graph3D()
	:m_CameraRadius(3.0f), m_CameraFOV(45.0f),
	m_CameraTheta(-Math::pi / 12.0f), m_CameraFi(2.0f * Math::pi / 3.0f),
	m_CameraThetaSpeed(0.5f), m_CameraFiSpeed(0.5f)
{
	m_FrameBuffer = FrameBuffer::CreateShared();

	App::GetScene("3D Scene")->GetCamera()->GetComponent<TransformComponent>().Position.y = -1.0f;
}

void Graph3D::OnUpdate(float dt)
{
	Renderer3D::SetFrameBuffer(m_FrameBuffer);

	auto& camera = App::GetCurrentCamera()->GetComponent<CameraComponent>();
	const auto& window = App::GetWindow();

	camera.SetPerspectiveProjection(m_CameraFOV, window->GetAspectRatio(), 0.1f, 100.0f);

	if (Input::Keyboard(TOMATO_KEY_DOWN))
	{
		m_CameraTheta += m_CameraThetaSpeed * dt;
	}
	if (Input::Keyboard(TOMATO_KEY_UP))
	{
		m_CameraTheta -= m_CameraThetaSpeed * dt;
	}
	if (Input::Keyboard(TOMATO_KEY_LEFT))
	{
		m_CameraFi -= m_CameraFiSpeed * dt;
	}
	if (Input::Keyboard(TOMATO_KEY_RIGHT))
	{
		m_CameraFi += m_CameraFiSpeed * dt;
	}

	CameraRotation();

	DrawLines();
}

void Graph3D::OnEvent(const Event& e)
{

}

void Graph3D::OnGUI()
{
	GUI::RenderWindow(m_FrameBuffer);
}

void Graph3D::DrawLines() const
{
	Renderer3D::Get()->DrawLine({ -1.0f,  0.0f,  0.0f }, { 1.0f, 0.0f, 0.0f }, Color::Red);
	Renderer3D::Get()->DrawLine({ 0.0f, -1.0f,  0.0f }, { 0.0f, 1.0f, 0.0f }, Color::Green);
	Renderer3D::Get()->DrawLine({ 0.0f,  0.0f, -1.0f }, { 0.0f, 0.0f, 1.0f }, Color::Blue);

	DrawGraph([](float x, float z) {
		return Math::Sin(x * z);
	}, Float3(0.0f, 1.0f, 1.0f));

	float alpha2 = 0.5f;
	
	DrawGraph([](float x, float z) {
		return -Math::Sin(x * z);
		}, Float3(1.0f, 0.0f, 0.0f), alpha2);

	//2D Graph
	/* Float3 last;
	for (float x = -10.0f; x <= 10.0f; x += 0.1f)
	{
		Float3 newPoint = { x / 10.0f, x / 10.0f, 0.0f };
		if (x != -10.0f)
		{
			Renderer3D::Get()->DrawLine(last, newPoint, Color::White);
		}
		last = newPoint;
	}*/
}

void Graph3D::CameraRotation() const
{
	Float3& cameraPos = App::GetScene("3D Scene")->GetCamera()->GetComponent<TransformComponent>().Position;

	cameraPos.y = m_CameraRadius * Math::Sin(m_CameraTheta);
	float radius = m_CameraRadius * Math::Cos(m_CameraTheta);

	cameraPos.x = radius * Math::Cos(m_CameraFi);
	cameraPos.z = radius * Math::Sin(m_CameraFi);
}

void Graph3D::DrawGraph(const std::function<float(float, float)>& f, const Float3& color, const float alpha) const
{
	float dx, dz;
	dx = dz = 0.03f;

	float limit = 1.0f;

	for (float x = -limit; x <= limit - dx; x += dx)
	{
		for (float z = -limit; z <= limit - dz; z += dz)
			{
				Renderer3D::Get()->DrawQuad(
					Float3(x, f(x, z), z),
					Float3(x, f(x, z + dz), z + dz),
					Float3(x + dx, f(x + dx, z + dz), z + dz),
					Float3(x + dx, f(x + dx, z), z),
					color * Float3(std::max(x, 0.5f), std::max(0.5f, z), std::max(z, x)),
					alpha
				);
			}
	}
}
