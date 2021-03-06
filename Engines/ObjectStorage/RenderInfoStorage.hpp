#pragma once
#include <string>
#include <unordered_map>
#include "Shared/AbstractException.hpp"
DefineNewException(RenderInfoException);
#include "FileLoader.hpp"
#include "DefaultObjectStorageData.hpp"
#include <memory>
class RenderInfo;
namespace mgl {
	class Color;
	namespace math {
		class vectorH;
	}
}
class RenderInfoStorage : protected FileLoader {
	using Palette = std::vector<std::shared_ptr<mgl::Color>>;
private:
	static bool wasRenderInfoLoaded;
protected:
	static std::unordered_map<std::string, std::shared_ptr<RenderInfo>> m_data;
	static std::unordered_map<std::string, Palette> m_palettes;
	size_t m_current_mode;
	Palette m_current_palette;
	std::vector<std::shared_ptr<RenderInfo>> m_current_render_info;
	std::shared_ptr<mgl::Color> m_current_color;
	std::shared_ptr<mgl::math::vectorH> m_current_scale;
	std::string m_current_name;
	size_t m_current_index;
protected:
	virtual void parse_file_type_info(std::string const& line) override;
	virtual void parse_line(std::string const& line) override;
	virtual void finalize_parsing() override;

	virtual bool parse_special_line(std::string const& line);
	virtual void parse_object_line(std::string const& line) { return parse_object_line(line, 0u); }
	virtual void parse_object_line(std::string const& line, size_t index);
	virtual void parse_palette_line(std::string const& line);
	virtual void parse_object_array_line(std::string const& line);
	virtual void parse_palette_object_array_line(std::string const& line);

	virtual std::string generate_file_type_info() override { return "with RenderInfo"; }

	void finalize_object(size_t mode);
public:
	RenderInfoStorage();
	static std::shared_ptr<RenderInfo> getRenderInfo(std::string const& obj);
	static std::shared_ptr<RenderInfo> getRenderInfo(std::string const& obj, size_t index);
	static Palette& getPalette(std::string const& obj);
	static std::string getPalette(Palette& inf);
	void load(std::string const& path = "/") { FileLoader::load(path, RenderInfoFileExtention); wasRenderInfoLoaded = true; }
	static void clean();
};