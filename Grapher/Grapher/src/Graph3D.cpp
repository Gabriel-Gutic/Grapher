#include "pch.h"
#include "Graph3D.h"

#include "Editor.h"

using namespace Tomato;


Graph3D::Graph3D()
	:m_CameraRadius(3.0f), m_CameraFOV(45.0f),
	m_CameraTheta(-Math::pi / 12.0f), m_CameraFi(2.0f * Math::pi / 3.0f),
	m_CameraThetaSpeed(0.5f), m_CameraFiSpeed(0.5f),
	m_Smoothness(0.7f)
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
	if (e.GetType() == EventType::Wheel)
	{
		auto ev = Event::Cast<WheelEvent>(e);
		if ((ev.GetValue() > 0.0f && m_CameraRadius < 100.0f) ||
			(ev.GetValue() < 0.0f && m_CameraRadius > 1.0f))
			m_CameraRadius += ev.GetValue() / 10.0f;
	}
}

void Graph3D::OnGUI()
{
	GUI::RenderWindow(m_FrameBuffer, true);

	ImGui::Begin("Renderer3D");
	ImGui::SetWindowFontScale(1.2f);

	float maxPoint = m_CameraRadius / 3.0f;
	std::string inflimit = Math::ToString(-maxPoint);
	std::string suplimit = Math::ToString(maxPoint);

	ImGui::Text("Limits:");
	ImGui::Text(("x: [" + inflimit + ", " + suplimit + "]").c_str());
	ImGui::Text(("y: [" + inflimit + ", " + suplimit + "]").c_str());
	ImGui::Text(("z: [" + inflimit + ", " + suplimit + "]").c_str());

	ImGui::SliderFloat("Smoothness", &m_Smoothness, 0.3f, 1.0f);

	ImGui::End();
}

void Graph3D::DrawLines() const
{
	float maxPoint = m_CameraRadius / 3.0f;

	Renderer3D::Get()->DrawLine({ -maxPoint,  0.0f,  0.0f }, { maxPoint, 0.0f, 0.0f }, Color::Red);
	Editor::GetFont().RenderText("x", { maxPoint, maxPoint / 20.0f, 0.0f },
		Color::Red, maxPoint
	);
	Renderer3D::Get()->DrawLine({ 0.0f, -maxPoint,  0.0f }, { 0.0f, maxPoint, 0.0f }, Color::Green);
	Editor::GetFont().RenderText("y", { maxPoint / 20.0f, maxPoint, 0.0f },
		Color::Green, maxPoint
	);
	Renderer3D::Get()->DrawLine({ 0.0f,  0.0f, -maxPoint }, { 0.0f, 0.0f, maxPoint }, Color::Blue);
	Editor::GetFont().RenderText("z", { 0.0f, maxPoint / 20.0f , maxPoint },
		Color::Blue, maxPoint
	);

	//DrawExplicitGraph([](float x, float z) {
	//	return (x * x + z * z) / 6.0f;
	//}, Float3(1.0f, 0.2f, 1.0f));
	//
	//DrawExplicitGraph([](float x, float z) {
	//	return -(x * x + z * z) / 6.0f;
	//	}, Float3(1.0f, 0.2f, 1.0f));

	//DrawParametricGraph([](float t, float s) {
	//	return Float3
	//	(
	//		Math::Sin(t) * Math::Cos(s),
	//		Math::Sin(t) * Math::Sin(s),
	//		Math::Cos(t)
	//	);
	//}, Float3(1.0f));

	
	DrawExplicitGraph([](float x, float z) {
		return -Math::Sin(x * z);
	}, Float3(0.5f, 0.4f, 0.3f));

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

void Graph3D::DrawExplicitGraph(const std::function<float(float, float)>& f, const Float3& color, const float alpha) const
{
	float dx, dz;
	dx = dz = m_CameraRadius / (100.0f * m_Smoothness);

	float limit = m_CameraRadius / 3.0f;

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

void Graph3D::DrawParametricGraph(const std::function<Float3(float, float)>& f, const Tomato::Float3& color, const float alpha) const
{
	float dt, ds;
	dt = ds = m_CameraRadius / (100.0f * m_Smoothness);

	float limit = m_CameraRadius / 3.0f;

	for (float t = -limit; t <= limit - dt; t += dt)
	{
		for (float s = -limit; s <= limit - ds; s += ds)
		{
			Renderer3D::Get()->DrawQuad(
				f(t, s),
				f(t, s + ds),
				f(t + dt, s + ds),
				f(t + dt, s),
				color * Float3(std::max(t, 0.5f), std::max(0.5f, s), std::max(t, s)),
				alpha
			);
		}
	}
}