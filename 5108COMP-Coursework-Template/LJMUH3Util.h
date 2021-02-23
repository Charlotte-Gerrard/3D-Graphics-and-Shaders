#ifndef __LJMU_UTIL_H_
#define __LJMU_UTIL_H_

#include <map>
#include <string>
#include <memory>
#include <stdexcept>
#include <cassert>
#include "ResourceProxyDX11.h"
#include "SpriteFontLoaderDX11.h"
#include "RendererDX11.h"
#include <iostream>

using namespace Glyph3;

namespace LJMUDX
{
	//---------------------UTIL RESOURCE HOLDER CLASS---------------------------------------------------------------------------------
	class LJMUResourceHelper
	{
	public:
		void			loadImage(int pid, const std::wstring& pfilename);
		void			loadFont(int pid, std::wstring& pfilename, int psize);
		ResourcePtr		getImage(int pid);
		SpriteFontPtr   getFont(int pid);

	private:
		void	insertResource(int pid, ResourcePtr presource);
		void	insertResource(int pid, SpriteFontPtr presource);


	private:
		std::map<int, ResourcePtr>	_res_map;
		std::map<int, SpriteFontPtr> _res_map_font;
	};
}

#include "LJMUH3Util.inl"

#endif 
