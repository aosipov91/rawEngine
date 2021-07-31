#ifndef GL_BUFFER_H_
#define GL_BUFFER_H_

#include "src/renderer/opengl/openGLExtensions.h"

using GLDeleter = void(*)(GLuint);

template<GLDeleter deleter>
class GLHandle {
public:
    explicit GLHandle(GLuint handle = 0) noexcept;
    GLHandle(GLHandle&& other)  noexcept;
    GLHandle& operator=(GLHandle&& other) noexcept;
    ~GLHandle();

    void reset() noexcept;
    GLuint get() const noexcept;
    explicit operator GLuint() const noexcept;
private:
    GLuint m_handle {0};
};

template<GLDeleter deleter>
GLHandle<deleter>::GLHandle(GLuint handle) noexcept
    : m_handle(handle)
{

}

template<GLDeleter deleter>
GLHandle<deleter>::GLHandle(GLHandle &&other) {
    other.m_handle = 0;
}

template<GLDeleter deleter>
GLHandle &GLHandle<deleter>::operator=(GLHandle &&other) noexcept {
    reset();
    std::swap(m_handle, other.m_handle);
}

template<GLDeleter deleter>
void GLHandle<deleter>::reset() noexcept {
    if (m_handle)
    {
        deleter(m_handle);
        m_handle = 0;
    }
}

template<GLDeleter deleter>
GLuint GLHandle<deleter>::get() const noexcept {
    return m_handle;
}

template<GLDeleter deleter>
GLHandle<deleter>::operator GLuint() const noexcept {
    return 0;
}

template<GLDeleter deleter>
GLHandle<deleter>::~GLHandle() {
    reset();
}


#endif

