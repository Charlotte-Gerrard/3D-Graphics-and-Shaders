#include "MaterialDX11.h"

namespace LJMUDX
{
	////////////////////////////////////
	// Static Class to help wire up our
	// Per-object shaders to our Actor
	// objects - does this by generating
	// Material Files
	////////////////////////////////////
	class LJMUShaderMaker
	{
	public:
		//---------STATIC METHODS----------------------------------------------------
		static Glyph3::MaterialPtr MakeCustom(Glyph3::RendererDX11& Renderer, std::wstring& file, unsigned int shaders);
		static Glyph3::MaterialPtr MakeStaticMesh(Glyph3::RendererDX11& Renderer);
		static Glyph3::MaterialPtr MakeActorPhong(Glyph3::RendererDX11& Renderer);
		static Glyph3::MaterialPtr MakeActorTextured(Glyph3::RendererDX11& Renderer);

	private:
		//---------CONSTRUCTORS/DESTRUCTORS------------------------------------------
		LJMUShaderMaker(){};
		~LJMUShaderMaker(){};
	};

}