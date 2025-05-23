/*
	The MIT License(MIT)

	Copyright(c) 2015 Vixen Team, Matt Guerrette

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files(the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions :
	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
*/

#include <vix_game.h>
#include <vix_sdlwindow.h>
#include <vix_debugutil.h>
#include <vix_renderer_singleton.h>
#include <vix_scenemanager.h>

namespace Vixen {

	SDLGameWindow::SDLGameWindow(const SDL_GW_Params& params)
	{
		m_params = params;
		m_running = false;
		m_hidden = false;
		m_paused = false;
		m_fullscreen = false;
		m_cursorHidden = false;
        m_captureText = false;
		m_clientRect = Rect(0, 0, 0, 0);
        m_keyboardState = NULL;
        m_mouseState = NULL;
		for (int i = 0; i < 4; i++)
		{
			m_controllerIndeces[i] = -1;
		}
	}

	SDLGameWindow::~SDLGameWindow()
	{

	}

    void* SDLGameWindow::VNativeHandle()
    {
        return m_nativeHandle;
    }

	void SDLGameWindow::VToggleCursor()
	{
		m_cursorHidden = !m_cursorHidden;
		if (m_cursorHidden)
			SDL_HideCursor();
		else
			SDL_ShowCursor();
	}

	void SDLGameWindow::VTogglePaused()
	{
		m_paused = !m_paused;
	}

	void SDLGameWindow::VTrapCursorCenter()
	{
		SDL_WarpMouseInWindow(m_windowHandle, VGetClientBounds().w/2, VGetClientBounds().h/2);
	}

	bool SDLGameWindow::VInit()
	{
		/* Initialize SDL
		*/
		if (!SDL_Init(SDL_INIT_EVENTS | SDL_INIT_GAMEPAD)) {
			DebugPrintF("SDL Failed to Initialize");
            return false;
		}

		/*Create the SDL_Window handle*/

		std::string title = m_params.title;
		m_windowHandle = SDL_CreateWindow(title.c_str(),
											m_params.width,
											m_params.height,
											 SDL_WINDOW_RESIZABLE);
		if (!m_windowHandle) {
			SDL_Quit();
			DebugPrintF("Failed to created SDL_Window handle");
            return false;
		}

#ifdef VIX_SYS_WINDOWS
		m_nativeHandle = static_cast<HWND>(
			SDL_GetPointerProperty(SDL_GetWindowProperties(m_windowHandle), SDL_PROP_WINDOW_WIN32_HWND_POINTER, nullptr));
#endif

#ifdef VIX_SYS_LINUX //for now
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
		/*create OpenGL context*/
		m_context = SDL_GL_CreateContext(m_windowHandle);
		if (!m_context) {
			SDL_Quit();
			DebugPrintF(VTEXT("Failed to create SDL_GL_Context handle"));
            return false;
		}
#endif
        m_mouseState = new SDLMouseState;
        m_keyboardState = new SDLKeyboardState;
		m_controllerState = new SDLControllerState;
        m_running = true;

		return true;
	}

    void SDLGameWindow::VPollEvents()
    {
		/*std::stringstream ss;
		ss << "FPS: " << Time::FPS();
		SDL_SetWindowTitle(m_windowHandle, ss.str().c_str());*/

        SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_EVENT_QUIT:
				VClose();
				break;

			case SDL_EVENT_KEY_DOWN:
			{
				((SDLKeyboardState*)m_keyboardState)->KeyDown(event.key.scancode);

				if (m_captureText && event.key.scancode == SDL_SCANCODE_BACKSPACE)
				{
					if (m_inputBuffer.size() > 0)
						m_inputBuffer.pop_back();
				}

			} break;

			case SDL_EVENT_KEY_UP:
				((SDLKeyboardState*)m_keyboardState)->KeyUp(event.key.scancode);
				break;

			case SDL_EVENT_MOUSE_BUTTON_DOWN:
			case SDL_EVENT_MOUSE_BUTTON_UP:
				((SDLMouseState*)m_mouseState)->MouseEvent(event.button);
				break;

			case SDL_EVENT_MOUSE_WHEEL:
				((SDLMouseState*)m_mouseState)->MouseWheelEvent(event.wheel);
				break;

			case SDL_EVENT_MOUSE_MOTION:
				((SDLMouseState*)m_mouseState)->MouseMove(event.motion.x, event.motion.y);
				break;
			case SDL_EVENT_GAMEPAD_ADDED:
			{
				// TODO: Convert this to SDL3
				/*for (int i = 0; i < 4; i++)
				{
					if (m_controllerIndeces[i] == -1)
					{
						m_controllers[i] = SDL_GameControllerOpen(event.cdevice.which);
						m_controllerIndeces[i] = SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(m_controllers[i]));
						m_controllerState->Connected(true, i);
						break;
					}
				}*/
			}
			break;
			case SDL_EVENT_GAMEPAD_REMOVED:
			{
				// TODO: Convert this to SDL3
				/*int cont = GetPlayerFromControllerIndex(event.cdevice.which);
				SDL_GameControllerClose(m_controllers[cont]);
				m_controllerState->Connected(false, cont);
				m_controllerIndeces[cont] = -1;*/
			}
			break;
			case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
				// TODO: Convert this to SDL3
				//m_controllerState->ButtonDown((SDL_GameControllerButton)event.cbutton.button, GetPlayerFromControllerIndex(event.cdevice.which));
				break;
			case SDL_EVENT_GAMEPAD_BUTTON_UP:
				// TODO: Convert this to SDL3
				//m_controllerState->ButtonUp((SDL_GameControllerButton)event.cbutton.button, GetPlayerFromControllerIndex(event.cdevice.which));
				break;
			case SDL_EVENT_GAMEPAD_AXIS_MOTION:
				// TODO: Convert this to SDL3
				//m_controllerState->Axis((SDL_GameControllerAxis)event.caxis.axis, event.caxis.value, GetPlayerFromControllerIndex(event.cdevice.which));
				break;

			case SDL_EVENT_TEXT_INPUT:
				m_inputBuffer += event.text.text;
				break;


			case SDL_EVENT_WINDOW_RESIZED:
			{
				//HANDLE WINDOW RESIZE
				Renderer::ResizeBuffers(event.window.data1, event.window.data2);

				SceneManager::UpdateCameraViewports(event.window.data1, event.window.data2);

			} break;

			default:
				break;
			}
        }
    }

    void SDLGameWindow::VStartTextCapture()
    {
        m_captureText = true;
        SDL_StartTextInput(m_windowHandle);
    }

    std::string SDLGameWindow::VInputBuffer()
    {
        return m_inputBuffer;
    }

    void SDLGameWindow::VStopTextCapture()
    {
        m_captureText = false;
        SDL_StopTextInput(m_windowHandle);
        m_inputBuffer.clear();
    }

    IKeyboardState* SDLGameWindow::VKeyboardState()
    {
        return m_keyboardState;
    }

    IMouseState* SDLGameWindow::VMouseState()
    {
        return m_mouseState;
    }

	SDLControllerState * SDLGameWindow::VControllerState()
	{
		return m_controllerState;
	}

    void SDLGameWindow::VPollNextFrame()
    {
        /*update keyboard state for next frame*/
        ((SDLKeyboardState*)m_keyboardState)->UpdatePrev();
        /*update mouse state for next frame*/
        ((SDLMouseState*)m_mouseState)->UpdatePrev();
		/*update mouse state for next frame*/
		(m_controllerState)->UpdatePrev();
    }

	void SDLGameWindow::VSetFullscreen(bool flag)
	{
		m_fullscreen = flag;
		if (flag) {
			SDL_SetWindowFullscreen(m_windowHandle, true);
		}
		else {
			SDL_SetWindowFullscreen(m_windowHandle, false);
		}
	}

	void SDLGameWindow::VSetVisible(bool flag)
	{


	}

    bool SDLGameWindow::VRunning()
    {
        return m_running;
    }

	void SDLGameWindow::VSwapBuffers()
	{
		SDL_GL_SwapWindow(m_windowHandle);
	}

    void SDLGameWindow::VSetTitle(std::string title)
    {
        SDL_SetWindowTitle(m_windowHandle, title.c_str());
    }

	const std::string& SDLGameWindow::VGetTitle()
	{
		return m_title;
	}

	const Rect SDLGameWindow::VGetClientBounds()
	{
	    if(m_fullscreen) {
            SDL_GetWindowPosition(m_windowHandle, &m_clientRect.x, &m_clientRect.y);
            SDL_GetWindowSize(m_windowHandle, &m_clientRect.w, &m_clientRect.h);
	    }
		else {
            //we will take the requested size from config
            m_clientRect.w = m_params.width;
            m_clientRect.h = m_params.height;
        }

		return m_clientRect;
	}

	bool SDLGameWindow::VIsPaused()
	{
		return m_paused;
	}

	bool SDLGameWindow::VIsHidden()
	{
		return m_hidden;
	}

	bool SDLGameWindow::VIsRunning()
	{
		return m_running;
	}

	void SDLGameWindow::VClose()
	{
		m_running = false;
		SDL_Quit();
	}

	int SDLGameWindow::GetPlayerFromControllerIndex(int controllerIndex)
	{
		for (int i = 0; i < 4; i++) {
			if (m_controllerIndeces[i] == controllerIndex) return i;
		}
	}

}
