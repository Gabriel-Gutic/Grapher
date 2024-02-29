#include "pch.h"

#include "Graph2DLayer.h"
#include "Graph3DLayer.h"
#include "Editor.h"


class Grapher : public Tomato::App
{
public:
	Grapher(const std::unordered_map<std::string, std::any>& args)
		:App(args)
	{
		auto& scene2D = CreateScene("2D Scene");
		scene2D->PushLayer(new Graph2DLayer());
		auto& scene3D = CreateScene("3D Scene");
		scene3D->PushLayer(new Graph3DLayer());

		SetCurrentScene("2D Scene");

		PushImGuiLayer("Editor", new Editor());
	}
	virtual ~Grapher() override = default;
};

Tomato::App* Tomato::App::Create(const std::unordered_map<std::string, std::any>& args)
{
	return new Grapher(args);
}