#include <iostream>
#include "Entity.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Player.h"

class InputSystem {
    bool bTabWasPressed = false;
public:
    void processInput(Entity& entity, GLFWwindow* window) {
        InputComponent* input = entity.GetComponent<InputComponent>();
        VelocityComponent* velocity = entity.GetComponent<VelocityComponent>();

        if (input) {
            // Update input states based on key presses
            input->updateInput(
                glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS,
                glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS,
                glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS,
                glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS,
                glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS);

			//Get access to player from entity
            Player* player = dynamic_cast<Player*>(&entity);
            if (player) {
				//Opens inventory when tab is pressed
                if (input->bTab && !bTabWasPressed) {
                    std::cout << "Tab key is pressed" << std::endl;
                    player->GetInventory();  // Call the GetInventory() function
                }
            }
			// calls tab once per press
			bTabWasPressed = input->bTab;

            // Update velocity based on input
            if (velocity) {
                velocity->velocity = glm::vec3(0.0f, velocity->velocity.y, 0.f); // Reset velocity

                // Encode movement direction into a single value
                int direction = 0;
                if (input->moveUp) direction |= 1;
                if (input->moveDown) direction |= 2;
                if (input->moveLeft) direction |= 4;
                if (input->moveRight) direction |= 8;

                // Switch case to handle movement direction
                switch (direction) {
                case 1: // Move up (W)
                    velocity->velocity = glm::vec3(0.0f, velocity->velocity.y, -20.0f);
                    break;
                case 2: // Move down (S)
                    velocity->velocity = glm::vec3(0.0f, velocity->velocity.y, 20.0f);
                    break;
                case 4: // Move left (A)
                    velocity->velocity = glm::vec3(-20.0f, velocity->velocity.y, 0.0f);
                    break;
                case 8: // Move right (D)
                    velocity->velocity = glm::vec3(20.0f, velocity->velocity.y, 0.0f);
                    break;
                case 5: // Move up-left (W + A)
                    velocity->velocity = glm::vec3(-20.0f, velocity->velocity.y, -20.0f);
                    break;
                case 9: // Move up-right (W + D)
                    velocity->velocity = glm::vec3(20.0f, velocity->velocity.y, -20.0f);
                    break;
                case 6: // Move down-left (S + A)
                    velocity->velocity = glm::vec3(-20.0f, velocity->velocity.y, 20.0f);
                    break;
                case 10: // Move down-right (S + D)
                    velocity->velocity = glm::vec3(20.0f, velocity->velocity.y, 20.0f);
                    break;
                default:
                    // No movement
                    velocity->velocity = glm::vec3(0.0f, velocity->velocity.y, 0.f);
                    break;

                }
            }
        }
    }
};