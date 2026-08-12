#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform sampler2D u_vat_texture;
uniform float u_time;
uniform float u_fps;
uniform int u_totalFrames;
uniform vec3 u_vatMin;
uniform vec3 u_vatMax;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;


void main() {
    float frame = mod(u_time * u_fps, float(u_totalFrames));
    float f0 = floor(frame);
    float f1 = f0 + 1.0;
    float blend = fract(frame);
    if (f1 >= float(u_totalFrames)) f1 = 0.0;

    vec2 texSize = textureSize(u_vat_texture, 0);
    float width = float(texSize.x);
    float height = float(texSize.y);

    //height = float(u_totalFrames * 2);
    float vertexID = float(gl_VertexID);
//     vec2 uv0 = vec2((vertexID + 0.5) / width, (f0 * 2 + 0.5) / height);
//     vec2 uv1 = vec2((vertexID + 0.5) / width, (f1 * 2 + 0.5) / height);
    vec2 uv0 = vec2((vertexID + 0.5) / width, (f0 + 0.5) / height);
    vec2 uv1 = vec2((vertexID + 0.5) / width, (f1 + 0.5) / height);

    vec3 pos0 = texture(u_vat_texture, uv0).rgb;
    vec3 pos1 = texture(u_vat_texture, uv1).rgb;

//     vec3 finalPos = mix(pos0, pos1, blend);
    //finalPos = u_vatMin + finalPos * (u_vatMax - u_vatMin);

    vec3 finalPos = mix(u_vatMin,u_vatMax, pos);

    vec4 worldPos = model * vec4(finalPos, 1.0);
    gl_Position = projection * view * worldPos;

    TexCoord = aTexCoord;
    Normal = mat3(transpose(inverse(model))) * aNormal;
    FragPos = worldPos.xyz;
}
