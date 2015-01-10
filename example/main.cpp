#include <chrono>
#include <thread>
#include <glload/gl_core.hpp>
#include "common/window.hpp"
#include <iostream>

bool run;
struct EventHandler : Peanuts::genericEventHandler{
    using Peanuts::genericEventHandler::operator();
    void operator()(const Peanuts::Event::Close& event) const{
        std::cout << "Window closed!" << std::endl;
        run = false;
    }
    void operator()(const Peanuts::Event::KeyDown& event) const{
        std::cout << "You pressed the quit key, good by!" << std::endl;
        if (event.key == Peanuts::KeyCode::Q){
            run = false;
        }
    }
};

GLuint create_shader(const std::string& source, GLenum shaderType) {
    const GLchar* source_glcstr = static_cast<const GLchar*>(source.c_str());

    GLuint shader = gl::CreateShader(shaderType);
    gl::ShaderSource(shader, 1, &source_glcstr, NULL);
    gl::CompileShader(shader);
    GLint test;
    gl::GetShaderiv(shader, gl::GL_COMPILE_STATUS, &test);
    if(!test) {
        std::cerr << "Shader compilation failed with this message:" << std::endl;
        std::vector<char> compilation_log(512);
        gl::GetShaderInfoLog(shader, compilation_log.size(), NULL, &compilation_log[0]);
        std::cerr << &compilation_log[0] << std::endl;
    }
    return shader;
}

GLuint create_program(const std::string& vertex_shader_code, const std::string& frag_shader_code) {
    GLuint vertexShader = create_shader(vertex_shader_code, gl::GL_VERTEX_SHADER);
    GLuint fragmentShader = create_shader(frag_shader_code, gl::GL_FRAGMENT_SHADER);
    GLuint shaderProgram = gl::CreateProgram();
    gl::AttachShader(shaderProgram, vertexShader);
    gl::AttachShader(shaderProgram, fragmentShader);
    gl::DeleteShader(vertexShader);
    gl::DeleteShader(fragmentShader);
    gl::LinkProgram(shaderProgram);
    gl::UseProgram(shaderProgram);
    return shaderProgram;
}

namespace Peanuts{
    int Main() {
        run = true;
        Peanuts::WindowOptions windowOptions("GL test", Peanuts::Windowed(std::pair<int,int>(640,480),Peanuts::Centered()), Peanuts::OpenGLVersion(1, 4));
        auto win  = Peanuts::Window::create(windowOptions);
        EventHandler eventHandler;
        gl::ClearColor(1.0f, 0.0f, 0.0f, 0.0f);
        std::chrono::milliseconds dura( 2000 );

        GLfloat vertices_position[24] = {
            0.0, 0.0,
            0.5, 0.0,
            0.5, 0.5,
        
            0.0, 0.0,
            0.0, 0.5,
            -0.5, 0.5,
        
            0.0, 0.0,
            -0.5, 0.0,
            -0.5, -0.5,     

            0.0, 0.0,
            0.0, -0.5,
            0.5, -0.5,
        };

        // Create a Vector Buffer Object that will store the vertices on video memory
        GLuint vbo;
        gl::GenBuffers(1, &vbo);
        gl::BindBuffer(gl::GL_ARRAY_BUFFER, vbo);
        gl::BufferData(gl::GL_ARRAY_BUFFER, sizeof(vertices_position), vertices_position, gl::GL_STATIC_DRAW);

        std::string vert_shader_code = (
            "#version 150\n"
            "\n"
            "in vec4 position;\n"
            "\n"
            "void main() {\n"
            "    gl_Position = position;\n"
            "}\n"
        );
        std::string frag_shader_code = (
            "#version 150\n"
            "\n"
            "out vec4 out_color;\n"
            "\n"
            "void main() {\n"
            "    out_color = vec4(0.0, 1.0, 1.0, 1.0);\n"
            "}\n"
        );
        auto program = create_program(vert_shader_code, frag_shader_code);

        GLuint vao;
        gl::GenVertexArrays(1, &vao);
        gl::BindVertexArray(vao);

        GLint position_attribute = gl::GetAttribLocation(program, "position");

        // Specify how the data for position can be accessed
        gl::VertexAttribPointer(position_attribute, 2, gl::GL_FLOAT, gl::GL_FALSE, 0, 0);
        // Enable the attribute
        gl::EnableVertexAttribArray(position_attribute);

        while (run) {
            gl::Clear(gl::GL_COLOR_BUFFER_BIT);
            gl::BindVertexArray(vao);
            gl::DrawArrays(gl::GL_TRIANGLES, 0, 12);

            std::this_thread::sleep_for(dura);
            win->pumpEvents();
            while(auto event = win->pollEvent()){
                boost::apply_visitor(eventHandler, *event);
            }
            win->swapBuffers();
        }
        return 0;
    }
}