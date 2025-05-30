#pragma once

void stop_renderer();

// In render_logic.h
void render_this(unsigned int screenWidth, unsigned int screenHeight,
                 const glm::vec3& cameraPos, const glm::vec3& cameraFront, const glm::vec3& cameraUp);
                 
void prepaire_renderer();