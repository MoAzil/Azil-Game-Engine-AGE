#pragma once

#include <memory>

namespace RendererAPI
{

// Forward declarations for user-defined types
struct Mesh;
struct Texture;
struct Vector3;

// Abstract base interface for all renderer backends
class IRenderer
{
public:
    virtual ~IRenderer() = default;

    // Core rendering function — draw a mesh with a texture, transparency, and position
    virtual void draw_mesh(const Mesh& mesh, const Texture& texture, float transparency, const Vector3& position) = 0;

    // Extend with more virtual functions here, e.g.:
    // virtual void clear_screen() = 0;
    // virtual void set_viewport(int x, int y, int width, int height) = 0;
};

// Enum to select which backend to use
enum class APIType
{
    None = 0,
    OpenGL,
    // Vulkan,
    // DirectX,
    // Metal,
};

// The RenderAPI class: singleton-style static interface for client code
class RenderAPI
{
public:
    // Initialize with desired backend API
    static void init(APIType type);

    // Release resources
    static void shutdown();

    // Check if API is initialized
    static bool is_initialized();

    // Main rendering call
    static void draw_mesh(const Mesh& mesh, const Texture& texture, float transparency, const Vector3& position);

    // You can add static wrappers for new features here,
    // which internally forward calls to the current backend.

private:
    // Pointer to the current backend implementation
    static std::unique_ptr<IRenderer> s_renderer;
};

} // namespace RendererAPI
