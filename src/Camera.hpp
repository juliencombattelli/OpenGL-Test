#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

class Camera
{
public:

    enum class Movement
    {
        Forward,
        Backward,
        Left,
        Right,
        Up,
        Down
    };

    Camera(glm::vec3 pos)
    {
        m_position = pos;
        updateCameraVectors();
    }

    Camera(glm::vec3 pos, glm::vec3 up, float yaw, float pitch)
    {
        m_position = pos;
        m_up = up;
        m_worldUp = up;
        m_yaw = yaw;
        m_pitch = pitch;
        updateCameraVectors();
    }

    glm::mat4 getViewMatrix() const
    {
        return glm::lookAt(m_position, m_position + m_front, m_up);
    }

    glm::mat4 getPerspectiveMatrix(const glm::vec2& screenSize) const
    {
        return glm::perspective(glm::radians(m_zoom), screenSize.x / screenSize.y, m_viewDistance[0], m_viewDistance[1]);
    }

    void processKeyboard(Movement direction, float deltaTime)
    {
        float velocity = m_movementSpeed * deltaTime;
        if (direction == Movement::Forward)
            m_position += m_front * velocity;
        if (direction == Movement::Backward)
            m_position -= m_front * velocity;
        if (direction == Movement::Left)
            m_position -= m_right * velocity;
        if (direction == Movement::Right)
            m_position += m_right * velocity;
        if (direction == Movement::Up)
            m_position.y += velocity;
        if (direction == Movement::Down)
            m_position.y -= velocity;
    }

    void processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= m_mouseSensitivity;
        yoffset *= m_mouseSensitivity;

        m_yaw += xoffset;
        m_pitch += yoffset;

        if (constrainPitch)
        {
            if (m_pitch > 89.0f)
                m_pitch = 89.0f;
            if (m_pitch < -89.0f)
                m_pitch = -89.0f;
        }

        updateCameraVectors();
    }

    void processMouseScroll(float yoffset)
    {
        if (m_zoom >= 1.0f && m_zoom <= 45.0f)
            m_zoom -= yoffset;
        if (m_zoom <= 1.0f)
            m_zoom = 1.0f;
        if (m_zoom >= 45.0f)
            m_zoom = 45.0f;
    }

    glm::vec3 getPosition() const
    {
        return m_position;
    }

private:

    void updateCameraVectors()
    {
        glm::vec3 front;
        front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
        front.y = sin(glm::radians(m_pitch));
        front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
        m_front = glm::normalize(front);
        m_right = glm::normalize(glm::cross(m_front, m_worldUp));
        m_up = glm::normalize(glm::cross(m_right, m_front));
    }

    glm::vec3 m_position{0.0f, 0.0f, 0.0f};
    glm::vec3 m_front{0.0f, 0.0f, -1.0f};
    glm::vec3 m_up{0.0f, 1.0f, 0.0f};
    glm::vec3 m_right{0.0f, 0.0f, 0.0f};
    glm::vec3 m_worldUp{m_up};

    glm::vec2 m_viewDistance{0.1f, 1000.0f};

    float m_yaw{-90.0f};
    float m_pitch{0.0f};

    float m_movementSpeed{2.5f};
    float m_mouseSensitivity{0.1f};
    float m_zoom{45.0f};
};

#endif
