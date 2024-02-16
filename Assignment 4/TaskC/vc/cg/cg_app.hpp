#pragma once

namespace cg
{
	class CgApp
	{

	public:

		// should contain all rendering related code, as well as scene updates
		virtual void render() = 0;

		// you will need to handle resize events, since the window aspect ratio changes
		virtual void resize(const unsigned w, const unsigned h) = 0;

		// glfw callbacks -- override if needed
		virtual void mouseButton(const int button, const int action, const int mods) {}
		virtual void mousePosition(const double xpos, const double ypos) {}
		virtual void keyInput(const int key, const int scancode, const int action, const int mods) {};
		virtual void mouseScroll(const double xoffset, const double yoffset) {};

		CgApp(CgApp const&) = delete;
		CgApp& operator=(CgApp const&) = delete;
		CgApp(CgApp&&) = delete;
		CgApp& operator=(CgApp&&) = delete;

	protected:

		CgApp() = default;
		virtual ~CgApp() = default;
	};
}