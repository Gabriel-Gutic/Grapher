#include "pch.h"
#include "Editor.h"

using namespace Tomato;


std::unique_ptr<Font> Editor::s_Font = nullptr; 

Editor::Editor()
{
	if (!s_Font)
		s_Font = std::make_unique<Font>("assets/Fonts/Roboto/Roboto-Regular.ttf");

	App::GetWindow()->SetTitle("Grapher");

	Renderer3D::SetBackgroundColor({ 0.1f, 0.105f, 0.11f });

	GUI::ShowDockspace();
	GUI::HideMainMenu();
	GUI::HideSecondMenu();
}

void Editor::OnUpdate(float dt)
{
	const auto& window = App::GetWindow();
	if (Input::Keyboard(TOMATO_KEY_MINUS))
	{
		window->SetFullscreen(true);
	}
	if (Input::Keyboard(TOMATO_KEY_EQUAL))
	{
		window->SetFullscreen(false);
	}
}

void Editor::OnGUI()
{
	//bool show_demo_window = true;
	//if (show_demo_window)
	//	ImGui::ShowDemoWindow(&show_demo_window);

	ImGui::Begin("Menu");

	ImGui::SetWindowFontScale(1.2f);

	std::string fps = "FPS: " + Math::ToString(App::GetFPS());
	ImGui::Text(fps.c_str());

	if (ImGui::Combo("Graph Type", &m_GraphType, "2D\0" "3D"))
	{
		if (m_GraphType == 0)
		{
			App::SetCurrentScene("2D Scene");
		}
		else
		{
			App::SetCurrentScene("3D Scene");
		}
	}

	ImGui::End();
}

Tomato::Font& Editor::GetFont()
{
	return *s_Font;
}
