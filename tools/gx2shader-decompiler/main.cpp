#include "gx2_shaders.h"

#include <excmd.h>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <libgpu/src/spirv/spirv_transpiler.h>

#include <memory>
#include <string>
#include <stdexcept>

#include <algorithm>
#include <vector>

template<typename ... Args>
std::string string_format( const std::string& format, Args ... args )
{
    int size_s = std::snprintf( nullptr, 0, format.c_str(), args ... ) + 1; // Extra space for '\0'
    if( size_s <= 0 ){ throw std::runtime_error( "Error during formatting." ); }
    auto size = static_cast<size_t>( size_s );
    auto buf = std::make_unique<char[]>( size );
    std::snprintf( buf.get(), size, format.c_str(), args ... );
    return std::string( buf.get(), buf.get() + size - 1 ); // We don't want the '\0' inside
}

forceinline const char* GX2SamplerVarTypeStr(cafe::gx2::GX2SamplerVarType type)
{
    switch (type)
    {
    case cafe::gx2::GX2SamplerVarType::Sampler1D:
        return "cafe::gx2::GX2SamplerVarType::Sampler1D";
    case cafe::gx2::GX2SamplerVarType::Sampler2D:
        return "cafe::gx2::GX2SamplerVarType::Sampler2D";
    case cafe::gx2::GX2SamplerVarType::Sampler2DRect:
        return "cafe::gx2::GX2SamplerVarType::Sampler2DRect";
    case cafe::gx2::GX2SamplerVarType::Sampler3D:
        return "cafe::gx2::GX2SamplerVarType::Sampler3D";
    case cafe::gx2::GX2SamplerVarType::SamplerCube:
        return "cafe::gx2::GX2SamplerVarType::SamplerCube";
    case cafe::gx2::GX2SamplerVarType::Sampler1DShadow:
        return "cafe::gx2::GX2SamplerVarType::Sampler1DShadow";
    case cafe::gx2::GX2SamplerVarType::Sampler2DShadow:
        return "cafe::gx2::GX2SamplerVarType::Sampler2DShadow";
    case cafe::gx2::GX2SamplerVarType::Sampler2DRectShadow:
        return "cafe::gx2::GX2SamplerVarType::Sampler2DRectShadow";
    case cafe::gx2::GX2SamplerVarType::SamplerCubeShadow:
        return "cafe::gx2::GX2SamplerVarType::SamplerCubeShadow";
    case cafe::gx2::GX2SamplerVarType::Sampler1DArray:
        return "cafe::gx2::GX2SamplerVarType::Sampler1DArray";
    case cafe::gx2::GX2SamplerVarType::Sampler2DArray:
        return "cafe::gx2::GX2SamplerVarType::Sampler2DArray";
    case cafe::gx2::GX2SamplerVarType::Sampler1DArrayShadow:
        return "cafe::gx2::GX2SamplerVarType::Sampler1DArrayShadow";
    case cafe::gx2::GX2SamplerVarType::Sampler2DArrayShadow:
        return "cafe::gx2::GX2SamplerVarType::Sampler2DArrayShadow";
    case cafe::gx2::GX2SamplerVarType::SamplerCubeArray:
        return "cafe::gx2::GX2SamplerVarType::SamplerCubeArray";
    case cafe::gx2::GX2SamplerVarType::SamplerCubeArrayShadow:
        return "cafe::gx2::GX2SamplerVarType::SamplerCubeArrayShadow";
    case cafe::gx2::GX2SamplerVarType::SamplerBuffer:
        return "cafe::gx2::GX2SamplerVarType::SamplerBuffer";
    case cafe::gx2::GX2SamplerVarType::Sampler2DMSAA:
        return "cafe::gx2::GX2SamplerVarType::Sampler2DMSAA";
    case cafe::gx2::GX2SamplerVarType::Sampler2DMSAAArray:
        return "cafe::gx2::GX2SamplerVarType::Sampler2DMSAAArray";
    default:
        assert(!"Invalid / Unsupported GX2SamplerVarType");
        return "";
    }
}

forceinline const char* SQ_TEX_DIM_Str(latte::SQ_TEX_DIM dim)
{
    switch (dim)
    {
    case latte::SQ_TEX_DIM::DIM_1D:
        return "latte::SQ_TEX_DIM::DIM_1D";
    case latte::SQ_TEX_DIM::DIM_2D:
        return "latte::SQ_TEX_DIM::DIM_2D";
    case latte::SQ_TEX_DIM::DIM_3D:
        return "latte::SQ_TEX_DIM::DIM_3D";
    case latte::SQ_TEX_DIM::DIM_CUBEMAP:
        return "latte::SQ_TEX_DIM::DIM_CUBEMAP";
    case latte::SQ_TEX_DIM::DIM_1D_ARRAY:
        return "latte::SQ_TEX_DIM::DIM_1D_ARRAY";
    case latte::SQ_TEX_DIM::DIM_2D_ARRAY:
        return "latte::SQ_TEX_DIM::DIM_2D_ARRAY";
    case latte::SQ_TEX_DIM::DIM_2D_MSAA:
        return "latte::SQ_TEX_DIM::DIM_2D_MSAA";
    case latte::SQ_TEX_DIM::DIM_2D_ARRAY_MSAA:
        return "latte::SQ_TEX_DIM::DIM_2D_ARRAY_MSAA";
    default:
        assert(!"Invalid / Unknown latte::SQ_TEX_DIM");
        return "";
    }
}

static latte::SQ_TEX_DIM getTexDimFromSamplerVarType(cafe::gx2::GX2SamplerVarType type)
{
    switch (type)
    {
    case cafe::gx2::GX2SamplerVarType::Sampler1D:
    case cafe::gx2::GX2SamplerVarType::Sampler1DShadow:
    case cafe::gx2::GX2SamplerVarType::SamplerBuffer:
        return latte::SQ_TEX_DIM::DIM_1D;
    case cafe::gx2::GX2SamplerVarType::Sampler2D:
    case cafe::gx2::GX2SamplerVarType::Sampler2DRect:
    case cafe::gx2::GX2SamplerVarType::Sampler2DShadow:
    case cafe::gx2::GX2SamplerVarType::Sampler2DRectShadow:
        return latte::SQ_TEX_DIM::DIM_2D;
    case cafe::gx2::GX2SamplerVarType::Sampler3D:
        return latte::SQ_TEX_DIM::DIM_3D;
    case cafe::gx2::GX2SamplerVarType::SamplerCube:
    case cafe::gx2::GX2SamplerVarType::SamplerCubeShadow:
        return latte::SQ_TEX_DIM::DIM_CUBEMAP;
    case cafe::gx2::GX2SamplerVarType::Sampler1DArray:
    case cafe::gx2::GX2SamplerVarType::Sampler1DArrayShadow:
        return latte::SQ_TEX_DIM::DIM_1D_ARRAY;
    case cafe::gx2::GX2SamplerVarType::Sampler2DArray:
    case cafe::gx2::GX2SamplerVarType::Sampler2DArrayShadow:
    //case cafe::gx2::GX2SamplerVarType::SamplerCube:
    //case cafe::gx2::GX2SamplerVarType::SamplerCubeShadow:
    case cafe::gx2::GX2SamplerVarType::SamplerCubeArray:
    case cafe::gx2::GX2SamplerVarType::SamplerCubeArrayShadow:
        return latte::SQ_TEX_DIM::DIM_2D_ARRAY;
    case cafe::gx2::GX2SamplerVarType::Sampler2DMSAA:
        return latte::SQ_TEX_DIM::DIM_2D_MSAA;
    case cafe::gx2::GX2SamplerVarType::Sampler2DMSAAArray:
        return latte::SQ_TEX_DIM::DIM_2D_ARRAY_MSAA;
    default:
        assert(!"Invalid / Unsupported GX2SamplerVarType");
        return latte::SQ_TEX_DIM::DIM_2D;
    }
}

std::string* CurrentAttribNames = NULL;
size_t MaxAttribCount = 0;

int main(int argc, char** argv)
{
    std::cout << "GX2Shader Decompiler v1.0" << std::endl << std::endl;

    excmd::parser parser;
    excmd::option_state options;

    parser.global_options()
      .add_option("v,vertex", excmd::value<std::string> { });

    parser.global_options()
      .add_option("p,pixel", excmd::value<std::string> { });

    parser.global_options()
      .add_option("g,geometry", excmd::value<std::string> { });

    // Parse command line
    try
    {
        options = parser.parse(argc, argv);
    }

    catch (excmd::exception ex)
    {
        std::cout << "Error parsing command line: " << ex.what() << std::endl;
		return 1;
    }

    if (!options.has("vertex") || !options.has("pixel"))
    {
        std::cout << "Error: must specify vertex and pixel shaders." << std::endl;
		return 1;
    }

    const GX2VertexShader* vertexShader;
    const GX2PixelShader* pixelShader;
    const GX2GeometryShader* geometryShader = NULL;

    std::string vshFilename = options.get<std::string>("vertex");
    std::string pshFilename = options.get<std::string>("pixel");
    std::string gshFilename;

    {
        if (!std::filesystem::exists(vshFilename))
        {
            std::cout << "Error: file not found: " << vshFilename << std::endl;
            return 1;
        }

	    std::ifstream inf(vshFilename.c_str(), std::ifstream::in | std::ifstream::binary);

        inf.seekg(0, std::ios::end);
        const size_t fileSize = inf.tellg();
        inf.seekg(0, std::ios::beg);

        uint8_t* const inb = new uint8_t[fileSize];
        inf.read(reinterpret_cast<char*>(inb), fileSize);
        inf.close();

        vertexShader = reinterpret_cast<const GX2VertexShader*>(inb);
	}

	{
	    if (!std::filesystem::exists(pshFilename))
        {
            std::cout << "Error: file not found: " << pshFilename << std::endl;

            delete[] vertexShader;
            return 1;
        }

	    std::ifstream inf(pshFilename.c_str(), std::ifstream::in | std::ifstream::binary);

        inf.seekg(0, std::ios::end);
        const size_t fileSize = inf.tellg();
        inf.seekg(0, std::ios::beg);

        uint8_t* const inb = new uint8_t[fileSize];
        inf.read(reinterpret_cast<char*>(inb), fileSize);
        inf.close();

        pixelShader = reinterpret_cast<const GX2PixelShader*>(inb);
	}

    if (options.has("geometry"))
	{
	    gshFilename = options.get<std::string>("geometry");
	    if (!std::filesystem::exists(gshFilename))
        {
            std::cout << "Error: file not found: " << gshFilename << std::endl;

            delete[] vertexShader;
            delete[] pixelShader;

            return 1;
        }

	    std::ifstream inf(gshFilename.c_str(), std::ifstream::in | std::ifstream::binary);

        inf.seekg(0, std::ios::end);
        const size_t fileSize = inf.tellg();
        inf.seekg(0, std::ios::beg);

        uint8_t* const inb = new uint8_t[fileSize];
        inf.read(reinterpret_cast<char*>(inb), fileSize);
        inf.close();

        geometryShader = reinterpret_cast<const GX2GeometryShader*>(inb);
	}

	{
        uint32_t maxAttribCount = 0;
        for (uint32_t i = 0; i < vertexShader->attribVarCount; i++)
        {
            const GX2AttribVar* attribVar = vertexShader->attribVars(vertexShader) + i;
            maxAttribCount += std::max(1u, attribVar->count) * GX2ShaderVarTypeToStreamCount(attribVar->type);
        }

        CurrentAttribNames = new std::string[maxAttribCount]; MaxAttribCount = maxAttribCount;
        GX2AttribStream* attribs = new GX2AttribStream[maxAttribCount];
        uint32_t attribCount = 0;
        uint32_t offset = 0;

        for (uint32_t i = 0; i < vertexShader->attribVarCount; i++)
        {
            const GX2AttribVar* attribVar = vertexShader->attribVars(vertexShader) + i;
            const char* name = attribVar->name(vertexShader);
            const uint32_t location = attribVar->location;
            const uint32_t arrayCount = std::max(1u, attribVar->count);
            const uint32_t streamCount = GX2ShaderVarTypeToStreamCount(attribVar->type);
            const cafe::gx2::GX2AttribFormat attribFormat = GX2ShaderVarTypeToAttribFormat(attribVar->type);
            const uint32_t byteSize = GX2AttribFormatByteSize(attribFormat);

#ifdef DEBUG
            std::cout << name << std::endl;
            std::cout << location << std::endl;
            std::cout << arrayCount << std::endl;
            std::cout << streamCount << std::endl;
            std::cout << GX2ShaderVarTypeStr(attribVar->type) << std::endl;
            std::cout << GX2ShaderVarTypeToAttribFormatStr(attribVar->type) << std::endl << std::endl;
#endif

            for (uint32_t y = 0; y < arrayCount; y++)
                for (uint32_t x = 0; x < streamCount; x++)
                {
                    CurrentAttribNames[attribCount] = string_format("%s_%u_%u", name, y, x);
                    GX2InitAttribStream(attribs + attribCount, location + y * streamCount + x, attribCount >> 5, offset, attribFormat, attribCount);

                    if ((attribCount & 31) && !(attribCount + 1 & 31))
                        offset = 0;
                    else
                        offset += byteSize;

                    attribCount++;
                }
        }

        GX2FetchShader fetchShader;
        uint8_t* const fetchShaderBuffer = new uint8_t[GX2CalcFetchShaderSize(attribCount)];
        GX2InitFetchShader(&fetchShader, fetchShaderBuffer, attribCount, attribs);

        spirv::VertexShaderDesc vtxShaderDesc = spirv::VertexShaderDesc { };
        vtxShaderDesc.type = spirv::ShaderType::Vertex;
        vtxShaderDesc.binary = gsl::make_span(vertexShader->data(vertexShader), vertexShader->size);
        vtxShaderDesc.fsBinary = gsl::make_span(fetchShader.data, fetchShader.size);
        vtxShaderDesc.aluInstPreferVector = true;

        std::vector<uint32_t> usedSamplers;

        for (uint32_t i = 0; i < vertexShader->samplerVarCount; i++)
        {
            const GX2SamplerVar* samplerVar = vertexShader->samplerVars(vertexShader) + i;
            uint32_t location = samplerVar->location;
            if (location >= latte::MaxTextures || location >= latte::MaxSamplers)
                continue;

            const cafe::gx2::GX2SamplerVarType samplerVarType = samplerVar->type;

#ifdef DEBUG
            const char* const name = samplerVar->name(vertexShader);
            std::cout << name << std::endl;
            std::cout << location << std::endl;
            std::cout << GX2SamplerVarTypeStr(samplerVarType) << std::endl;
#endif
            const latte::SQ_TEX_DIM dim = getTexDimFromSamplerVarType(samplerVarType);
#ifdef DEBUG
            std::cout << SQ_TEX_DIM_Str(dim) << std::endl << std::endl;
#endif

            vtxShaderDesc.texDims[location] = dim;
            usedSamplers.push_back(location);
        }

        for (auto i = 0; i < latte::MaxTextures; ++i)
        {
            if (std::find(usedSamplers.begin(), usedSamplers.end(), static_cast<uint32_t>(i)) == usedSamplers.end())
                vtxShaderDesc.texDims[i] = latte::SQ_TEX_DIM::DIM_2D;

            vtxShaderDesc.texFormat[i] = spirv::TextureInputType::FLOAT;
        }

        vtxShaderDesc.regs.sq_pgm_resources_vs = vertexShader->regs.sq_pgm_resources_vs;
        vtxShaderDesc.regs.pa_cl_vs_out_cntl = vertexShader->regs.pa_cl_vs_out_cntl;

        for (auto i = 0u; i < vertexShader->regs.num_sq_vtx_semantic; ++i)
            vtxShaderDesc.regs.sq_vtx_semantics[i] = vertexShader->regs.sq_vtx_semantic[i];

        for (auto i = vertexShader->regs.num_sq_vtx_semantic; i < 32; ++i)
        {
            vtxShaderDesc.regs.sq_vtx_semantics[i] = vtxShaderDesc.regs.sq_vtx_semantics[i]
                .SEMANTIC_ID(0);
        }

        if (vertexShader->hasStreamOut)
            for (auto i = 0; i < latte::MaxStreamOutBuffers; ++i)
                vtxShaderDesc.streamOutStride[i] = vertexShader->streamOutStride[i];

        else
            for (auto i = 0; i < latte::MaxStreamOutBuffers; ++i)
                vtxShaderDesc.streamOutStride[i] = 0;

#ifdef DEBUG
        std::cout << "vertex" << std::endl;
#endif
        spirv::VertexShader vtxDecomp;
        const auto success = spirv::translate(vtxShaderDesc, &vtxDecomp);

        delete[] fetchShaderBuffer;
        delete[] attribs;
        delete[] CurrentAttribNames; CurrentAttribNames = NULL; MaxAttribCount = 0;

        if (!success)
        {
            std::cout << "Failed to translate vertex shader" << std::endl;

            delete[] vertexShader;
            delete[] pixelShader;
            delete[] geometryShader;

            return 1;
        }

        {
            std::ofstream outf(vshFilename + ".vert.spv", std::ofstream::out | std::ofstream::binary);
            outf.write(reinterpret_cast<const char*>(vtxDecomp.binary.data()), vtxDecomp.binary.size() * sizeof(unsigned int));
            outf.close();
        }
	}

    {
        spirv::PixelShaderDesc frgShaderDesc = spirv::PixelShaderDesc { };
        frgShaderDesc.type = spirv::ShaderType::Pixel;
        frgShaderDesc.binary = gsl::make_span(pixelShader->data(pixelShader), pixelShader->size);
        frgShaderDesc.aluInstPreferVector = true;

        for (auto i = 0; i < latte::MaxRenderTargets; ++i)
            frgShaderDesc.pixelOutType[i] = spirv::PixelOutputType::FLOAT;

        std::vector<uint32_t> usedSamplers;

        for (uint32_t i = 0; i < pixelShader->samplerVarCount; i++)
        {
            const GX2SamplerVar* samplerVar = pixelShader->samplerVars(pixelShader) + i;
            uint32_t location = samplerVar->location;
            if (location >= latte::MaxTextures || location >= latte::MaxSamplers)
                continue;

            const cafe::gx2::GX2SamplerVarType samplerVarType = samplerVar->type;

#ifdef DEBUG
            const char* const name = samplerVar->name(pixelShader);
            std::cout << name << std::endl;
            std::cout << location << std::endl;
            std::cout << GX2SamplerVarTypeStr(samplerVarType) << std::endl;
#endif
            const latte::SQ_TEX_DIM dim = getTexDimFromSamplerVarType(samplerVarType);
#ifdef DEBUG
            std::cout << SQ_TEX_DIM_Str(dim) << std::endl << std::endl;
#endif

            frgShaderDesc.texDims[location] = dim;
            usedSamplers.push_back(location);
        }

        for (auto i = 0; i < latte::MaxTextures; ++i)
        {
            if (std::find(usedSamplers.begin(), usedSamplers.end(), static_cast<uint32_t>(i)) == usedSamplers.end())
                frgShaderDesc.texDims[i] = latte::SQ_TEX_DIM::DIM_2D;

            frgShaderDesc.texFormat[i] = spirv::TextureInputType::FLOAT;
        }

        frgShaderDesc.regs.sq_pgm_resources_ps = pixelShader->regs.sq_pgm_resources_ps;
        frgShaderDesc.regs.sq_pgm_exports_ps = pixelShader->regs.sq_pgm_exports_ps;

        frgShaderDesc.regs.spi_ps_in_control_0 = pixelShader->regs.spi_ps_in_control_0;
        frgShaderDesc.regs.spi_ps_in_control_1 = pixelShader->regs.spi_ps_in_control_1;
        frgShaderDesc.regs.spi_vs_out_config = vertexShader->regs.spi_vs_out_config;

        frgShaderDesc.regs.cb_shader_control = pixelShader->regs.cb_shader_control;
        frgShaderDesc.regs.cb_shader_mask = pixelShader->regs.cb_shader_mask;
        frgShaderDesc.regs.db_shader_control = pixelShader->regs.db_shader_control;

        for (auto i = 0u; i < pixelShader->regs.num_spi_ps_input_cntl; ++i)
            frgShaderDesc.regs.spi_ps_input_cntls[i] = pixelShader->regs.spi_ps_input_cntls[i];

        for (auto i = pixelShader->regs.num_spi_ps_input_cntl; i < 32; ++i)
        {
            frgShaderDesc.regs.spi_ps_input_cntls[i] = frgShaderDesc.regs.spi_ps_input_cntls[i]
                .SEMANTIC(0)
                .DEFAULT_VAL(0)
                .FLAT_SHADE(0)
                .SEL_CENTROID(0)
                .SEL_LINEAR(0)
                .CYL_WRAP(0)
                .PT_SPRITE_TEX(0)
                .SEL_SAMPLE(0);
        }

        for (auto i = 0u; i < vertexShader->regs.num_spi_vs_out_id; ++i)
            frgShaderDesc.regs.spi_vs_out_ids[i] = vertexShader->regs.spi_vs_out_id[i];

        for (auto i = vertexShader->regs.num_spi_vs_out_id; i < 10; ++i)
        {
            frgShaderDesc.regs.spi_vs_out_ids[i] = frgShaderDesc.regs.spi_vs_out_ids[i]
                .SEMANTIC_0(0)
                .SEMANTIC_1(0)
                .SEMANTIC_2(0)
                .SEMANTIC_3(0);
        }

#ifdef DEBUG
        std::cout << "pixel" << std::endl;
#endif
        spirv::PixelShader frgDecomp;
        if (!spirv::translate(frgShaderDesc, &frgDecomp))
        {
            std::cout << "Failed to translate pixel shader" << std::endl;

            delete[] vertexShader;
            delete[] pixelShader;
            delete[] geometryShader;

            return 1;
        }

        {
            std::ofstream outf(pshFilename + ".frag.spv", std::ofstream::out | std::ofstream::binary);
            outf.write(reinterpret_cast<const char*>(frgDecomp.binary.data()), frgDecomp.binary.size() * sizeof(unsigned int));
            outf.close();
        }
	}

    if (geometryShader != NULL)
    {
        spirv::GeometryShaderDesc geoShaderDesc = spirv::GeometryShaderDesc { };
        geoShaderDesc.type = spirv::ShaderType::Geometry;
        geoShaderDesc.binary = gsl::make_span(geometryShader->data(geometryShader), geometryShader->size);
        geoShaderDesc.dcBinary = gsl::make_span(geometryShader->vertexShaderData(geometryShader), geometryShader->vertexShaderSize);
        geoShaderDesc.aluInstPreferVector = true;

        std::vector<uint32_t> usedSamplers;

        for (uint32_t i = 0; i < geometryShader->samplerVarCount; i++)
        {
            const GX2SamplerVar* samplerVar = geometryShader->samplerVars(geometryShader) + i;
            uint32_t location = samplerVar->location;
            if (location >= latte::MaxTextures || location >= latte::MaxSamplers)
                continue;

            const cafe::gx2::GX2SamplerVarType samplerVarType = samplerVar->type;

#ifdef DEBUG
            const char* const name = samplerVar->name(geometryShader);
            std::cout << name << std::endl;
            std::cout << location << std::endl;
            std::cout << GX2SamplerVarTypeStr(samplerVarType) << std::endl;
#endif
            const latte::SQ_TEX_DIM dim = getTexDimFromSamplerVarType(samplerVarType);
#ifdef DEBUG
            std::cout << SQ_TEX_DIM_Str(dim) << std::endl << std::endl;
#endif

            geoShaderDesc.texDims[location] = dim;
            usedSamplers.push_back(location);
        }

        for (auto i = 0; i < latte::MaxTextures; ++i)
        {
            if (std::find(usedSamplers.begin(), usedSamplers.end(), static_cast<uint32_t>(i)) == usedSamplers.end())
                geoShaderDesc.texDims[i] = latte::SQ_TEX_DIM::DIM_2D;

            geoShaderDesc.texFormat[i] = spirv::TextureInputType::FLOAT;
        }

        geoShaderDesc.regs.sq_gs_vert_itemsize = geometryShader->regs.sq_gs_vert_itemsize;
        geoShaderDesc.regs.vgt_gs_out_prim_type = geometryShader->regs.vgt_gs_out_prim_type.PRIM_TYPE();
        geoShaderDesc.regs.vgt_gs_mode = geometryShader->regs.vgt_gs_mode;
        geoShaderDesc.regs.sq_gsvs_ring_itemsize = geometryShader->ringItemSize;
        geoShaderDesc.regs.pa_cl_vs_out_cntl = geometryShader->regs.pa_cl_vs_out_cntl;

        if (geometryShader->hasStreamOut)
            for (auto i = 0; i < latte::MaxStreamOutBuffers; ++i)
                geoShaderDesc.streamOutStride[i] = geometryShader->streamOutStride[i];

        else
            for (auto i = 0; i < latte::MaxStreamOutBuffers; ++i)
                geoShaderDesc.streamOutStride[i] = 0;

#ifdef DEBUG
        std::cout << "geometry" << std::endl;
#endif
        spirv::GeometryShader geoDecomp;
        if (!spirv::translate(geoShaderDesc, &geoDecomp))
        {
            std::cout << "Failed to translate geometry shader" << std::endl;

            delete[] vertexShader;
            delete[] pixelShader;
            delete[] geometryShader;

            return 1;
        }

        {
            std::ofstream outf(gshFilename + ".geom.spv", std::ofstream::out | std::ofstream::binary);
            outf.write(reinterpret_cast<const char*>(geoDecomp.binary.data()), geoDecomp.binary.size() * sizeof(unsigned int));
            outf.close();
        }
    }

    delete[] vertexShader;
    delete[] pixelShader;
    delete[] geometryShader;

    return 0;
}
