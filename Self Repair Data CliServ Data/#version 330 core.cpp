#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragNormal;

void main() {
    FragNormal = normalize(normal);
    gl_Position = projection * view * model * vec4(position, 1.0);
}
in vec3 FragNormal;
out vec4 FragColor;

void main() {
    float intensity = dot(normalize(FragNormal), vec3(0.0, 0.0, 1.0));
    vec3 crystalColor = mix(vec3(0.2, 0.8, 1.0), vec3(1.0, 1.0, 1.0), intensity);
    FragColor = vec4(crystalColor, 0.7); // Teilweise transparente Kristallstruktur
}
#include <GL/glew.h>
#include <GLFW/glfw3.h>

void renderCrystalMatrix() {
    // Beispiel-Kristall mit OpenGL zeichnen
}

int main() {
    if (!glfwInit()) return -1;
    GLFWwindow* window = glfwCreateWindow(800, 600, "Kristall-Matrix", NULL, NULL);
    if (!window) { glfwTerminate(); return -1; }
    glfwMakeContextCurrent(window);
    glewInit();

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        renderCrystalMatrix(); // Deine Kristall-Struktur rendern
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
out vec4 FragColor;
in vec3 FragNormal;
in vec3 ViewDir;

uniform vec3 lightDir;
uniform vec3 crystalColor;
uniform float roughness;
uniform float metallic;

void main() {
    // Normalisierung
    vec3 N = normalize(FragNormal);
    vec3 V = normalize(ViewDir);
    vec3 L = normalize(lightDir);

    // Lambert Beleuchtung
    float NdotL = max(dot(N, L), 0.0);

    // Fresnel-Effekt
    float fresnel = pow(1.0 - max(dot(N, V), 0.0), 5.0);

    // Lichtberechnung
    vec3 diffuse = crystalColor * NdotL;
    vec3 specular = mix(vec3(0.04), crystalColor, metallic) * fresnel * (1.0 - roughness);

    // Ergebnisfarbe mit Transparenz
    FragColor = vec4(diffuse + specular, 0.8);
}
out vec4 FragColor;
in vec3 FragNormal;
in vec3 ViewDir;
uniform vec3 lightDir;
uniform vec3 crystalColor;
uniform float roughness;
uniform float metallic;

void main() {
    vec3 N = normalize(FragNormal);
    vec3 V = normalize(ViewDir);
    vec3 L = normalize(lightDir);

    // Blinn-Phong Spekularlicht
    vec3 R = reflect(-L, N);
    float specularIntensity = pow(max(dot(V, R), 0.0), 16.0 / roughness);
    vec3 specular = crystalColor * specularIntensity * metallic;

    // Diffuses Licht
    float NdotL = max(dot(N, L), 0.0);
    vec3 diffuse = crystalColor * NdotL;

    // Reflexion hinzufügen
    FragColor = vec4(diffuse + specular, 0.8); // Transparenter Kristall mit Reflexionen
}
#include <GLFW/glfw3.h>
float rotationAngle = 0.0f;

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        rotationAngle += 10.0f; // Kristall dreht sich bei Klick
    }
}

int main() {
    if (!glfwInit()) return -1;
    GLFWwindow* window = glfwCreateWindow(800, 600, "Kristall-Matrix", NULL, NULL);
    if (!window) { glfwTerminate(); return -1; }
    glfwMakeContextCurrent(window);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Kristall mit "rotationAngle" drehen
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
out vec4 FragColor;
in vec3 FragNormal;
in vec3 ViewDir;
uniform samplerCube environmentMap;

void main() {
    vec3 N = normalize(FragNormal);
    vec3 V = normalize(ViewDir);
    vec3 R = reflect(-V, N);
    vec3 reflectedColor = texture(environmentMap, R).rgb;

    FragColor = vec4(reflectedColor, 0.8); // Kristall mit spiegelnder Reflexion
}
out vec4 FragColor;
in vec3 FragNormal;
in vec3 ViewDir;
uniform vec3 lightDir;

void main() {
    vec3 N = normalize(FragNormal);
    vec3 V = normalize(ViewDir);
    vec3 L = normalize(lightDir);

    // Lichtbrechung mit Farbstreuung
    float refraction = 1.0 - dot(N, V);
    
    // Farbkomponenten basierend auf Brechungswinkel
    vec3 whiteLight = vec3(1.0, 1.0, 1.0) * refraction;
    vec3 blueLight = vec3(0.2, 0.6, 1.0) * (1.0 - refraction) * 0.8;
    vec3 yellowLight = vec3(1.0, 0.8, 0.2) * (1.0 - refraction) * 0.5;

    // Farbverlauf kombinieren
    vec3 finalColor = mix(whiteLight, mix(blueLight, yellowLight, 0.5), 0.5);
    
    FragColor = vec4(finalColor, 0.85); // Kristall mit dreifarbiger Lichtbrechung
}
out vec4 FragColor;
in vec3 FragNormal;
in vec3 ViewDir;
uniform vec3 lightDir;
uniform int clickState; // Wird von C++ gesetzt, um Klick-Effekt zu ändern

void main() {
    vec3 N = normalize(FragNormal);
    vec3 V = normalize(ViewDir);
    vec3 L = normalize(lightDir);

    float refraction = 1.0 - dot(N, V);
    
    // Farbkomposition basierend auf Klickzustand
    vec3 color1 = vec3(1.0, 1.0, 1.0) * refraction; // Weiß
    vec3 color2 = vec3(0.2, 0.6, 1.0) * (1.0 - refraction); // Blau
    vec3 color3 = vec3(1.0, 0.8, 0.2) * (1.0 - refraction) * 0.5; // Gelb
    vec3 color4 = vec3(1.0, 0.2, 0.2) * (1.0 - refraction) * 0.4; // Rot

    // Farbwechsel bei Klick
    vec3 finalColor;
    if (clickState % 4 == 0) finalColor = mix(color1, color2, 0.5);
    else if (clickState % 4 == 1) finalColor = mix(color2, color3, 0.5);
    else if (clickState % 4 == 2) finalColor = mix(color3, color4, 0.5);
    else finalColor = mix(color4, color1, 0.5);

    FragColor = vec4(finalColor, 0.85);
}
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragNormal;
out vec3 WorldPos;

void main() {
    FragNormal = normalize(mat3(model) * normal); // Normalen korrekt transformieren
    WorldPos = vec3(model * vec4(position, 1.0)); // Weltposition für Beleuchtung
    gl_Position = projection * view * vec4(WorldPos, 1.0);
}
in vec3 FragNormal;
in vec3 WorldPos;
out vec4 FragColor;

uniform vec3 lightDir;
uniform vec3 crystalColor;
uniform float roughness;
uniform float metallic;

void main() {
    vec3 N = normalize(FragNormal);
    vec3 V = normalize(-WorldPos); // Betrachtungsrichtung
    vec3 L = normalize(lightDir);

    float NdotL = max(dot(N, L), 0.0);
    vec3 diffuse = crystalColor * NdotL;

    float fresnel = pow(1.0 - max(dot(N, V), 0.0), 5.0);
    vec3 specular = mix(vec3(0.04), crystalColor, metallic) * fresnel * (1.0 - roughness);

    FragColor = vec4(diffuse + specular, 0.85);
}