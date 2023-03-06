#pragma once

#include "../Resources/ModelLoader.h"

void DrawCube(bool isBorder, glm::mat4& view, glm::vec3& viewPos, Renderer::Shader& Shader, GLuint& VBO, glm::vec3& position, glm::vec3& scale,
    GLuint& texture1, GLuint& texture2, float angle, glm::vec3& axis, Renderer::Shader* pShSingleColor) {

    //draw obj
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0xFF);

    Shader.Use();
    Shader.SetUniform("view", view);
    Shader.SetUniform("viewPos", viewPos);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(model, position);
    model = glm::scale(model, scale);
    if (angle != 0) {
        model = glm::rotate(model, angle, axis);
    }
    Shader.SetUniform("model", model);

    glBindVertexArray(VBO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    //draw border
    if (isBorder) {

        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        glStencilMask(0x00);

        Renderer::Shader newShader = *pShSingleColor;

        newShader.Use();
        newShader.SetUniform("view", view);
        newShader.SetUniform("color", glm::vec3(1.0f, 1.0f, 1.0f));

        glm::mat4 model = glm::mat4(1.0f);

        model = glm::translate(model, position);
        model = glm::scale(model, glm::vec3(1.05f));
        if (angle != 0) {
            model = glm::rotate(model, angle, axis);
        }
        newShader.SetUniform("model", model);

        glBindVertexArray(VBO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(0xFF);
    }
}

void DrawSkybox(glm::mat4& view, Renderer::Shader& Shader, GLuint VBO, unsigned int cubemapTexture) {
    glDepthMask(GL_FALSE);
    glFrontFace(GL_CCW);

    Shader.Use();
    Shader.SetUniform("view", glm::mat4(glm::mat3(view)));

    glBindVertexArray(VBO);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glDepthMask(GL_TRUE);
    glFrontFace(GL_CW);
}

void DrawReflectCube(glm::mat4& view, Renderer::Shader& Shader, GLuint VBO, glm::vec3& position, unsigned int cubemapTexture, glm::vec3& cameraPos) {
    Shader.Use();
    Shader.SetUniform("view", view);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    Shader.SetUniform("model", model);
    Shader.SetUniform("cameraPos", cameraPos);

    glBindVertexArray(VBO);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void DrawLamp(glm::mat4& view, Renderer::Shader& Shader, GLuint VBO, glm::vec3& position, glm::vec3& scale, glm::vec3& color) {

    Shader.Use();
    Shader.SetUniform("view", view);
    Shader.SetUniform("color", color);

    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(model, position);
    model = glm::scale(model, scale);
    Shader.SetUniform("model", model);

    glBindVertexArray(VBO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void DrawFloor(glm::mat4& view, Renderer::Shader& Shader, GLuint VBO, glm::vec3 color, glm::vec3& position, glm::vec3& scale) {

    Shader.Use();
    Shader.SetUniform("view", view);
    Shader.SetUniform("color", color);

    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(model, position);
    model = glm::scale(model, scale);
    Shader.SetUniform("model", model);

    glBindVertexArray(VBO);
    glDrawArrays(GL_TRIANGLES, 30, 6);
}

void DrawModel(glm::mat4& view, glm::vec3& viewPos, Renderer::Shader& Shader, Model& Model, glm::vec3& position, glm::vec3& scale) {

    Shader.Use();
    Shader.SetUniform("view", view);
    Shader.SetUniform("viewPos", viewPos);
    Shader.SetUniform("time", static_cast<float>(glfwGetTime()));

    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(model, position);
    model = glm::scale(model, scale);
    Shader.SetUniform("model", model);

    Model.Draw(Shader);
}

void DrawGrass(glm::mat4& view, Renderer::Shader& Shader, unsigned int VBO, glm::vec3& position, unsigned int grassTex) {

    Shader.Use();
    Shader.SetUniform("view", view);

    glBindVertexArray(VBO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, grassTex);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    Shader.SetUniform("model", model);

    glDrawArrays(GL_TRIANGLES, 0, 6);
}