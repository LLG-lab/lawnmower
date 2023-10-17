#include <GL/glew.h>
#include <sstream>
#include <csignal>
#include <stdexcept>

#include <client.hpp>

#include <easylogging++.h>

#include "lawnmower.pb.h"

#define client_log(__X__) \
    CLOG(__X__, "client")

#define TIMER_INTERVAL_MILLIS 50

const char *client::vertex_shader_code_ =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aColor;\n"
    "layout (location = 2) in vec2 aTexCoord;\n"
    "\n"
    "out vec3 ourColor;\n"
    "out vec2 TexCoord;\n"
    "\n"
    "void main()\n"
    "{\n"
    "    gl_Position = vec4(aPos, 1.0);\n"
    "    ourColor = aColor;\n"
    "    TexCoord = aTexCoord;\n"
    "}";

const char *client::fragment_shader_code_ =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "\n"
    "in vec3 ourColor;\n"
    "in vec2 TexCoord;\n"
    "\n"
    "uniform sampler2D ourTexture;\n"
    "\n"
    "void main()\n"
    "{\n"
    "    FragColor = texture(ourTexture, TexCoord);\n"
    "}";

client::client(GLFWwindow *window, boost::asio::io_context &io_context, const lawnmowerc_settings &cfg)
    : ioctx_ {io_context},
      timer_ {io_context},
      settings_ { cfg },
      sb_ { settings_.get_camera_width(), settings_.get_camera_height() },
      camera_ { settings_.get_camera_width(), settings_.get_camera_height() },
      console_ {80, 20},
      connection_ { new ssl_connection(ioctx_, settings_, console_) },
      connection_stage_ { connection_stage::DISCONNECTED },
      joystick_id_ {-1},
      window_ {window},
      VBO_ {0},
      VAO_ {0},
      EBO_ {0},
      texture_ {0}
{
    el::Loggers::getLogger("client", true);

    setup_connection_callbacks();
    init_gdi();
    init_timer(TIMER_INTERVAL_MILLIS);
}

client::~client(void)
{
    if (VAO_) glDeleteVertexArrays(1, &VAO_);
    if (VBO_) glDeleteBuffers(1, &VBO_);
    if (EBO_) glDeleteBuffers(1, &EBO_);

    timer_.cancel();
}

void client::connect(void)
{
    if (! test_and_setup_joystick())
    {
        console_ << "E: Session cannot be establish due to no joystick\n"
                 << "   is available or has insufficient capabilities.\n";

        return;
    }
    
    connection_stage_ = connection_stage::CONNECTING;
    connection_ -> connect();
}

void client::on_alarm(void)
{
    //
    // Should terminate program?
    //

    if (glfwWindowShouldClose(window_))
    {
        // raise signal.
        std::raise(SIGTERM);
        return;
    }

    process_input();
    render();

    if (connection_ -> can_send())
    {
        ProtoClientMessage client_msg;

        bool have_anything_to_send = false;

        if (steering_wheel_.is_data())
        {
            have_anything_to_send = true;
            client_msg.set_steeringwheelreq(steering_wheel_.get_data());
        }

        if (acceleration_pedal_.is_data())
        {
            have_anything_to_send = true;
            client_msg.set_accelerationpedalreq(acceleration_pedal_.get_data());
        }

        if (have_anything_to_send)
        {
            #if GOOGLE_PROTOBUF_VERSION < 3004000
            int size = client_msg.ByteSize();
            #else
            int size = client_msg.ByteSizeLong();
            #endif

            std::vector<char> buffer(size);

            client_msg.SerializeToArray(&buffer.front(), size);

            connection_ -> send_data(buffer);
        }
    }
}

void client::on_device_data(const std::vector<char> &data)
{
    ProtoAgentMessage agent_msg;

    agent_msg.ParseFromArray(&data.front(), data.size());

    if (agent_msg.has_videoframe())
    {
        camera_.put_data(agent_msg.videoframe());
    }

    if (agent_msg.has_steeringwheelpotentiometer())
    {
        indications_.set_wheel_turn(agent_msg.steeringwheelpotentiometer());
    }

    if (agent_msg.has_accelerationpedalpotentiometer())
    {
        indications_.set_gas(agent_msg.accelerationpedalpotentiometer());
    }
}

void client::on_connected(void)
{
    connection_stage_ = connection_stage::CONNECTED;

    console_ << "-----------------------\n"
             << "CONNECTION ESTABLISHED.\n";

    console_.set_preview_mode();
    indications_.visible();
}

void client::on_connection_error(const boost::system::error_code &ec)
{
    connection_stage_ = connection_stage::DISCONNECTED;
    connection_.reset(new ssl_connection(ioctx_, settings_, console_));
    setup_connection_callbacks();

    console_ << "DISCONNECTED\n";
    console_ << "Press \"START\" to reconnect.\n";
    camera_.monochrome();
    indications_.invisible();

    console_.set_permanent_mode();
}

void client::process_input(void)
{
    glfwPollEvents();

    if (glfwGetKey(window_, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window_, true);
    }

    int buttons_count = 0;
    const unsigned char *buttons = glfwGetJoystickButtons(joystick_id_, &buttons_count);
    int axes_count = 0;
    const float *axes = glfwGetJoystickAxes(joystick_id_, &axes_count);
    int index;

    index = settings_.get_steering_wheel_index();
    if (index < axes_count)
    {
        steering_wheel_.put_data(axes[index]);
    }

    index = settings_.get_acceleration_pedal_index();
    if (index < axes_count)
    {
        acceleration_pedal_.put_data((-1.0)*axes[index]);
    }

    if (connection_stage_ == connection_stage::DISCONNECTED)
    {
        index = settings_.get_start_button_index();
        if (index < buttons_count && buttons[index] == GLFW_PRESS)
        {
            console_ << "Connecting to lawnmower device...\n";

            connect();
        }
    }
    else
    {
        index = settings_.get_triangle_button_index();
        if (index < buttons_count && buttons[index] == GLFW_PRESS)
        {
            console_.set_permanent_mode();
        }

        index = settings_.get_circle_button_index();
        if (index < buttons_count && buttons[index] == GLFW_PRESS)
        {
            console_.set_preview_mode();
        }
    }
}

void client::render(void)
{
    //
    // At first put all visible items together onto sb_.
    //

    camera_.render(sb_);
    console_.render(sb_);
    indications_.render(sb_);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, sb_.get_width(), sb_.get_height(), 0, GL_RGB, GL_UNSIGNED_BYTE, sb_.get_raw_data());
    glGenerateMipmap(GL_TEXTURE_2D);

    //
    // Bind Texture.
    //

    glBindTexture(GL_TEXTURE_2D, texture_);

    //
    // Render container.
    //

    glUseProgram(ID_);
    glBindVertexArray(VAO_);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    //
    // Swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    //

    glfwSwapBuffers(window_);
}

void client::init_gdi(void)
{
    //
    // Build and compile shader zprogram.
    //

    unsigned int vertex, fragment;
    int success;
    char info_log[1024];

    // Vertex shader.
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &client::vertex_shader_code_, NULL);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);

    if (! success)
    {
        glGetShaderInfoLog(vertex, 1024, NULL, info_log);

        std::ostringstream msg;
        msg << "Vertex shader compilation error: " << info_log; 

        throw std::runtime_error(msg.str());
    }


    // Fragment shader.
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &client::fragment_shader_code_, NULL);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);

    if (! success)
    {
        glGetShaderInfoLog(fragment, 1024, NULL, info_log);

        std::ostringstream msg;
        msg << "Fragment shader compilation error: " << info_log; 

        throw std::runtime_error(msg.str());
    }


    // Shader program.
    ID_ = glCreateProgram();
    glAttachShader(ID_, vertex);
    glAttachShader(ID_, fragment);
    glLinkProgram(ID_);
    glGetProgramiv(ID_, GL_LINK_STATUS, &success);

    if (! success)
    {
        glGetProgramInfoLog(ID_, 1024, NULL, info_log);

        std::ostringstream msg;
        msg << "Shader program linking error: " << info_log; 

        throw std::runtime_error(msg.str());
    }
        
    // Delete the shaders as they're linked into program now and no longer necessary.
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    //
    // Set up vertex data (and buffer(s)) and configure vertex attributes.
    //

    float vertices[] = {
        // positions          // colors           // texture coords
         1.0f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
    };

    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    glGenVertexArrays(1, &VAO_);
    glGenBuffers(1, &VBO_);
    glGenBuffers(1, &EBO_);

    glBindVertexArray(VAO_);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    //
    // Create a texture.
    //

    glGenTextures(1, &texture_);
    glBindTexture(GL_TEXTURE_2D, texture_);

    // Set the texture wrapping parameters.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Set texture filtering parameters.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //
    // Setup alignment for screen buffer purposes.
    //

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}

void client::init_timer(int millis)
{
    timer_.cancel();
    timer_.expires_after(boost::asio::chrono::milliseconds(millis));
    timer_.async_wait(
        [this](const boost::system::error_code &error)
        {
            if (! error)
            {
                on_alarm();

                init_timer(TIMER_INTERVAL_MILLIS);
            }
            else
            {
                client_log(ERROR) << "Timer error ("
                                 << error.value() << "): "
                                 << error.message() << ".";

                timer_.cancel();

                throw std::runtime_error("Timer error");
            }
        });
}

bool client::test_and_setup_joystick(void)
{
    struct {
        int id;
        std::string name;
    } joysticks[] = {
        {GLFW_JOYSTICK_1, "JOYSTICK_1"},
        {GLFW_JOYSTICK_2, "JOYSTICK_2"},
        {GLFW_JOYSTICK_3, "JOYSTICK_3"},
        {GLFW_JOYSTICK_4, "JOYSTICK_4"},
        {GLFW_JOYSTICK_5, "JOYSTICK_5"},
        {GLFW_JOYSTICK_6, "JOYSTICK_6"},
        {GLFW_JOYSTICK_7, "JOYSTICK_7"},
        {GLFW_JOYSTICK_8, "JOYSTICK_8"},
        {GLFW_JOYSTICK_9, "JOYSTICK_9"},
        {GLFW_JOYSTICK_10, "JOYSTICK_10"},
        {GLFW_JOYSTICK_11, "JOYSTICK_11"},
        {GLFW_JOYSTICK_12, "JOYSTICK_12"},
        {GLFW_JOYSTICK_13, "JOYSTICK_13"},
        {GLFW_JOYSTICK_14, "JOYSTICK_14"},
        {GLFW_JOYSTICK_15, "JOYSTICK_15"},
        {GLFW_JOYSTICK_16, "JOYSTICK_16"},
    };

    for (auto &j : joysticks)
    {
        int present = glfwJoystickPresent(j.id);

        if (present)
        {
            console_ << "Joystick " << j.name
                     << " - `" << glfwGetJoystickName(j.id)
                     << "' is present.\n";

            int count;

            //
            // Review axes.
            //

            const float *axes = glfwGetJoystickAxes(j.id, &count);
            console_ << "Num of axes: " << count << "\n";
            console_ << "axes states: [";
            for (int i = 0; i < count; i++)
            {
                console_ << axes[i] << ",";
            }
            console_ << "]\n";

            //
            // Review buttons.
            //

            const unsigned char *buttons = glfwGetJoystickButtons(j.id, &count);
            console_ << "Num of buttons: " << count << "\n";
            console_ << "buttons states: [";
            for (int i = 0; i < count; i++)
            {
                console_ << (buttons[i] == GLFW_PRESS ? "P" : "-") << ",";
            }
            console_ << "]\n";

            joystick_id_ = j.id;

            return true;
        }
    }

    console_ << "E: No joystick detected.\n";

    return false;
}

void client::setup_connection_callbacks(void)
{
    connection_ -> set_on_error_callback([this](const boost::system::error_code &ec)
        {
            on_connection_error(ec);
        });
    
    connection_ -> set_on_connected_callback([this](void)
        {
            on_connected();
        });

    connection_ -> set_on_data_callback([this](const std::vector<char> &data)
        {
            on_device_data(data);
        });
}
