#pragma once
#include "Graph2D.h"


class Graph2DLayer : public Tomato::Layer
{
public:
	Graph2DLayer();
	~Graph2DLayer() = default;

	virtual void OnUpdate(float dt) override;
	virtual void OnEvent(const Tomato::Event& e) override;
	virtual void OnGUI() override;
private:
	void DrawExplicitGraph(const std::function<float(float)>& func, const Tomato::Float3& color = Tomato::Color::White);
	void DrawParametricGraph(const std::function<Tomato::Float2(float)>& func, const Tomato::Float2& interval, const Tomato::Float3& color = Tomato::Color::White);
	void DrawLines();
	static bool Valid(float x);
private:
	float m_CameraSpeed = 10.0f;
	float m_CameraOrthoSize = 100.0f;

	std::shared_ptr<Tomato::FrameBuffer> m_FrameBuffer;

	std::vector<std::unique_ptr<Graph2D>> m_Graphs;
	const char* m_CurrentGraphName;
	int m_CurrentGraphIndex;
};
