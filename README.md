# 1158engine

1158engine is an ongoing project for a 3D engine built using OpenGL. The engine is designed to provide a flexible framework for rendering 3D graphics and managing game objects through a component-based architecture. While the engine is still in development, it currently supports basic functionalities such as scene management and various components that can be attached to game objects.

Note: The engine is not yet complete; it can handle some fundamental tasks, but it does not currently support the full development of a modern 3D software.

## Table of Contents

- [Features](#features)
- [Getting Started](#getting-started)
- [Usage](#usage)
- [Wiki](#wiki)
- [Contribution](#contribution)

## Features

- **Basic Scene Management**: Create and manage scenes.
- **Component System**: Attach various components to `GameObject` entities for enhanced functionality.
- **Rendering**: Render 3D objects and text using OpenGL
- **Shader Support**: Write and use custom shaders for advanced rendering.

## Getting Started

To get started with this project, follow these steps:

1. **Clone the repository**:
    ```bash
    git clone https://github.com/FabioArdis/1158engine.git
    cd 1158engine
    ```
2. **Install the required dependencies**:
    
    Ensure you have the following dependencies installed on your system:
    - [CMake](https://cmake.org/): Build system
    - [GLAD](https://glad.dav1d.de/): OpenGL function loader.
    - [GLM](https://github.com/g-truc/glm): Mathematics library for graphics software.
    - [GLFW](https://www.glfw.org/): Window management and input handling.
    - [FreeType](https://freetype.org/): Font rendering library.
3. **Build the project**:
    ```bash
    mkdir build
    cd build
    cmake ..
    make
    ```
4. **Run the application**:
    ```bash
    ./1158engine
    ```

## Usage
Simply execute the engine binary generated in the root directory. At this stage of development you can use the demo scene to start testing functionalities and components.

## Wiki
A wiki will be released once the project reaches a more presentable stage. This will provide detailed guides, tutorials, and API references to assist users in understanding and utilizing the engine effectively.

## Contribution
Contribuitions are welcome, feel free to submit issues or pull requests.