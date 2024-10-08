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
                renderComponent->Draw.DrawCube(renderComponent->color, positionComponent->position, renderComponent->size);
            }
            else if (renderComponent->shape == "plane") {
                renderComponent->Draw.DrawPlane(renderComponent->color, positionComponent->position, renderComponent->size);
            }
            else if (renderComponent->shape == "sphere") {
                renderComponent->Draw.DrawSphere(renderComponent->color, positionComponent->position, renderComponent->size);
            }
            else if (renderComponent->shape == "boundingbox") {
                renderComponent->Draw.DrawBoundingBox(renderComponent->color, positionComponent->position, renderComponent->size);
            }
            
            // Call the general render function
            renderComponent->Draw.Render(shader, viewproj);
            SetVertecies(entity);
        }
    }
    void Rotate(Entity& entity, float deltaTime) {
        auto* renderComponent = entity.GetComponent<RenderComponent>();
        if (renderComponent) {

            renderComponent->Draw.RotateCube(deltaTime);
        }

    }
    void UpdateGrid() {


    }
    void SetVertecies(Entity& entity) {
        auto* renderComponent = entity.GetComponent<RenderComponent>();
        if (renderComponent) {
            renderComponent->vertices = renderComponent->Draw.GetVertices();
            //std::cout << renderComponent->vertices.size() << "size of vert" << std::endl;

        }
        
        

    }

};