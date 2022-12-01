#include "pch.h"
#include "Editor.h"

using namespace Tomato;


Editor::Editor()
{
	App::GetWindow()->SetTitle("Grapher");

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
	bool show_demo_window = true;
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	ImGui::Begin("Menu");



	ImGui::End();
}
