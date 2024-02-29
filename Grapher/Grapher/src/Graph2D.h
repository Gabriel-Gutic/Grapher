#pragma once


class Graph2D
{
public:
	Graph2D(std::string_view name);

	virtual void Draw(float cameraSize) const = 0;
	virtual void UI() = 0;

	const std::string& GetName() const;
	bool IsVisible() const;
protected:
	static bool Valid(float x);
protected:
	bool isVisible;
	float m_Smoothness;
	Tomato::Float3 m_Color;
	std::string m_Name;
};


class ExplicitGraph2D : public Graph2D
{
public:
	ExplicitGraph2D(std::string_view name, const std::function<float(float x)>& func);
	~ExplicitGraph2D() = default;

	virtual void Draw(float cameraSize) const override;
	virtual void UI() override;
private:
	std::function<float(float x)> m_Func;
};


class ParametricGraph2D : public Graph2D
{
public:
	ParametricGraph2D(std::string_view name, const std::function<Tomato::Float2(float t)>& func, const Tomato::Float2& limits = { -10.0f, 10.0f });
	~ParametricGraph2D() = default;

	virtual void Draw(float cameraSize) const override;
	virtual void UI() override;
private:
	std::function<Tomato::Float2(float t)> m_Func;
	Tomato::Float2 m_Interval;
	float m_Speed;
	const Tomato::Float2 m_Limits;
};