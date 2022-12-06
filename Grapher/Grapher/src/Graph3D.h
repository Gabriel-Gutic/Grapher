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
	void DrawGraph(std::function<float(float, float)> f, const float alpha = 1.0f);
	void DrawLines() const;
	void CameraRotation() const;
private:
	float m_CameraFOV;
	float m_CameraRadius;
	float m_CameraTheta;
	float m_CameraThetaSpeed;
	float m_CameraFi;
	float m_CameraFiSpeed;

	std::shared_ptr<Tomato::FrameBuffer> m_FrameBuffer;
};
