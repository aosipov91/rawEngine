#ifndef APP_H
#define APP_H

#include "platform.h"
#include "src/renderer/opengl/openGLExtensions.h"
#include <glm/vec2.hpp>


namespace core {

class Application
{
public:
    Application();
    virtual ~Application();

    bool setVideoMode(int width, int height, bool fullScreen = true);
    void setTitle(const char *title);

    void setCursor(const int x, const int y);
    void showCursor(int show);
		void createBlankCursor();

		enum MouseButton {
			MOUSE_LEFT   = 0,
			MOUSE_MIDDLE = 1,
			MOUSE_RIGHT  = 2,

			NUM_MOUSE_BUTTONS
		};

		void setCursorPos(int x, int y);
		bool onMouseButton(const int x, const int y, const MouseButton button, const bool pressed);
		void captureMouse(const bool value);
		bool onMouseMove(const int x, const int y);

    void checkExtension(const char *extension);

    void error();
    static void exit(const char* error = NULL,...);


    void main();

    virtual void idle() {}
    virtual void render() {}
    virtual void buttonPress(int) {}
    virtual void buttonRelease(int) {}
    virtual void keyPress(int) {}
    virtual void keyRelease(int) {}

		bool isKeyPressed ( int keyCode ) const
		{
			return (keyCode >= 0 && keyCode < sizeof ( keys )) ? (keys [keyCode] != 0) : false;
		}

    enum {
        KEY_ESC = 256,
        KEY_TAB,
        KEY_RETURN,
        KEY_BACKSPACE,
        KEY_DELETE,
        KEY_HOME,
        KEY_END,
        KEY_PGUP,
        KEY_PGDOWN,
        KEY_LEFT,
        KEY_RIGHT,
        KEY_UP,
        KEY_DOWN,
        KEY_SHIFT,
        KEY_CTRL,
        KEY_ALT
    };

    enum {
        BUTTON_LEFT = 1 << 0,
        BUTTON_MIDDLE = 1 << 1,
        BUTTON_RIGHT = 1 << 2,
        BUTTON_UP = 1 << 3,
        BUTTON_DOWN = 1 << 4
    };


    bool mouseStates[NUM_MOUSE_BUTTONS];
		bool mouseCaptured;
		glm::vec2 mouseDelta;
    bool invertMouse;
    float mouseSensibility;
		

    int keys[512];

    int mWindowWidth;
    int mWindowHeight;
    bool mFullScreen;
    char mTitle[1024];

    int mMouseX;
    int mMouseY;
    int mMouseButton;

    float mFps;
    float mInvertFps;
};

}


#endif
