#include "App.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "src/core/timer.h"


namespace core {

#ifndef _WIN32

static Display *display;
static int screen;
static XF86VidModeModeInfo **modes;
static Window window;
static GLXContext context;
static Atom WM_DELETE_WINDOW;
static Cursor cursor;
static int done;
static Cursor blankCursor;

typedef GLXContext (*glXCreateContextAttribsARBProc)
	(Display*, GLXFBConfig, GLXContext, Bool, const int*);

/*
 */
Application::Application() {
	memset(this,0,sizeof(Application));
	timer = (struct Timer*)calloc(1, sizeof(struct Timer));
	init_timer(timer);
	display = XOpenDisplay(NULL);
	if(!display) exit("couldn`t open display");

    mouseCaptured = false;
    mouseDelta = glm::vec2();
    mouseSensibility = .3f;
    #if defined(_WIN32)
    invertMouse = false;
    #elif defined(__linux__)
    invertMouse = true;
    #endif
}

Application::~Application() {
	if(context) glXDestroyContext(display,context);
	if(window) XDestroyWindow(display,window);
	if(modes) XF86VidModeSwitchToMode(display,screen,modes[0]);
	if(display) XCloseDisplay(display);
}

/*
 */
static int modescmp(const void *pa,const void *pb) {
	XF86VidModeModeInfo *a = *(XF86VidModeModeInfo**)pa;
	XF86VidModeModeInfo *b = *(XF86VidModeModeInfo**)pb;
	if(a->hdisplay > b->hdisplay) return -1;
	return b->vdisplay - a->vdisplay;
}

/*
 */
bool Application::setVideoMode(int w,int h,bool fs) {
	
	if(window) {
		XDestroyWindow(display,window);
		if(modes) XF86VidModeSwitchToMode(display,screen,modes[0]);
	}
	
	mWindowWidth = w;
	mWindowHeight = h;
	mFullScreen = fs;
	
	try {
		int attrib[] = {
			GLX_RGBA,
			GLX_RED_SIZE,8,
			GLX_GREEN_SIZE,8,
			GLX_BLUE_SIZE,8,
			GLX_ALPHA_SIZE,8,
			GLX_DEPTH_SIZE,24,
			GLX_STENCIL_SIZE,8,
			GLX_DOUBLEBUFFER,
			None
		};
		screen = DefaultScreen(display);
		Window rootWindow = RootWindow(display,screen);
		
		int num_fbc;
		GLXFBConfig *fbc = glXChooseFBConfig(display, DefaultScreen(display), attrib, &num_fbc);
		if (!fbc)
		{
			throw("glXChooseFBConfig() failed");
			return false;
		}

		glXCreateContextAttribsARBProc glXCreateContextAttribsARB = {};
		glXCreateContextAttribsARB =
				(glXCreateContextAttribsARBProc)
				glXGetProcAddress((const GLubyte*)"glXCreateContextAttribsARB");
		if (!glXCreateContextAttribsARB)
		{
			throw("glXCreateContextAttribsARB() not found\n");
			return false;
		}

		static int context_attribs[] = {
			GLX_CONTEXT_MAJOR_VERSION_ARB, 3,	
			GLX_CONTEXT_MINOR_VERSION_ARB, 3,
            GLX_CONTEXT_FLAGS_ARB        , GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
            GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
                None
		};

		// Create modern OpenGL context
		context = glXCreateContextAttribsARB(display, fbc[0], NULL, true, context_attribs);
		if (!context)
		{
			throw("Failed to create OpenGL context. Exiting.\n");
			return false;
		}

		XVisualInfo *visual = glXChooseVisual(display,screen,attrib);
		if(!visual) throw("couldn`t get an RGBA, double-buffered visual");
		
		if(mFullScreen) {
			int i,nmodes;
			XF86VidModeModeLine mode;
			if(XF86VidModeGetModeLine(display,screen,&nmodes,&mode) && XF86VidModeGetAllModeLines(display,screen,&nmodes,&modes)) {
				qsort(modes,nmodes,sizeof(XF86VidModeModeInfo*),modescmp);
				for(i = nmodes - 1; i > 0; i--) if (modes[i]->hdisplay >= mWindowWidth && modes[i]->vdisplay >= mWindowHeight) break;
				if(modes[i]->hdisplay != mode.hdisplay || modes[i]->vdisplay != mode.vdisplay) {
					mWindowWidth = modes[i]->hdisplay;
					mWindowHeight = modes[i]->vdisplay;
					XF86VidModeSwitchToMode(display,screen,modes[i]);
				}
				XF86VidModeSetViewPort(display,screen,0,0);
			} else mFullScreen = 0;
		}
		
		XSetWindowAttributes attr;
		unsigned long mask;
		attr.background_pixel = 0;
		attr.border_pixel = 0;
		attr.colormap = XCreateColormap(display,rootWindow,visual->visual,AllocNone);
		attr.event_mask = StructureNotifyMask | KeyPressMask | KeyReleaseMask | PointerMotionMask | ButtonPressMask | ButtonReleaseMask;
		if(mFullScreen) {
			mask = CWBackPixel | CWColormap | CWOverrideRedirect | CWSaveUnder | CWBackingStore | CWEventMask;
			attr.override_redirect = True;
			attr.backing_store = NotUseful;
			attr.save_under = False;
		} else {
			mask = CWBackPixel | CWBorderPixel | CWColormap | CWEventMask;
		}
		
		window = XCreateWindow(display,rootWindow,0,0,mWindowWidth,mWindowHeight,0,visual->depth,InputOutput,visual->visual,mask,&attr);
		createBlankCursor();

		XMapWindow(display,window);
		
		if(mFullScreen) {
			XMoveWindow(display,window,0,0);
			XRaiseWindow(display,window);
			XWarpPointer(display,None,window,0,0,0,0,mWindowWidth / 2,mWindowHeight / 2);
			XFlush(display);
			XF86VidModeSetViewPort(display,screen,0,0);
			XGrabPointer(display,window,True,0,GrabModeAsync,GrabModeAsync,window,None,CurrentTime);
			XGrabKeyboard(display,window,True,GrabModeAsync,GrabModeAsync,CurrentTime);
		} else {
			WM_DELETE_WINDOW = XInternAtom(display,"WM_DELETE_WINDOW",False);
			XSetWMProtocols(display,window,&WM_DELETE_WINDOW,1);
		}
		
		XFlush(display);
		
		
		glXMakeCurrent(display,window,context);
    init_opengl_extensions();
		glViewport(0,0,mWindowWidth,mWindowHeight);
	}
	catch(const char *error) {
		window = 0;
		exit(error);
		return false;
	}
	return true;
}

bool Application::onMouseMove(const int x, const int y){
    
    if (mouseCaptured)
    {

			static bool changed = false;
			if (changed = !changed){
				mouseDelta.x += (invertMouse? 1 : -1) * mouseSensibility * (mWindowHeight / 2 - y);
				mouseDelta.y += (invertMouse? 1 : -1) * mouseSensibility * (mWindowWidth / 2 - x);
				setCursorPos(mWindowWidth / 2, mWindowHeight / 2);
			}
	
    }
    return false;
}

void Application::setCursorPos(const int x, const int y){
    XWarpPointer(display, None, window, 0, 0, 0, 0, x, y);
    XFlush(display);
}

void Application::captureMouse(const bool value){
	if (mouseCaptured != value)
	{
		static int mouseX, mouseY;
		if (value){
			XGrabPointer(display, window, True, ButtonPressMask, GrabModeAsync, GrabModeAsync, window, blankCursor, CurrentTime);

			int rx, ry;
			unsigned int mask;
			Window root, child;
			XQueryPointer(display, window, &root, &child, &rx, &ry, &mouseX, &mouseY, &mask);
			setCursorPos(mWindowWidth / 2, mWindowHeight / 2);
		} else {
			setCursorPos(mouseX, mouseY);
			XUngrabPointer(display, CurrentTime);
		}

		mouseCaptured = value;
	}
}


bool Application::onMouseButton(const int x, const int y, const MouseButton button, const bool pressed){
  if (!mouseCaptured){

		if (button == MOUSE_LEFT && pressed){
				captureMouse(true);
				return true;
		}

  }
  return false;
}

void Application::createBlankCursor()
{
    XColor dummy;
    char data = 0;
    Pixmap blank = XCreateBitmapFromData(display, window, &data, 1, 1);
    blankCursor = XCreatePixmapCursor(display, blank, blank, &dummy, &dummy, 0, 0);
    XFreePixmap(display, blank);
    XGrabKeyboard(display, window, True, GrabModeAsync, GrabModeAsync, CurrentTime);
}

/*
 */
void Application::setTitle(const char *title) {
	strcpy(this->mTitle,title);
	XStoreName(display,window,mTitle);
	XSetIconName(display,window,mTitle);
}

/*
 */
void Application::setCursor(int x,int y) {
	XWarpPointer(display,None,window,0,0,0,0,x,y);
	XFlush(display);
}

/*
 */
void Application::showCursor(int show) {
	if(show) XDefineCursor(display,window,0);
	else {
		if(!cursor) {
			char data[] = { 0 };
			Pixmap blank;
			XColor dummy;
			blank = XCreateBitmapFromData(display,window,data,1,1);
			cursor = XCreatePixmapCursor(display,blank,blank,&dummy,&dummy,0,0);
			XFreePixmap(display,blank);
		}
		XDefineCursor(display,window,cursor);
	}
}

/*
 */
void Application::checkExtension(const char *extension) {
	static char *extensions = NULL;
	if(!extensions) extensions = (char*)glGetString(GL_EXTENSIONS);
	if(strstr(extensions,extension)) return;
	char error[1024];
	sprintf(error,"OpenGL extension \"%s\" is not supported by current hardware",extension);
	exit(error);
}

/*
 */
void Application::error() {
	GLenum error;
	while((error = glGetError()) != GL_NO_ERROR) {
		fprintf(stderr,"OpenGL error 0x%04X: %s\n",error,gluErrorString(error));
	}
}

/*
 */
void Application::exit(const char *error,...) {
	if(error) {
		char buf[1024];
		va_list arg;
		va_start(arg,error);
		vsprintf(buf,error,arg);
		va_end(arg);
		fprintf(stderr,"Application exit: %s\n",buf);
	}
	done = 1;
}


/*
 */
static int getTime() {
	struct timeval tval;
	struct timezone tzone;
	gettimeofday(&tval,&tzone);
	return tval.tv_sec * 1000 + tval.tv_usec / 1000;
}

/*
 */

static int translateKey(int key) {
	int ret;
	if(key & 0xff00) {
		switch(key) {
			case XK_Escape: ret = Application::KEY_ESC; break;
			case XK_Tab: ret = Application::KEY_TAB; break;
			case XK_Return: ret = Application::KEY_RETURN; break;
			case XK_BackSpace: ret = Application::KEY_BACKSPACE; break;
			case XK_Delete: ret = Application::KEY_DELETE; break;
			case XK_Home: ret = Application::KEY_HOME; break;
			case XK_End: ret = Application::KEY_END; break;
			case XK_Page_Up: ret = Application::KEY_PGUP; break;
			case XK_Page_Down: ret = Application::KEY_PGDOWN; break;
			case XK_Left: ret = Application::KEY_LEFT; break;
			case XK_Right: ret = Application::KEY_RIGHT; break;
			case XK_Up: ret = Application::KEY_UP; break;
			case XK_Down: ret = Application::KEY_DOWN; break;
			case XK_Shift_L: case XK_Shift_R: ret = Application::KEY_SHIFT; break;
			case XK_Control_L: case XK_Control_R: ret = Application::KEY_CTRL; break;
			case XK_Alt_L: case XK_Alt_R: ret = Application::KEY_ALT; break;
			default: ret = (key < 0x06ff) ? (key & 0x00ff) : 0;
		}
	} else {
		ret = key;
	}
	return ret;
}

/*
 */
void Application::main() {
	KeySym key;
	int startTime = getTime(),endTime = 0,counter = 0;
    mFps = 60;
	while(window && !done) {
		while(XPending(display) > 0) {
			XEvent event;
			XNextEvent(display,&event);
			switch(event.type) {
				case ClientMessage:
					if(event.xclient.format == 32 && event.xclient.data.l[0] == (long)WM_DELETE_WINDOW) exit();
					break;
				case ConfigureNotify:
					mWindowWidth = event.xconfigure.width;
					mWindowHeight = event.xconfigure.height;
					glViewport(0,0,mWindowWidth,mWindowHeight);
					break;
				case KeyPress:
					XLookupString(&event.xkey,NULL,0,&key,NULL);
					key = translateKey(key);
					keys[key] = 1;
					if(keys[KEY_ALT] && keys[KEY_RETURN]) {
						keys[KEY_ALT] = 0;
						keys[KEY_RETURN] = 0;
						setVideoMode(mWindowWidth,mWindowHeight,!mFullScreen);
						setTitle(mTitle);
					}
					if (keys[KEY_ESC])
					{
						exit(EXIT_SUCCESS);
					}
					if(key) keyPress(key);
					break;
				case KeyRelease:
					XLookupString(&event.xkey,NULL,0,&key,NULL);
					key = translateKey(key);
					keys[key] = 0;
					if(key) keyRelease(key);
					break;
				case MotionNotify:
					static int lastX, lastY;
					onMouseMove(event.xmotion.x, event.xmotion.y);
					lastX = event.xmotion.x;
					lastY = event.xmotion.y;

					mMouseX = event.xmotion.x;
					mMouseY = event.xmotion.y;
					break;
				case ButtonPress:
					mMouseButton |= 1 << ((event.xbutton.button - 1));
					buttonPress(1 << (event.xbutton.button - 1));
					
					onMouseButton(event.xbutton.x, event.xbutton.y, (MouseButton) (event.xbutton.button - 1), true);
					if ((event.xbutton.button -1 ) == MOUSE_RIGHT){
						mouseStates[MOUSE_RIGHT] = true;
					}

					if ((event.xbutton.button -1 ) == MOUSE_LEFT){
						mouseStates[MOUSE_LEFT] = true;
					}

					break;
				case ButtonRelease:
					if(event.xbutton.button < 4) mMouseButton &= ~(1 << (event.xbutton.button - 1));
					buttonRelease(1 << (event.xbutton.button - 1));

					onMouseButton(event.xbutton.x, event.xbutton.y, (MouseButton) (event.xbutton.button - 1), false);
					if ((event.xbutton.button -1 ) == MOUSE_RIGHT){
						mouseStates[MOUSE_RIGHT] = false;
					}

					if ((event.xbutton.button -1 ) == MOUSE_LEFT){
						mouseStates[MOUSE_LEFT] = false;
					}

					break;
			}
		}
		
		if(counter++ == 10) {
			endTime = startTime;
			startTime = getTime(); 
			mFps = counter * 1000.0 / (float)(startTime - endTime);
			counter = 0;
		}
		mInvertFps = 1.0 / mFps;
		
		idle();
		render();
		
		glXSwapBuffers(display,window);
		mMouseButton &= ~(BUTTON_UP | BUTTON_DOWN);
	}
}



#else

//extern int main(int argc,char **argv);

static Application *glApp;
static HWND window;
static HGLRC context;
static HCURSOR cursor;
static int done;

Application::Application() {
	memset(this,0,sizeof(Application));
	timer = (struct Timer*)calloc(1, sizeof(struct Timer));
	init_timer(timer);

}

Application::~Application() {
        if (timer)
        {
            free(timer);
        }
	if(window) {
		HDC hdc = GetDC(window);
		wglMakeCurrent(hdc,0);
		wglDeleteContext(context);
		ReleaseDC(window,hdc);
		DestroyWindow(window);
		if(mFullScreen) {
			ChangeDisplaySettings(NULL,0);
			ShowCursor(true);
		}
	}
}

/*
 */
static int translateKey(int key) {
	int ret;
	switch(key) {
		case VK_ESCAPE : ret = Application::KEY_ESC; break;
		case VK_TAB: ret = Application::KEY_TAB; break;
		case VK_RETURN: ret = Application::KEY_RETURN; break;
		case VK_BACK: ret = Application::KEY_BACKSPACE; break;
		case VK_DELETE: ret = Application::KEY_DELETE; break;
		case VK_HOME: ret = Application::KEY_HOME; break;
		case VK_END: ret = Application::KEY_END; break;
		case VK_PRIOR: ret = Application::KEY_PGUP; break;
		case VK_NEXT: ret = Application::KEY_PGDOWN; break;
		case VK_LEFT: ret = Application::KEY_LEFT; break;
		case VK_RIGHT: ret = Application::KEY_RIGHT; break;
		case VK_UP: ret = Application::KEY_UP; break;
		case VK_DOWN: ret = Application::KEY_DOWN; break;
		case VK_SHIFT: ret = Application::KEY_SHIFT; break;
		case VK_CONTROL: ret = Application::KEY_CTRL; break;
		default:
			ret = MapVirtualKey(key,2);
			if(strchr("1234567890-=",ret)) {
				if(glApp->keys[Application::KEY_SHIFT]) {
					if(ret == '1') ret = '!';
					else if(ret == '2') ret = '@';
					else if(ret == '3') ret = '#';
					else if(ret == '4') ret = '$';
					else if(ret == '5') ret = '%';
					else if(ret == '6') ret = '^';
					else if(ret == '7') ret = '&';
					else if(ret == '8') ret = '*';
					else if(ret == '9') ret = '(';
					else if(ret == '0') ret = ')';
					else if(ret == '-') ret = '_';
					else if(ret == '=') ret = '+';
				}
			} else if(isascii(ret)) {
				if(glApp->keys[Application::KEY_SHIFT]) ret = toupper(ret);
				else ret = tolower(ret);
			}
			else ret = 0;
	}
	return ret;
}

/*
 */
LRESULT CALLBACK windowProc(HWND window,UINT message,WPARAM wparam,LPARAM lparam) {
	switch(message) {
		case WM_SIZE:
			glApp->mWindowWidth = LOWORD(lparam);
			glApp->mWindowHeight = HIWORD(lparam);
			glViewport(0,0,glApp->mWindowWidth,glApp->mWindowHeight);
			break;
                        // TODO: It's trash. Need to fix that
		case WM_KEYDOWN: {
				//int key = translateKey((int)wparam);
                                glApp->keys[wparam] = message != WM_KEYUP;
				//glApp->keys[key] = 1;
				//glApp->keyPress(key);
			}
			break;
		case WM_KEYUP: {
				//int key = translateKey((int)wparam);
                                glApp->keys[wparam] = message != WM_KEYUP;
				//glApp->keys[key] = 0;
				//glApp->keyRelease(key);
			}
			break;
		case WM_CLOSE:
			glApp->exit();
			break;
		case WM_LBUTTONDOWN:
			glApp->mMouseButton |= Application::BUTTON_LEFT;
			glApp->buttonPress(Application::BUTTON_LEFT);
			break;
		case WM_LBUTTONUP:
			glApp->mMouseButton &= ~Application::BUTTON_LEFT;
			glApp->buttonRelease(Application::BUTTON_LEFT);
			break;
		case WM_MBUTTONDOWN:
			glApp->mMouseButton |= Application::BUTTON_MIDDLE;
			glApp->buttonPress(Application::BUTTON_MIDDLE);
			break;
		case WM_MBUTTONUP:
			glApp->mMouseButton &= ~Application::BUTTON_MIDDLE;
			glApp->buttonRelease(Application::BUTTON_MIDDLE);
			break;
		case WM_RBUTTONDOWN:
			glApp->mMouseButton |= Application::BUTTON_RIGHT;
			glApp->buttonPress(Application::BUTTON_RIGHT);
			break;
		case WM_RBUTTONUP:
			glApp->mMouseButton &= ~Application::BUTTON_RIGHT;
			glApp->buttonRelease(Application::BUTTON_RIGHT);
			break;
		case 0x020A: //WM_MOUSEWHEEL:
			if((short)HIWORD(wparam) > 0) {
				glApp->mMouseButton |= Application::BUTTON_UP;
				glApp->buttonPress(Application::BUTTON_UP);
				glApp->buttonRelease(Application::BUTTON_UP);
			} else {
				glApp->mMouseButton |= Application::BUTTON_DOWN;
				glApp->buttonPress(Application::BUTTON_DOWN);
				glApp->buttonRelease(Application::BUTTON_DOWN);
			}
			break;
		case WM_MOUSEMOVE:
			glApp->mMouseX = LOWORD(lparam);
			glApp->mMouseY = HIWORD(lparam);
			if(glApp->mMouseX & 1 << 15) glApp->mMouseX -= (1 << 16);
			if(glApp->mMouseY & 1 << 15) glApp->mMouseY -= (1 << 16);
			break;
	}
	return DefWindowProc(window,message,wparam,lparam);
}

/*
 */
bool Application::setVideoMode(int w,int h,bool fs) {
	
	if(window) {
		DestroyWindow(window);
		if(mFullScreen) {
			ChangeDisplaySettings(NULL,0);
			ShowCursor(true);
		}
	}
	
	mWindowWidth = w;
	mWindowHeight = h;
	mFullScreen = fs;
	glApp = this;
	
	try {
		HDC hdc;
		int pixelformat;
		PIXELFORMATDESCRIPTOR pfd = {
			sizeof(PIXELFORMATDESCRIPTOR), 1, PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
			PFD_TYPE_RGBA, 32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 32, 8, 0, PFD_MAIN_PLANE, 0, 0, 0, 0
		};
		
		HINSTANCE hInstance = GetModuleHandle(NULL);
		if(!window) {
			WNDCLASS wc;
			wc.style = 0;
			wc.lpfnWndProc = (WNDPROC)windowProc;
			wc.cbClsExtra = 0;
			wc.cbWndExtra = 0;
			wc.hInstance = hInstance;
			wc.hIcon = 0;
			wc.hCursor = LoadCursor(NULL,IDC_ARROW);
			wc.hbrBackground = NULL;
			wc.lpszMenuName = NULL;
			wc.lpszClassName = "Application";
			if(!RegisterClass(&wc)) throw("RegisterClass() fail");
		}

		if(mFullScreen) {
			DEVMODE settings;
			memset(&settings,0,sizeof(DEVMODE));
			settings.dmSize = sizeof(DEVMODE);
			settings.dmPelsWidth = w;
			settings.dmPelsHeight = h;
			settings.dmBitsPerPel = 32;
			settings.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;
			if(ChangeDisplaySettings(&settings,CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) throw("ChangeDisplaySettings() fail");
			window = CreateWindowEx(0,"Application","GLApp",WS_POPUP,0,0,mWindowWidth,mWindowHeight,NULL,NULL,hInstance,NULL);
		} else {
			RECT windowRect = {0, 0, mWindowWidth, mWindowHeight };
			AdjustWindowRectEx(&windowRect,WS_OVERLAPPEDWINDOW,0,0);
			mWindowWidth = windowRect.right - windowRect.left;
			mWindowHeight = windowRect.bottom - windowRect.top;
			window = CreateWindowEx(0,"Application","GLApp",WS_OVERLAPPEDWINDOW,CW_USEDEFAULT,CW_USEDEFAULT,mWindowWidth,mWindowHeight,NULL,NULL,hInstance,NULL);
        }
		
		if(!window) throw("CreateWindowEx() fail");
		if(!(hdc = GetDC(window))) throw("GetDC() fail");
		
		if(!(pixelformat = ChoosePixelFormat(hdc,&pfd))) throw("ChoosePixelFormat() fail");
		if(!(SetPixelFormat(hdc,pixelformat,&pfd))) throw("SetPixelFormat() fail");

    HGLRC tempRC = wglCreateContext(hdc);
    wglMakeCurrent(hdc, tempRC);
    PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = NULL;
    wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");

    const int attribList[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
        WGL_CONTEXT_MINOR_VERSION_ARB, 3,
        WGL_CONTEXT_FLAGS_ARB, 0,
        WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        0,
    };

    context = wglCreateContextAttribsARB(hdc, 0, attribList);

    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(tempRC);
    wglMakeCurrent(hdc, context);

        init_opengl_extensions();

		glViewport(0,0,glApp->mWindowWidth,glApp->mWindowHeight);
	}
	catch(const char *error) {
		window = 0;
		exit(error);
		return 0;
	}
	ShowWindow(window,SW_SHOW);
	UpdateWindow(window);
	SetForegroundWindow(window);
	SetFocus(window);
	return true;
}

/*
 */
void Application::setTitle(const char *title) {
	SetWindowText(window,title);
}

/*
 */
void Application::setCursor(int x,int y) {
	POINT pt;
	pt.x = x;
	pt.y = y;
	ClientToScreen(window,&pt);
	SetCursorPos(pt.x,pt.y);
}

/*
 */
void Application::showCursor(int show) {
	if(!cursor) cursor = GetCursor();
	if(show) SetCursor(cursor);
	else SetCursor(NULL);
}

/*
 */
void Application::checkExtension(const char *extension) {
	static char *extensions = NULL;
	if(!extensions) extensions = (char*)glGetString(GL_EXTENSIONS);
	if(strstr(extensions,extension)) return;
	char error[1024];
	sprintf(error,"OpenGL extension \"%s\" is not supported by current hardware",extension);
	exit(error);
}

/*
 */
void Application::error() {
/*
	GLenum error;
	while((error = glGetError()) != GL_NO_ERROR) {
		MessageBox(0,(char*)gluErrorString(error),"OpenGL error",MB_OK);
	}
*/
}

/*
 */
void Application::exit(const char *error,...) {
	if(error) {
		char buf[1024];
		va_list arg;
		va_start(arg,error);
		vsprintf(buf,error,arg);
		va_end(arg);
		MessageBox(0,buf,"Application exit",MB_OK);
	}
	done = 1;
}

/*
int Application::selectFile(const char *title,char *name) {
	OPENFILENAME ofn;
	memset(&ofn,0,sizeof(ofn));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = window;
	ofn.lpstrFileTitle = name;
	ofn.nMaxFileTitle = 512;
	ofn.lpstrFilter = "*.*";
	ofn.nFilterIndex = 1;
	ofn.lpstrTitle = title;
	ofn.Flags = OFN_FILEMUSTEXIST;
	return GetOpenFileName(&ofn);
}
*/

/*

static int getTime() {
	static int base;
	static int initialized = 0;
	if(!initialized) {
		base = timeGetTime();
		initialized = 1;
	}
	return timeGetTime() - base;
}
*/

/*
 */
void Application::main() {
    int startTime = 0,endTime = 0,counter = 0;
    mFps = 60;
    MSG msg;
    while(window && !done) {
        if(PeekMessage(&msg,NULL,0,0,PM_NOREMOVE))
        {
            if(!GetMessage(&msg,NULL,0,0))
            { 
                return;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        } else {
            if(counter++ == 10)
            {
                endTime = startTime;
                startTime = get_running_time(timer);
                mFps = counter * 1000.0f / (float)(startTime - endTime);
                counter = 0;
            }
            mInvertFps = 1.0f / mFps;
            
            idle();
            render();
            
            SwapBuffers(wglGetCurrentDC());
            mMouseButton &= ~(BUTTON_UP | BUTTON_DOWN);
        }
    }
}


#endif

}
