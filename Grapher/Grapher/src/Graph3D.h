#pragma once


class Graph3D
{
public:
	Graph3D(std::string_view name);

	virtual void Draw(float cameraRadius) const = 0;
	virtual void UI() = 0;

	const std::string& GetName() const;
	bool IsVisible() const;
protected:
	bool isVisible;
	float m_Smoothness;
	Tomato::Float4 m_Color;
	std::string m_Name;
};


class ExplicitGraph3D : public Graph3D
{
public:
	ExplicitGraph3D(std::string_view name, const std::function<float(float x, float z)>& func);
	~ExplicitGraph3D() = default;

	virtual void Draw(float cameraRadius) const override;
	virtual void UI() override;
protected:
	std::function<float(float x, float z)> m_Func;
};


class ParametricGraph3D : public Graph3D
{
public:
	ParametricGraph3D(std::string_view name, const std::function<Tomato::Float3(float t, float s)>& func, const Tomato::Float2& tLimits = { -10.0f, 10.0f }, const Tomato::Float2& sLimits = { -10.0f, 10.0f });
	~ParametricGraph3D() = default;

	virtual void Draw(float cameraRadius) const override;
	virtual void UI() override;
protected:
	std::function<Tomato::Float3(float t, float s)> m_Func;
	Tomato::Float2 m_TValues;
	const Tomato::Float2 m_TLimits;
	float m_TSpeed;
	Tomato::Float2 m_SValues;
	const Tomato::Float2 m_SLimits;
	float m_SSpeed;
};
