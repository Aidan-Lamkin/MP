#ifndef LAB05_LAB05_ENGINE_HPP
#define LAB05_LAB05_ENGINE_HPP

#include <CSCI441/FreeCam.hpp>
#include <CSCI441/OpenGLEngine.hpp>
#include <CSCI441/ShaderProgram.hpp>

#include "motorcycle.hpp"
#include "bobomb.hpp"
#include "robot.hpp"
#include "ArcBallCam.hpp"

#include <vector>

class MPEngine : public CSCI441::OpenGLEngine {
public:
    MPEngine();
    ~MPEngine();

    void run() final;

    /// \desc handle any key events inside the engine
    /// \param key key as represented by GLFW_KEY_ macros
    /// \param action key event action as represented by GLFW_ macros
    void handleKeyEvent(GLint key, GLint action);

    /// \desc handle any mouse button events inside the engine
    /// \param button mouse button as represented by GLFW_MOUSE_BUTTON_ macros
    /// \param action mouse event as represented by GLFW_ macros
    void handleMouseButtonEvent(GLint button, GLint action);

    /// \desc handle any cursor movement events inside the engine
    /// \param currMousePosition the current cursor position
    void handleCursorPositionEvent(glm::vec2 currMousePosition);

    /// \desc value off-screen to represent mouse has not begun interacting with window yet
    static constexpr GLfloat MOUSE_UNINITIALIZED = -9999.0f;

private:
    void _setupGLFW() final;
    void _setupOpenGL() final;
    void _setupShaders() final;
    void _setupBuffers() final;
    void _setupScene() final;

    void _cleanupBuffers() final;
    void _cleanupShaders() final;

    void _changeCamera(bool up);

    /// \desc draws everything to the scene from a particular point of view
    /// \param viewMtx the current view matrix for our camera
    /// \param projMtx the current projection matrix for our camera
    void _renderScene(glm::mat4 viewMtx, glm::mat4 projMtx) const;
    /// \desc handles moving our FreeCam as determined by keyboard input
    void _updateScene();

    /// \desc tracks the number of different keys that can be present as determined by GLFW
    static constexpr GLuint NUM_KEYS = GLFW_KEY_LAST;
    /// \desc boolean array tracking each key state.  if true, then the key is in a pressed or held
    /// down state.  if false, then the key is in a released state and not being interacted with
    GLboolean _keys[NUM_KEYS];

    /// \desc last location of the mouse in window coordinates
    glm::vec2 _mousePosition;
    /// \desc current state of the left mouse button
    GLint _leftMouseButtonState;
    GLint _shiftButtonState;

    /// \desc the arcball camera in our world
    CSCI441::ArcballCam* _arcballCam;

    /// \desc the freecam camera in our world
    CSCI441::FreeCam* _freeCam;

    /// \desc the firstPerson camera in our world
    CSCI441::FreeCam* _firstPersonCam;

    GLint _cameraIndex = 0;

    /// \desc int to choose model drawn
    GLint _modelChoice;

    /// \desc our motorcycle model
    Motorcycle* _motorcycle;

    /// \desc our bobomb model
    Bobomb* _bobomb;

    /// \desc our robot model
    Robot* _robot;

    /// \desc the size of the world (controls the ground size and locations of buildings)
    static constexpr GLfloat WORLD_SIZE = 55.0f;
    /// \desc VAO for our ground
    GLuint _groundVAO;
    /// \desc the number of points that make up our ground object
    GLsizei _numGroundPoints;

    /// \desc creates the ground VAO
    void _createGroundBuffers();

    /// \desc smart container to store information specific to each building we wish to draw
    struct BuildingData {
        /// \desc transformations to position and size the building
        glm::mat4 modelMatrix;
        /// \desc color to draw the building
        glm::vec3 color;
    };
    /// \desc information list of all the buildings to draw
    std::vector<BuildingData> _buildings;


    struct TreeData {
        glm::mat4 modelMatrix;
        glm::vec3 leafTranslate;
        glm::vec3 treeColor;
        glm::vec3 leafColor;
    };

    std::vector<TreeData> _trees;

    /// \desc generates building information to make up our scene
    void _generateEnvironment();

    /// \desc shader program that performs lighting
    CSCI441::ShaderProgram* _lightingShaderProgram = nullptr;   // the wrapper for our shader program
    /// \desc stores the locations of all of our shader uniforms
    struct LightingShaderUniformLocations {
        /// \desc precomputed MVP matrix location
        GLint mvpMatrix;
        /// \desc material diffuse color location
        GLint materialColor;
        GLint pointLightColor;
        GLint pointLightPosition;
        GLint lightColor;
        GLint lightDirection;
        GLint normalMatrix;
        GLint modelMtx;
        GLint spotLightPosition;
        GLint spotLightColor;
        GLint spotLightPhi;
        GLint spotLightDirection;
    } _lightingShaderUniformLocations;
    /// \desc stores the locations of all of our shader attributes
    struct LightingShaderAttributeLocations {
        /// \desc vertex position location
        GLint vPos;
        GLint vNormal;
    } _lightingShaderAttributeLocations;

    bool firstPersonOn = false;

    /// \desc precomputes the matrix uniforms CPU-side and then sends them
    /// to the GPU to be used in the shader for each vertex.  It is more efficient
    /// to calculate these once and then use the resultant product in the shader.
    /// \param modelMtx model transformation matrix
    /// \param viewMtx camera view matrix
    /// \param projMtx camera projection matrix
    void _computeAndSendMatrixUniforms(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) const;

    void _drawFirstPerson(glm::mat4 viewMtx, glm::mat4 projMtx);
};

void a3_engine_keyboard_callback(GLFWwindow *window, int key, int scancode, int action, int mods );
void a3_engine_cursor_callback(GLFWwindow *window, double x, double y );
void a3_engine_mouse_button_callback(GLFWwindow *window, int button, int action, int mods );

#endif // LAB05_LAB05_ENGINE_HPP
