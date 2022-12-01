#pragma once


class Graph2D : public Tomato::Layer
{
public:
	Graph2D();
	~Graph2D() = default;

	virtual void OnUpdate(float dt) override;
	virtual void OnEvent(const Tomato::Event& e) override;
	virtual void OnGUI() override;
private:
	float f(float x);
	void DrawLines();
	void CameraRepositioning();
private:
	float m_CameraTheta = Tomato::Math::pi / 2.0f;
	float m_CameraSpeed = 10.0f;
	float m_CameraOrthoSize = 2.0f;

	std::shared_ptr<Tomato::FrameBuffer> m_FrameBuffer;
};