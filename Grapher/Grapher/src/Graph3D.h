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
	void DrawExplicitGraph(const std::function<float(float, float)>& f, const Tomato::Float3& color = Tomato::Float3(1.0f, 1.0f, 1.0f), const float alpha = 1.0f) const;
	void DrawParametricGraph(const std::function<Tomato::Float3(float, float)>& f, const Tomato::Float3& color = Tomato::Float3(1.0f, 1.0f, 1.0f), const float alpha = 1.0f) const;
	void DrawLines() const;
	void CameraRotation() const;
private:
	float m_CameraFOV;
	float m_CameraRadius;
	float m_CameraTheta;
	float m_CameraThetaSpeed;
	float m_CameraFi;
	float m_CameraFiSpeed;

	float m_Smoothness;

	std::shared_ptr<Tomato::FrameBuffer> m_FrameBuffer;
};
