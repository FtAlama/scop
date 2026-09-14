#ifndef RENDERPASS_HPP

#define RENDERPASS_HPP

#include "vk_context.hpp"

class RenderPass {
public:
	RenderPass(vk_context &);
	~RenderPass();
private:
	vk_context &ctx;

};

#endif
