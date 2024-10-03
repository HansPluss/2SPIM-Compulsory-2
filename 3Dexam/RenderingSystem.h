#pragma once
#include "Draw.h"
#include "Entity.h"
#include "Resources/Shaders/shaderClass.h"
class Shader;
class RenderingSystem {
public:
    Draw draw;

    void Render(Entity& entity, Shader& shader, glm::mat4 viewproj) {
        // Check if the entity has a RenderComponent
        auto* renderComponent = entity.GetComponent<RenderComponent>();
        auto* positionComponent = entity.GetComponent<PositionComponent>();

        if (renderComponent && positionComponent) {
            // Render based on the shape type
            if (renderComponent->shape == "cube") {
                draw.DrawCube(renderComponent->color, positionComponent->position, renderComponent->size);
            }
            else if (renderComponent->shape == "plane") {
                draw.DrawPlane(renderComponent->color, positionComponent->position, renderComponent->size);
            }
            else if (renderComponent->shape == "sphere") {
                draw.DrawSphere(renderComponent->color, positionComponent->position, renderComponent->size);
            }

            // Call the general render function
            draw.Render(shader, viewproj);
        }
    }
};