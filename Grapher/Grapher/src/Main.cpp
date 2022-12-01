#include "pch.h"

#include "Graph2D.h"
#include "Graph3D.h"
#include "Editor.h"


class Grapher : public Tomato::App
{
public:
	Grapher(const std::unordered_map<std::string, std::any>& args)
		:App(args)
	{
		auto& scene2D = CreateScene("2D Scene");
		scene2D->PushLayer(new Graph2D());
		auto& scene3D = CreateScene("3D Scene");
		scene3D->PushLayer(new Graph3D());

		SetCurrentScene("2D Scene");

		PushImGuiLayer("Editor", new Editor());
	}
	virtual ~Grapher() override = default;
};

Tomato::App* Tomato::App::Create(const std::unordered_map<std::string, std::any>& args)
{
	return new Grapher(args);
}