#pragma once


class Graph3D : public Tomato::Layer
{
public:
	Graph3D();
	~Graph3D() = default;

	virtual void OnUpdate(float dt) override;
	virtual void OnEvent(const Tomato::Event& e) override;
	virtual void OnGUI() override;
private:
	void DrawLines();
private:
	float m_CameraSpeed = 0.1f;
	float m_CameraFOV = 45.0f;

	std::shared_ptr<Tomato::FrameBuffer> m_FrameBuffer;
};
