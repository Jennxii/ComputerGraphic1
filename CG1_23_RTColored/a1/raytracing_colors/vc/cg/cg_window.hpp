#pragma once

#include "ogl.hpp"
#include "debug_output.hpp"
#include "cg_app.hpp"

//#include "..\imgui\imgui.h"
//#include "..\imgui\imgui_impl_glfw.h"
//#include "..\imgui\imgui_impl_opengl3.h"

#include <GLFW\glfw3.h>

#include <iostream>

namespace cg
{
	class CgWindow
	{

	public:

		CgWindow() = default;
		virtual ~CgWindow() = default;

		static void windowResizeCallback(GLFWwindow *window, int w, int h)
		{
			if (w <= 0 || h <= 0)
				return;

			CgApp *app = static_cast<CgApp *>(glfwGetWindowUserPointer(window));
			app->resize(w, h);
		}

		static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
		{
			CgApp *app = static_cast<CgApp *>(glfwGetWindowUserPointer(window));
			app->mouseButton(button, action, mods);
		}

		static void mousePositionCallback(GLFWwindow* window, double xpos, double ypos)
		{
			CgApp *app = static_cast<CgApp *>(glfwGetWindowUserPointer(window));
			app->mousePosition(xpos, ypos);
		}

		static void keyInputCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			CgApp *app = static_cast<CgApp *>(glfwGetWindowUserPointer(window));
			app->keyInput(key, scancode, action, mods);
		}

		static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
		{
			CgApp *app = static_cast<CgApp *>(glfwGetWindowUserPointer(window));
			app->mouseScroll(xoffset, yoffset);
		}

		template <typename CgAppDerived>
		static std::shared_ptr<CgWindow> create(
			const std::string title,
			const bool resizable,
			const unsigned width,
			const unsigned height,
			const unsigned posX,
			const unsigned posY,
			const unsigned ogl_version_major,
			const unsigned ogl_version_minor,
			const unsigned samples)
		{
			if (width == 0 || height == 0)
			{
				std::cout << "invalid window dimensions" << std::endl;
				return nullptr;
			}

			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, ogl_version_major);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, ogl_version_minor);
			glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			glfwWindowHint(GLFW_SAMPLES, samples);
			glfwWindowHint(GLFW_RESIZABLE, resizable);

			GLFWwindow *window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
			if (!window)
			{
				std::cout << "failed to create GLFW window" << std::endl;
				return nullptr;
			}
			glfwMakeContextCurrent(window);
			glewInit();

			int w = 0; int h = 0;
			glfwGetWindowSize(window, &w, &h);
			if (w <= 0 || h <= 0)
			{
				std::cout << "sth. went wrong, glfw window has invalid dimensions " << w << " " << h << std::endl;
				glfwDestroyWindow(window);
				return nullptr;
			}

			//IMGUI_CHECKVERSION();
			//ImGui::CreateContext();
			//ImGuiIO& io = ImGui::GetIO(); (void)io;
			//ImGui::StyleColorsDark();
			//const char* glsl_version = "#version 330";
			//ImGui_ImplGlfw_InitForOpenGL(window, true);
			//ImGui_ImplOpenGL3_Init(glsl_version);

			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			glDebugMessageCallback(cg::OglDebugOutput::debugCallback, NULL);
			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);

			std::shared_ptr<CgApp> app = CgAppDerived::create(w, h);
			if (!app)
			{
				std::cout << "sth. went wrong, could not create app" << std::endl;
				glfwDestroyWindow(window);
				return nullptr;
			}

			glfwSetWindowPos(window, posX, posY);
			glfwSetWindowUserPointer(window, app.get());
			glfwSetFramebufferSizeCallback(window, CgWindow::windowResizeCallback);
			glfwSetMouseButtonCallback(window, CgWindow::mouseButtonCallback);
			glfwSetCursorPosCallback(window, CgWindow::mousePositionCallback);
			glfwSetKeyCallback(window, CgWindow::keyInputCallback);
			glfwSetScrollCallback(window, CgWindow::scrollCallback);

			std::shared_ptr<CgWindow> result(new CgWindow(), CgWindow::Deleter());
			result->mCgApp = app;
			result->mGlfwWindow = window;

			return result;
		}

		void onNewFrame() const
		{
			glfwMakeContextCurrent(mGlfwWindow);
			mCgApp->render();
			glfwSwapBuffers(mGlfwWindow);
		}

		bool shouldClose() const
		{
			return glfwWindowShouldClose(mGlfwWindow);
		}

		CgWindow(CgWindow const&) = delete;
		CgWindow& operator=(CgWindow const&) = delete;
		CgWindow(CgWindow &&) = delete;
		CgWindow& operator=(CgWindow &&) = delete;

	private:

		struct Deleter
		{
			void operator()(CgWindow *& p)
			{
				// cleanup needs current context
				glfwMakeContextCurrent(p->mGlfwWindow);

				glfwSetWindowUserPointer(p->mGlfwWindow, nullptr);

				// destroy cg app
				p->mCgApp = nullptr;

				// destroy glfw window
				glfwDestroyWindow(p->mGlfwWindow);
				p->mGlfwWindow = nullptr;

				// finally, destroy the object itself
				delete p;
				p = nullptr;
			}
		};

		GLFWwindow *mGlfwWindow = nullptr;
		std::shared_ptr<CgApp> mCgApp = nullptr;

	};
}