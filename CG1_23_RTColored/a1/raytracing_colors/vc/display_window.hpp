#pragma once

#include "cg/ogl.hpp"
#include "cg/cg_app.hpp"

#include <memory>
#include <chrono>

struct Framebuffer;

class DisplayWindow : public cg::CgApp
{

public:

	static std::shared_ptr<DisplayWindow> create(const unsigned w, const unsigned h);

	void render();
	void resize(unsigned w, unsigned h);

private:

	DisplayWindow() = delete;
	~DisplayWindow() = default;
	DisplayWindow(const unsigned w, const unsigned h);

	void display(Framebuffer const& framebuffer);

	struct Deleter
	{
		void operator()(DisplayWindow*& obj) const;
	};

	friend struct Deleter;

	unsigned mWindowWidth;
	unsigned mWindowHeight;
	unsigned mDisplayTextureWidth = 0;
	unsigned mDisplayTextureHeight = 0;

	GLuint mDisplayTexture = 0;
	GLuint mDisplayShader = 0;
	GLuint mDummyMesh = 0;
};
