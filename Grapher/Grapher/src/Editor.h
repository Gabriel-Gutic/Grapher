#pragma once


class Editor : public Tomato::ImGuiLayer
{
public:
	Editor();
	~Editor() = default;

	virtual void OnUpdate(float dt) override;
	virtual void OnGUI() override;

	static Tomato::Font& GetFont();
private:
	int m_GraphType = 0;

	static std::unique_ptr<Tomato::Font> s_Font;
};
