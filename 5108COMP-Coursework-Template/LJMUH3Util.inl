namespace LJMUDX
{

	void LJMUResourceHelper::loadImage(int pid, const std::wstring& pfilename)
	{
		ResourcePtr tresource;
		tresource = RendererDX11::Get()->LoadTexture(pfilename);
		if (tresource == nullptr)
		{
			std::cout << "Error Loading " << pfilename.c_str();
		}

		// If loading successful, insert resource to map
		this->insertResource(pid, std::move(tresource));
	}

	void LJMUResourceHelper::loadFont(int pid, std::wstring& pname, int psize)
	{
		SpriteFontPtr tresource;
		tresource = SpriteFontLoaderDX11::LoadFont(pname, psize, 0, false);
		if (tresource == nullptr)
		{
			std::cout << "Error Loading " << pname.c_str();
		}

		// If loading successful, insert resource to map
		this->insertResource(pid, std::move(tresource));
	}

	ResourcePtr LJMUResourceHelper::getImage(int pid)
	{
		auto tfound = this->_res_map.find(pid);
		assert(tfound != this->_res_map.end());
		return tfound->second;
	}

	SpriteFontPtr LJMUResourceHelper::getFont(int pid)
	{
		auto tfound = this->_res_map_font.find(pid);
		assert(tfound != this->_res_map_font.end());
		return tfound->second;
	}

	void LJMUResourceHelper::insertResource(int pid, ResourcePtr presource)
	{
		// Insert and check success
		auto tinserted = this->_res_map.insert(std::make_pair(pid, std::move(presource)));
		assert(tinserted.second);
	}

	void LJMUResourceHelper::insertResource(int pid, SpriteFontPtr presource)
	{
		// Insert and check success
		auto tinserted = this->_res_map_font.insert(std::make_pair(pid, std::move(presource)));
		assert(tinserted.second);
	}

}