//
// Created by xxhij on 10/8/2022.
//

#ifndef MP_BOBOMB_HPP
#define MP_BOBOMB_HPP


#include <GL/glew.h>

#include <glm/glm.hpp>

class Bobomb {
public:
    /// \desc creates a simple bobomb in a boot
    /// \param shaderProgramHandle shader program handle that the bobomb should be drawn using
    /// \param mvpMtxUniformLocation uniform location for the full precomputed MVP matrix
    /// \param normalMtxUniformLocation uniform location for the precomputed Normal matrix
    /// \param materialColorUniformLocation uniform location for the material diffuse color
    Bobomb( GLuint shaderProgramHandle, GLint mvpMtxUniformLocation, GLint normalMtxUniformLocation, GLint materialColorUniformLocation,GLint modelMtxUniformLocation );

    /// \desc draws the model bobomb for a given MVP matrix
    /// \param modelMtx existing model matrix to apply to bobomb
    /// \param viewMtx camera view matrix to apply to bobomb
    /// \param projMtx camera projection matrix to apply to bobomb
    /// \note internally uses the provided shader program and sets the necessary uniforms
    /// for the MVP and Normal Matrices as well as the material diffuse color
    void drawBobomb( glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx );

    /// \desc simulates the bobomb driving by rotating the wheels and increasing its position relative to its direction
    void driveForward(GLfloat worldSize);
    /// \desc simulates the bobomb driving by rotating the wheels and decreasing its position relative to its direction
    void driveBackward(GLfloat worldSize);
    /// \desc rotates bobomb's direction based on key input
    void rotateBobomb(GLuint key);

    // position getters, setters
    glm::vec3 getPosition();
    void setPosition(glm::vec3 nPosit);

    // direction setter, getter -- setter goes unused
    GLfloat getDirection();
    void setDirection(GLfloat nDirec);

    void _updateFlicker();


private:

    /// \desc current position of bobomb in space (encoded as vec3 x,y,z coordinates)
    glm::vec3 _bobombPosition;
    /// \desc angle indicating direction bobomb is facing in space (encoded as float representing theta)
    GLfloat _bobombDirection;
    GLfloat _bobombDirectionRotationSpeed;

    /// \desc current angle of rotation for the wheels (float as theta)
    GLfloat _wheelAngle;
    /// \desc one rotation step
    GLfloat _wheelAngleRotationSpeed;

    /// \desc handle of the shader program to use when drawing the bobomb
    GLuint _shaderProgramHandle;
    /// \desc stores the uniform locations needed for the plan information
    struct ShaderProgramUniformLocations {
        /// \desc location of the precomputed ModelViewProjection matrix
        GLint mvpMtx;
        /// \desc location of the precomputed Normal matrix
        GLint normalMtx;
        /// \desc location of the material diffuse color
        GLint materialColor;
        /// \desc location of the model matrix
        GLint modelMtx;
    } _shaderProgramUniformLocations;


    /// \desc color the bobomb's body
    glm::vec3 _colorBody;
    /// \desc amount to scale the bobomb's body by
    glm::vec3 _scaleBody;

    /// \desc color the bobomb's eye
    glm::vec3 _colorEye;
    /// \desc amount to scale the bobomb's eye by
    glm::vec3 _scaleEye;

    /// \desc color the bobomb's fuse
    glm::vec3 _colorFuse;
    /// \desc amount to translate the bobomb's fuse by
    glm::vec3 _transFuse;

    /// \desc color the bobomb's wheels
    glm::vec3 _colorWheel;

    /// \desc color the bobomb's boot
    glm::vec3 _colorBoot;
    /// \desc amount(s) to translate the bobomb's boot by
    glm::vec3 _transBootA;
    glm::vec3 _transBootB;
    glm::vec3 _transBootC;

    // flicker colors, variables --
    // used for non-moving idle animation,
    // where the wick flicks between red and yellow.

    glm::vec3 _colorFlicker;
    glm::vec3 _colorFlickerEx;
    bool _isFlicker;
    int _flickerCheck;


    /// \desc draws just the bobomb's body
    /// \param modelMtx existing model matrix to apply to bobomb
    /// \param viewMtx camera view matrix to apply to bobomb
    /// \param projMtx camera projection matrix to apply to bobomb
    void _drawBobombBody(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const;
    /// \desc draws a single eye
    /// \param isLeftWing true if left eye, false if right eye (controls if translation applied)
    /// \param modelMtx existing model matrix to apply to bobomb
    /// \param viewMtx camera view matrix to apply to bobomb
    /// \param projMtx camera projection matrix to apply to bobomb
    void _drawBobombEye(bool isLeftEye, glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const;
    /// \desc draws the fuse (and animated flicker) of the bobomb
    /// \param modelMtx existing model matrix to apply to bobomb
    /// \param viewMtx camera view matrix to apply to bobomb
    /// \param projMtx camera projection matrix to apply to bobomb
    void _drawBobombFuse(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const;
    void _drawBobombFlicker(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const;
    /// \desc draws the boot car of the bobomb
    /// \param modelMtx existing model matrix to apply to bobomb
    /// \param viewMtx camera view matrix to apply to bobomb
    /// \param projMtx camera projection matrix to apply to bobomb
    void _drawBobombBoot(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const;
    /// \desc draws the wheels of the boot of the bobomb
    /// \param modelMtx existing model matrix to apply to bobomb
    /// \param viewMtx camera view matrix to apply to bobomb
    /// \param projMtx camera projection matrix to apply to bobomb
    void _drawBobombWheels(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const;


    /// \desc precomputes the matrix uniforms CPU-side and then sends them
    /// to the GPU to be used in the shader for each vertex.  It is more efficient
    /// to calculate these once and then use the resultant product in the shader.
    /// \param modelMtx model transformation matrix
    /// \param viewMtx camera view matrix
    /// \param projMtx camera projection matrix
    void _computeAndSendMatrixUniforms(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) const;
};


#endif //MP_BOBOMB_HPP
