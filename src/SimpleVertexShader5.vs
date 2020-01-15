#version 330 core

//Entrees
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexColor_modelspace;
layout(location = 2) in float vertexDepth_modelspace;

// Sortie
out vec3 colort;
out float depth;

// Params
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main(){
  gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertexPosition_modelspace, 1.0);
  depth       = gl_Position.z + vertexDepth_modelspace;
  colort      = vertexColor_modelspace;
}