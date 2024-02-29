#include "pch.h"
#include "Graph3DLayer.h"

#include "Editor.h"

using namespace Tomato;


Graph3DLayer::Graph3DLayer()
	:m_CameraRadius(5.0f), m_CameraExtraRadius(0.2f), m_CameraSpeed(1.0f),
	m_CameraFOV(45.0f),
	m_CameraTheta(-Math::pi / 12.0f), m_CameraFi(Math::pi / 3.0f),
	m_CameraThetaSpeed(0.5f), m_CameraFiSpeed(0.5f),
	m_CurrentGraphName(nullptr), m_CurrentGraphIndex(-1)
{
	m_FrameBuffer = FrameBuffer::CreateShared(); 

	App::GetScene("3D Scene")->GetCamera()->GetComponent<TransformComponent>().Position.y = -1.0f;


	/*m_Graphs.push_back(std::make_unique<ExplicitGraph3D>("First Graph",
		[](float x, float z) {
			return x * x * x + z * z * z;
		}));*/
	m_Graphs.push_back(std::make_unique<ExplicitGraph3D>("First Graph",
		[](float x, float z) {
			return Math::Sin(x * z);
		}));
	/*m_Graphs.push_back(std::make_unique<ParametricGraph3D>("Sphere Graph", 
		[](float t, float s) {
			return Float3
			(
				Math::Sin(t) * Math::Cos(s),
				Math::Cos(t),
				Math::Sin(t) * Math::Sin(s)
			);
		}, Float2(0.0f, 2 * Math::pi + 0.2f), Float2(0.0f, Math::pi + 0.2f)));*/
	/*
	
	m_Graphs.push_back(std::make_unique<ExplicitGraph3D>("Plane", [](float x, float z) {
		return 1 - x;
	}));*/

	/*m_Graphs.push_back(std::make_unique<ExplicitGraph3D>("Plane1", [](float x, float z) {
		return (x * x + z * z) / 2.0f;
		}));*/

	/*m_Graphs.push_back(std::make_unique<ParametricGraph3D>("Cone Graph",
		[](float t, float s) {
			return Float3
			(
				Math::Sqrt(2 * s) * Math::Cos(t),
				s,
				Math::Sqrt(2 * s) * Math::Sin(t)
			);
		}, Float2(0.0f, 2.0f * Math::pi + 0.1f), Float2(0.0f, 5.0f)));*/

	Renderer3D::SetLight({ 2.0f, 0.0f, 0.0f });
}

void Graph3DLayer::OnUpdate(float dt)
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

	if (Input::Keyboard(TOMATO_KEY_I))
	{
		m_CameraExtraRadius += m_CameraSpeed * dt;
	}
	if (Input::Keyboard(TOMATO_KEY_K))
	{
		if (m_CameraExtraRadius > 0.2f)
			m_CameraExtraRadius -= m_CameraSpeed * dt;
	}

	CameraRotation();

	DrawLines();
}

void Graph3DLayer::OnEvent(const Event& e)
{
	if (e.GetType() == EventType::Wheel)
	{
		auto ev = Event::Cast<WheelEvent>(e);
		if ((ev.GetValue() > 0.0f && m_CameraRadius < 80.0f) ||
			(ev.GetValue() < 0.0f && m_CameraRadius > 1.0f))
			m_CameraRadius += ev.GetValue() / 3.0f;
	}
}

void Graph3DLayer::OnGUI()
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

	//ImGui::SliderFloat("Smoothness", &m_Smoothness, 0.3f, 1.0f);

	if (ImGui::BeginCombo("Functions", m_CurrentGraphName))
	{
		for (int i = 0; i < m_Graphs.size(); i++)
		{
			const char* name = m_Graphs[i]->GetName().c_str();
			bool isSelected = (m_CurrentGraphName == name);
			if (ImGui::Selectable(name, isSelected))
			{
				m_CurrentGraphName = name;
				m_CurrentGraphIndex = i;
			}

			if (isSelected)
				ImGui::SetItemDefaultFocus();
		}

		ImGui::EndCombo();
	}

	if (m_CurrentGraphIndex >= 0)
	{
		m_Graphs[m_CurrentGraphIndex]->UI();
	}

	ImGui::End();
}

void Graph3DLayer::DrawLines() const
{
	float maxPoint = m_CameraRadius / 3.0f;

	for (const auto& graph : m_Graphs)
	{
		graph->Draw(m_CameraRadius);
	}

	Renderer3D::Get()->DrawLine({ -maxPoint,  0.0f,  0.0f }, { maxPoint, 0.0f, 0.0f }, Color::Red);
	Editor::GetFont().RenderText("x", { maxPoint, maxPoint / 20.0f, 0.0f },
		Color::Red, maxPoint
	);

	float d = std::max(static_cast<float>(static_cast<int>(maxPoint / 5.0f)), 1.0f);
	float fontSize = maxPoint * 0.8f;

	for (float x = d; x <= maxPoint; x += d)
	{
		// Positive
		Renderer3D::Get()->DrawLine({ x, -maxPoint / 100.0f, 0.0f },
			{ x, maxPoint / 100.0f, 0.0f },
			Color::Red);
		Editor::GetFont().RenderText(Math::ToString(x), { x, maxPoint / 25.0f, 0.0f },
			Color::Red, fontSize
		);

		// Negative
		Renderer3D::Get()->DrawLine({ -x, -maxPoint / 100.0f, 0.0f },
			{ -x, maxPoint / 100.0f, 0.0f },
			Color::Red);
		Editor::GetFont().RenderText(Math::ToString(-x), { -x, maxPoint / 25.0f, 0.0f },
			Color::Red, fontSize
		);
	}

	Renderer3D::Get()->DrawLine({ 0.0f, -maxPoint,  0.0f }, { 0.0f, maxPoint, 0.0f }, Color::Green);
	Editor::GetFont().RenderText("y", { maxPoint / 20.0f, maxPoint, 0.0f },
		Color::Green, maxPoint
	);

	for (float y = d; y <= maxPoint; y += d)
	{
		float textWidth = Editor::GetFont().GetSize(Math::ToString(y), fontSize).x;

		// Positive
		Renderer3D::Get()->DrawLine({ -maxPoint / 100.0f, y, 0.0f },
			{ maxPoint / 100.0f, y, 0.0f },
			Color::Green);
		Editor::GetFont().RenderText(Math::ToString(y), { -maxPoint / 25.0f - textWidth / 3.0f, y, 0.0f },
			Color::Green, fontSize
		);

		// Negative
		Renderer3D::Get()->DrawLine({ -maxPoint / 100.0f, -y, 0.0f },
			{ maxPoint / 100.0f, -y, 0.0f },
			Color::Green);
		Editor::GetFont().RenderText(Math::ToString(-y), { -maxPoint / 25.0f - textWidth / 3.0f, -y, 0.0f },
			Color::Green, fontSize
		);
	}

	Renderer3D::Get()->DrawLine({ 0.0f,  0.0f, -maxPoint }, { 0.0f, 0.0f, maxPoint }, Color::Blue);
	Editor::GetFont().RenderText("z", { 0.0f, maxPoint / 20.0f , maxPoint },
		Color::Blue, maxPoint
	);

	for (float z = d; z <= maxPoint; z += d)
	{
		// Positive
		Renderer3D::Get()->DrawLine({ 0.0f, -maxPoint / 100.0f, z },
			{ 0.0f, maxPoint / 100.0f, z },
			Color::Blue);
		Editor::GetFont().RenderText(Math::ToString(z), { 0.0f, maxPoint / 25.0f, z },
			Color::Blue, fontSize, Float3(0.0f, 90.0f, 0.0f)
		);

		// Negative
		Renderer3D::Get()->DrawLine({ 0.0f, -maxPoint / 100.0f, -z },
			{ 0.0f, maxPoint / 100.0f, -z },
			Color::Blue);
		Editor::GetFont().RenderText(Math::ToString(-z), { 0.0f, maxPoint / 25.0f, -z },
			Color::Blue, fontSize, Float3(0.0f, 90.0f, 0.0f)
		);
	}


}

void Graph3DLayer::CameraRotation() const
{
	Float3& cameraPos = App::GetScene("3D Scene")->GetCamera()->GetComponent<TransformComponent>().Position;

	cameraPos.y = (m_CameraRadius + m_CameraExtraRadius) * Math::Sin(m_CameraTheta);
	float radius = (m_CameraRadius + m_CameraExtraRadius) * Math::Cos(m_CameraTheta);

	cameraPos.x = radius * Math::Cos(m_CameraFi);
	cameraPos.z = radius * Math::Sin(m_CameraFi);
}