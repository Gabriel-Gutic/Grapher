#include "pch.h"
#include "Graph2DLayer.h"

#include "Editor.h"

using namespace Tomato;


Graph2DLayer::Graph2DLayer()
	:m_CurrentGraphName(""), m_CurrentGraphIndex(-1)
{
	m_FrameBuffer = FrameBuffer::CreateShared();
	Renderer3D::SetFrameBuffer(m_FrameBuffer);

	/*m_Graphs.emplace_back(std::make_unique<ExplicitGraph2D>("Cbrt", [](float x) {
		return Math::Cbrt(x);
	}));*/

	m_Graphs.emplace_back(std::make_unique<ExplicitGraph2D>("Sin", [](float x) {
		return Math::Sin(x);
	}));

	/*m_Graphs.emplace_back(std::make_unique<ParametricGraph2D>("Parametric", [](float t) {
		return Float2(
			Math::Cos(t),
			Math::Sin(t)
		);
	}, Float2(0, 2.0f * Math::pi + 0.01f)));*/

	/*m_Graphs.emplace_back(std::make_unique<ParametricGraph2D>("Parametric", [](float t) {
		return Float2(
			t,
			Math::Cbrt(t)
		);
	}));*/
}

void Graph2DLayer::OnUpdate(float dt)
{
	Renderer3D::SetFrameBuffer(m_FrameBuffer);

	auto& camera = App::GetCurrentCamera()->GetComponent<CameraComponent>();
	const auto& window = App::GetWindow();

	camera.SetOrthographicProjection(
		-window->GetAspectRatio() * m_CameraOrthoSize, 
		 window->GetAspectRatio() * m_CameraOrthoSize, 
		-m_CameraOrthoSize, m_CameraOrthoSize, 0.1f, 100.0f);

	DrawLines();
}

void Graph2DLayer::OnEvent(const Event& e)
{
	if (e.GetType() == EventType::Wheel)
	{
		auto ev = Event::Cast<WheelEvent>(e);
		if (m_CameraOrthoSize > 2.0f || ev.GetValue() > 0)
			m_CameraOrthoSize += ev.GetValue();
	}
}

void Graph2DLayer::OnGUI()
{
	GUI::RenderWindow(m_FrameBuffer, true);

	ImGui::Begin("Renderer 2D");
	ImGui::SetWindowFontScale(1.2f);

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

	if (m_CurrentGraphIndex != -1)
		m_Graphs[m_CurrentGraphIndex]->UI();

	ImGui::End();
}

void Graph2DLayer::DrawLines()
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

	for (const auto& graph : m_Graphs)
	{
		graph->Draw(m_CameraOrthoSize);
	}
}

bool Graph2DLayer::Valid(float x)
{
	return !(std::isnan(x) || std::isinf(x));
}
