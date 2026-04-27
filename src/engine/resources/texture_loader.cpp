#include "texture_loader.h"

#include <SDL_image.h>

#include <medusa/engine/context.h>
#include <medusa/graphics/texture.h>

#include <core/utilities/logging.h>

using namespace medusa;
using namespace medusa::loaders;


//
static SDL_Surface* toRGBA(SDL_Surface* surface)
{
    // SDL_PIXELFORMAT_RGBA32 is always RGBA byte-order regardless of endianness
    if (surface->format->format == SDL_PIXELFORMAT_RGBA32)
        return surface; // Already correct, no conversion needed

    SDL_Surface* converted = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA32, 0);
    if (! converted)
    {
        logging::error(fmt::format("TextureLoader: SDL format conversion failed: {}", SDL_GetError()));
    }

    return converted;
}


//
bool TextureAsset::info(std::shared_ptr<Config> config)
{
    std::string pathNodeName = fmt::format("{}.filename", getAssetName());

    setFilename(config->getValue<std::string>(pathNodeName, ""));

    return true;
}


//
std::shared_ptr<ITexture> TextureAsset::load(std::shared_ptr<IContext> context, std::shared_ptr<IAssetReader> reader)
{
    std::vector<uint8_t> buffer = reader->readBinary(getFilename());
    std::shared_ptr<ITexture> texture = loaders::TextureLoader::loadTexture2D(context, buffer);

    return texture;
}


//
std::shared_ptr<ITexture> TextureLoader::loadTexture2D(std::shared_ptr<IContext> context, const std::string& filename)
{
    // IMG_Load supports PNG, JPG, BMP, TGA, GIF, and more
    SDL_Surface* raw = IMG_Load(filename.c_str());
    if (!raw)
    {
        logging::error(fmt::format("TextureLoader: failed to load `{}`: {}", filename, IMG_GetError()));
        throw MedusaError("Failed to load image");
    }

    SDL_Surface* rgba = toRGBA(raw);
    if (!rgba)
    {
        SDL_FreeSurface(raw);
        throw MedusaError("Failed to convert image format");
    }

    uint32_t width = static_cast<uint32_t>(rgba->w);
    uint32_t height = static_cast<uint32_t>(rgba->h);

    // Create the texture
    std::shared_ptr<ITexture> texture = context->createTexture();
    bool result = texture->create(rgba->pixels, width, height, TextureFormat::RGBA8);
    if (! result)
    {
        logging::error(fmt::format("TextureLoader: ITexture::create() failed for `{}`", filename));
    }

    // Free the converted surface if it's a separate allocation
    if (rgba != raw)
        SDL_FreeSurface(rgba);

    SDL_FreeSurface(raw);

    return texture;
}


//
std::shared_ptr<ITexture> TextureLoader::loadTexture2D(std::shared_ptr<IContext> context, std::vector<uint8_t>& bytes)
{
    // Wrap the raw memory buffer as an SDL I/O stream
    SDL_RWops* rw = SDL_RWFromMem((void*)bytes.data(), (int)bytes.size());
    if (!rw)
    {
        SDL_Log("SDL_RWFromMem error: %s", SDL_GetError());
        return NULL;
    }

    //
    SDL_Surface* raw = IMG_Load_RW(rw, 0);
    if (!raw)
    {
        SDL_Log("IMG_Load_RW error: %s", IMG_GetError());
        return NULL;
    }

    // Convert to RGBA
    SDL_Surface* rgba = toRGBA(raw);
    if (!rgba)
    {
        SDL_FreeSurface(raw);
        throw MedusaError("Failed to convert image format");
    }

    uint32_t width = static_cast<uint32_t>(rgba->w);
    uint32_t height = static_cast<uint32_t>(rgba->h);

    // Create the texture
    std::shared_ptr<ITexture> texture = context->createTexture();
    bool result = texture->create(rgba->pixels, width, height, TextureFormat::RGBA8);
    if (!result)
    {
        logging::error(fmt::format("TextureLoader: ITexture::create() failed from RAW binary"));
    }

    // Free the converted surface if it's a separate allocation
    if (rgba != raw)
        SDL_FreeSurface(rgba);

    SDL_FreeSurface(raw);

    return texture;
}
