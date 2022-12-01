#pragma once


class Editor : public Tomato::ImGuiLayer
{
public:
	Editor();
	~Editor() = default;

	virtual void OnUpdate(float dt) override;
	virtual void OnGUI() override;
private:

};
