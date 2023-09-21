from __future__ import annotations

import logging
from typing import TYPE_CHECKING, Generator, Tuple, List

from medusa.vulkan import vulkan as vk
from medusa.vulkan import extensions as vkx

from medusa.vulkan.synchronisation import Semaphore, Fence
from medusa.vulkan.surface import SurfaceInfo

from medusa.vulkan.vulkan import VulkanHandle

if TYPE_CHECKING:
    from medusa.vulkan.context import Context
    from medusa.vulkan.command_buffer import CommandBuffer


logger = logging.getLogger(__name__)


class ColourAttachment(object):
    def __init__(self, context: Context, surface_format: int, image_handle: VulkanHandle):

        self.__view: VulkanHandle = None
        self.__image: VulkanHandle = image_handle

        self.__context: Context = context

        subresource_range = vk.VkImageSubresourceRange(
            aspectMask=vk.VK_IMAGE_ASPECT_COLOR_BIT,
            baseMipLevel=0,
            levelCount=1,
            baseArrayLayer=0,
            layerCount=1,
        )

        components = vk.VkComponentMapping(
            r=vk.VK_COMPONENT_SWIZZLE_IDENTITY,
            g=vk.VK_COMPONENT_SWIZZLE_IDENTITY,
            b=vk.VK_COMPONENT_SWIZZLE_IDENTITY,
            a=vk.VK_COMPONENT_SWIZZLE_IDENTITY,
        )

        view_info = vk.VkImageViewCreateInfo(
            image=image_handle,
            viewType=vk.VK_IMAGE_VIEW_TYPE_2D,
            format=surface_format,
            components=components,
            subresourceRange=subresource_range,
        )

        self.__view = vk.vkCreateImageView(context.logical_device.handle, view_info, None)
        logger.info("Vulkan Image View: Created")

    def __del__(self):
        if self.__view:
            vk.vkDestroyImageView(self.__context.logical_device.handle, self.__view, None)
            logger.info("Vulkan Image View: Destroyed")

        self.__view = None

    @property
    def handle(self) -> VulkanHandle:
        return self.__view


class DepthAttachment(object):
    def __init__(self, context: Context, surface_info: SurfaceInfo):
        self.__image: VulkanHandle = None
        self.__image_view: VulkanHandle = None
        self.__image_memory: VulkanHandle = None

        self.__context: Context = context

        extent = vk.VkExtent3D(surface_info.width, surface_info.height, 1)

        # Create Image
        image_info = vk.VkImageCreateInfo(
            imageType=vk.VK_IMAGE_TYPE_2D,
            extent=extent,
            mipLevels=1,
            arrayLayers=1,
            format=surface_info.depth_format,
            tiling=vk.VK_IMAGE_TILING_OPTIMAL,
            initialLayout=vk.VK_IMAGE_LAYOUT_UNDEFINED,
            usage=vk.VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
            samples=vk.VK_SAMPLE_COUNT_1_BIT,
            sharingMode=vk.VK_SHARING_MODE_EXCLUSIVE,
        )

        self.__image = vk.vkCreateImage(self.__context.logical_device.handle, image_info, None)

        # Allocate Memory for Image
        memory_requirements = vk.vkGetImageMemoryRequirements(self.__context.logical_device.handle, self.__image)

        alloc_info = vk.VkMemoryAllocateInfo(
            allocationSize=memory_requirements.size,
            memoryTypeIndex=self.__context.find_memory_type(memory_requirements.memoryTypeBits, vk.VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT),
        )

        self.__image_memory = vk.vkAllocateMemory(self.__context.logical_device.handle, alloc_info, None)

        # Bind Memory to Image
        vk.vkBindImageMemory(self.__context.logical_device.handle, self.__image, self.__image_memory, 0)

        # Create View
        subresource_range = vk.VkImageSubresourceRange(
            aspectMask=vk.VK_IMAGE_ASPECT_DEPTH_BIT,
            baseMipLevel=0,
            levelCount=1,
            baseArrayLayer=0,
            layerCount=1,
        )

        view_info = vk.VkImageViewCreateInfo(
            image=self.__image,
            viewType=vk.VK_IMAGE_VIEW_TYPE_2D,
            format=surface_info.depth_format,
            subresourceRange=subresource_range,
        )

        self.__image_view = vk.vkCreateImageView(self.__context.logical_device.handle, view_info, None)
        logger.info("Vulkan Depth Image: Created")

    def __del__(self):

        if self.__image_view:
            vk.vkDestroyImageView(self.__context.logical_device.handle, self.__image_view, None)

        if self.__image_memory:
            vk.vkFreeMemory(self.__context.logical_device.handle, self.__image_memory, None)

        if self.__image:
            vk.vkDestroyImage(self.__context.logical_device.handle, self.__image, None)

        logger.info("Vulkan Depth Image: Destroyed")

        self.__image_view = None
        self.__image_memory = None
        self.__image = None

    @property
    def handle(self) -> VulkanHandle:
        return self.__image_view


class Frame(object):
    # TODO: Suspect this is the root of how offscreen rendering works... this could/should be abstraction :)

    """Combine the Attachments""" ""

    def __init__(self, context: Context, surface_info: SurfaceInfo, image_index: int, image_handle: VulkanHandle):

        # extent = surface_info.extent

        self.__surface_info = surface_info
        self.__colour_attachment: ColourAttachment = ColourAttachment(context, surface_info.surface_format, image_handle)
        self.__depth_attachment: DepthAttachment = DepthAttachment(context, surface_info)
        self.__image_index: int = image_index
        self.__fence: Fence = None

    def get_handles(self) -> Tuple[VulkanHandle, ...]:
        return self.__colour_attachment.handle, self.__depth_attachment.handle

    def surface_info(self) -> SurfaceInfo:
        return self.__surface_info

    @property
    def image_index(self) -> int:
        return self.__image_index

    def use_fence(self, fence: Fence):
        self.__fence = fence

    def wait_fence(self):
        if self.__fence:
            self.__fence.wait()


class FrameSync(object):
    def __init__(self, context: Context, render_queue: CommandBuffer):
        self.__image_available: Semaphore = Semaphore(context)
        self.__render_finished: Semaphore = Semaphore(context)
        self.__in_flight_fence: Fence = Fence(context)
        self.__render_queue: CommandBuffer = render_queue

    def __del__(self):
        self.__render_queue = None
        self.__in_flight_fence = None
        self.__render_finished = None
        self.__image_available = None

    @property
    def image_available(self) -> Semaphore:
        return self.__image_available

    @property
    def render_finished(self) -> Semaphore:
        return self.__render_finished

    @property
    def in_flight_fence(self) -> Fence:
        return self.__in_flight_fence

    @property
    def render_queue(self) -> CommandBuffer:
        return self.__render_queue


class SwapChain(object):
    def __init__(self, context: Context, surface_info: SurfaceInfo, old_swap_chain: SwapChain = None):
        self.__context: Context = context

        self.__frames: List[Frame] = None

        self.__swap_chain: VulkanHandle = None

        present_mode = context.physical_device.select_swap_present_mode()
        image_count = context.physical_device.get_surface_images()
        surface_transform = context.physical_device.get_surface_transform()

        self.__surface_info: SurfaceInfo = surface_info

        self.__image_count: int = image_count

        # Get Graphics Queues, determine sharing mode
        queue_indices = self.__context.logical_device.get_graphics_queue_indices()

        logger.info(f"Surface Format {surface_info.surface_format}, {surface_info.surface_colour_space}")
        logger.info(f"Swap Chain Resolution={surface_info.width}x{surface_info.height}")

        if len(set(queue_indices)) != len(queue_indices):
            queue_indices = None

        swap_chain_info = vk.VkSwapchainCreateInfoKHR(
            surface=context.surface.handle,
            minImageCount=image_count,
            imageFormat=surface_info.surface_format,
            imageColorSpace=surface_info.surface_colour_space,
            imageExtent=surface_info.get_extent(),
            imageArrayLayers=1,
            imageUsage=vk.VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
            imageSharingMode=vk.VK_SHARING_MODE_CONCURRENT if queue_indices else vk.VK_SHARING_MODE_EXCLUSIVE,
            pQueueFamilyIndices=queue_indices,
            preTransform=surface_transform,
            compositeAlpha=vk.VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
            presentMode=present_mode,
            clipped=True,
            oldSwapchain=None,  # TODO: Can assist with recreation...
        )

        self.__swap_chain = vkx.vkCreateSwapchainKHR(self.__context.logical_device.handle, swap_chain_info, None)
        logger.info("Vulkan Swap Chain: Created")

        swap_chain_images = vkx.vkGetSwapchainImagesKHR(self.__context.logical_device.handle, self.__swap_chain)

        self.__frames = [Frame(context, surface_info, idx, image) for idx, image in enumerate(swap_chain_images)]

    def __del__(self):
        if self.__swap_chain:
            vkx.vkDestroySwapchainKHR(self.__context.logical_device.handle, self.__swap_chain, None)
            logger.info("Vulkan Swap Chain: Destroyed")

        self.__frames.clear()
        self.__frames = None

        self.__swap_chain = None

    @property
    def handle(self) -> VulkanHandle:
        return self.__swap_chain

    @property
    def image_count(self) -> int:
        return self.__image_count

    def iter_frames(self) -> Generator[Frame, None, None]:
        for frame in self.__frames:
            yield frame

    def acquire_next_frame(self, sync: FrameSync = None) -> Frame:
        # TODO: Expect Exceptions for Resizing etc

        sync.in_flight_fence.wait()

        try:
            idx = vkx.vkAcquireNextImageKHR(
                self.__context.logical_device.handle,
                self.handle,
                int(1e9),
                sync.image_available.handle,
                vk.VK_NULL_HANDLE,
            )

            frame = self.__frames[idx]
            return frame
        except vk.VkErrorOutOfDateKhr as e:
            # TODO: Recreate Swap Chain
            logger.warning("Failed acquiring next image")
            raise e
        return None

    def present(self, command_buffer: CommandBuffer, sync: FrameSync, frame: Frame):

        # Wait for the frame to finish whatever it was doing.
        frame.wait_fence()

        # Update the fence to use for this image
        frame.use_fence(sync.in_flight_fence)

        # Reset Fence associated with this frame
        sync.in_flight_fence.reset()

        # Submit the graphics queue
        submit_info = vk.VkSubmitInfo(
            pWaitSemaphores=[sync.image_available.handle],
            pWaitDstStageMask=[vk.VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT],
            pCommandBuffers=[command_buffer.handle],
            pSignalSemaphores=[sync.render_finished.handle],
        )
        vk.vkQueueSubmit(self.__context.logical_device.graphics_queue.handle, 1, [submit_info], sync.in_flight_fence.handle)

        # Present
        present_info = vk.VkPresentInfoKHR(
            pWaitSemaphores=[sync.render_finished.handle],
            pSwapchains=[self.handle],
            pImageIndices=[frame.image_index],
        )

        vkx.vkQueuePresentKHR(self.__context.logical_device.present_queue.handle, present_info)
