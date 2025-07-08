#pragma once

#include <medusa/medusa.h>

namespace medusa
{
	enum class BufferType
	{
		Array,
		//AtomicCounter,
		//CopyRead,
		//CopyWrite,
		//DispatchIndirect,
		//DrawIndirect,
		ElementArray,
		//PixelPack,
		//PixelUnpack,
		//Query,
		ShaderStorage,
		Texture,
		//TransformFeedback,
		Uniform,
	};

	enum class BufferUsage
	{
		StaticDraw,
		StaticRead,
		StaticCopy,
		DynamicDraw,
		DynamicRead,
		DynamicCopy,
		StreamDraw,
		StreamRead,
		StreamCopy,
	};


	/// <summary>
	///
	/// </summary>
	class IBuffer
	{
	public:
		IBuffer() {}
		virtual ~IBuffer() {}

		// Binding
		virtual bool bind() const = 0;
		virtual bool unbind() const = 0;

		// Buffer
		virtual inline size_t size() = 0;
		//virtual inline void* data() = 0;
		virtual bool create(const void* ptr, size_t nBytes, BufferUsage usage) = 0;
		virtual bool update(const void* ptr, size_t nOffset, size_t nBytes) = 0;

		// API
		virtual void* handle() = 0;
		// virtual BufferType type() = 0; // Not necessary most likely -- but could end up being a templated literal
		virtual BufferUsage usage() = 0;

	private:
	};





	class IUniformBuffer : public IBuffer
	{
	public:
		virtual inline const uint32_t bindingIndex() const = 0;
	};

	class IStorageBuffer : public IBuffer
	{
	public:
		virtual inline const uint32_t bindingIndex() const = 0;
	};


	//typedef IBuffer IUniformBuffer;
	//typedef IBuffer IStorageBuffer;
}
