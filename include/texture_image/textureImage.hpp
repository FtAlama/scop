#ifndef TEXTUREIMAGE_HPP

#define TEXTUREIMAGE_HPP

#include "vk_context.hpp"
#include <cstdint>
#include <string>

struct textCoord {
	uint32_t width;
	uint32_t height;
};

class TextureImage {
public:
	TextureImage(vk_context &, std::string const);
	const VkImage &get() const { return textureImage; }
	~TextureImage();
private:
	vk_context &ctx;
	VkImage textureImage;
	VkDeviceMemory textureImageMemory;

	class NestedImage {
	public:
		NestedImage();
		~NestedImage();
	};
};


#endif
