#pragma once
#include "Math.h"
#include "Timer.h"

namespace dae
{
	struct Camera
	{
		Camera() = default;

		Camera(const Vector3& _origin, float _fovAngle) :
			m_Origin{ _origin },
			m_FovAngle{ _fovAngle }
		{
		}


		Vector3 m_Origin{};
		float m_FovAngle{ 90.f };
		float m_Fov{ tanf((m_FovAngle * TO_RADIANS) / 2.f) };

		Vector3 m_Forward{ Vector3::UnitZ };
		Vector3 m_Up{ Vector3::UnitY };
		Vector3 m_Right{ Vector3::UnitX };

		float m_TotalPitch{};
		float m_TotalYaw{};
		float m_SpeedCamera{ 0.01f };
		float m_SpeedMovement{ 100 };

		float m_Near{ 0.1f };
		float m_Far{ 1000.0f };

		Matrix m_InvViewMatrix{};
		Matrix m_ViewMatrix{};
		Matrix m_ProjectionMatrix{};

		Matrix m_WorldViewProjectMatrix{};

		float m_AspectRatio{};

		float m_MovementSpeed{ 300 };

		void Initialize(float _fovAngle = 90.f, Vector3 _origin = { 0.f,0.f,0.f }, float aspectRatio = 1.f)
		{
			m_FovAngle = _fovAngle;
			m_Fov = tanf((m_FovAngle * TO_RADIANS) / 2.f);

			m_Origin = _origin;

			m_AspectRatio = aspectRatio;
			CalculateViewMatrix();
			CalculateProjectionMatrix();
		}

		void CalculateViewMatrix()
		{
			m_ViewMatrix =
			{
				m_Right,
				m_Up,
				m_Forward,
				m_Origin
			};

			m_InvViewMatrix = Matrix::Inverse(m_ViewMatrix);
		}

		void CalculateProjectionMatrix()
		{
			//TODO W3
			m_ProjectionMatrix = {
			{1 / (m_AspectRatio * m_Fov), 0, 0, 0},
			{0, 1 / m_Fov, 0, 0},
			{0, 0, (m_Far / (m_Far - m_Near)), 1},
			{0, 0, -((m_Far * m_Near) / (m_Far - m_Near)), 0}
			};
			//ProjectionMatrix => Matrix::CreatePerspectiveFovLH(...) [not implemented yet]
			//DirectX Implementation => https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dxmatrixperspectivefovlh
		}

		Matrix GetProjectionViewMatrix() const
		{
			return m_InvViewMatrix * m_ProjectionMatrix;
		}

		Vector3 GetOrigin() const
		{
			return m_Origin;
		}

		void Update(const Timer* pTimer)
		{
			const float deltaTime = pTimer->GetElapsed();

			//Keyboard Input
			const uint8_t* pKeyboardState = SDL_GetKeyboardState(nullptr);

			if (pKeyboardState[SDL_SCANCODE_LSHIFT])
			{
				m_SpeedMovement = m_MovementSpeed;
			}
			else
				m_SpeedMovement = 100;

			if (pKeyboardState[SDL_SCANCODE_W] || pKeyboardState[SDL_SCANCODE_UP])
			{
				m_Origin += m_Forward * m_SpeedMovement * deltaTime;
			}
			if (pKeyboardState[SDL_SCANCODE_S] || pKeyboardState[SDL_SCANCODE_DOWN])
			{
				m_Origin -= m_Forward * m_SpeedMovement * deltaTime;
			}
			if (pKeyboardState[SDL_SCANCODE_A] || pKeyboardState[SDL_SCANCODE_LEFT])
			{
				m_Origin -= m_Right * m_SpeedMovement * deltaTime;
			}
			if (pKeyboardState[SDL_SCANCODE_D] || pKeyboardState[SDL_SCANCODE_RIGHT])
			{
				m_Origin += m_Right * m_SpeedMovement * deltaTime;
			}

			//Mouse Input
			int mouseX{}, mouseY{};
			const uint32_t mouseState = SDL_GetRelativeMouseState(&mouseX, &mouseY);

			const float verticalMovement = mouseY * m_SpeedCamera;
			const float horizontalMovement = mouseX * m_SpeedCamera;
			if (mouseState & SDL_BUTTON(SDL_BUTTON_RIGHT) && mouseState & SDL_BUTTON(SDL_BUTTON_LEFT))
			{
				m_Origin.y += verticalMovement * m_SpeedMovement;
			}
			else if (mouseState & SDL_BUTTON(SDL_BUTTON_RIGHT))
			{
				m_TotalPitch += horizontalMovement;
				m_TotalYaw -= verticalMovement;
				std::cout << m_TotalPitch;
			}
			else if (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT))
			{
				m_Origin -= m_Forward * verticalMovement * m_SpeedMovement;
				m_TotalPitch += horizontalMovement;
			}
			UpdateCamera();

			//Update Matrices
			CalculateProjectionMatrix();
		}

		void UpdateCamera()
		{
			const Matrix final{ Matrix::CreateRotationX(m_TotalYaw) * Matrix::CreateRotationY(m_TotalPitch) };

			m_Forward = final.TransformVector(Vector3::UnitZ);
			m_Forward.Normalize();

			m_Right = Vector3::Cross(Vector3::UnitY, m_Forward).Normalized();
			m_Up = Vector3::Cross(m_Forward, m_Right).Normalized();

			CalculateViewMatrix();
		}
	};
}