attribute vec4 position;
attribute vec3 normal;
attribute vec2 tcoord;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
varying vec2 tcoordVarying;
void main()
{
tcoordVarying = vec2(tcoord.x, 1.0-tcoord.y);
gl_Position = (projectionMatrix * (viewMatrix * (modelMatrix * position)));
}
