#ifndef QUEUE_FAMILIES_HPP

#define QUEUE_FAMILIES_HPP

#include <cstdint>
#include <optional>

typedef struct s_queueFamily {
	std::optional<uint32_t> graphicsFamily;
} QueueFamilyIndices;

#endif
