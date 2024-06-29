#include "gx2_shaders.h"

#include <common/align.h>
#include <libgpu/latte/latte_instructions.h>

namespace {

struct IndexMapEntry
{
    uint32_t gpr;
    latte::SQ_CHAN chan;
};

static const auto
FetchesPerControlFlow = 16u;

static latte::SQ_DATA_FORMAT getAttribFormatDataFormat(cafe::gx2::GX2AttribFormat format)
{
    switch (format & 0x1F)
    {
    case cafe::gx2::GX2AttribFormatType::TYPE_8:
        return latte::SQ_DATA_FORMAT::FMT_8;
    case cafe::gx2::GX2AttribFormatType::TYPE_4_4:
        return latte::SQ_DATA_FORMAT::FMT_4_4;
    case cafe::gx2::GX2AttribFormatType::TYPE_16:
        return latte::SQ_DATA_FORMAT::FMT_16;
    case cafe::gx2::GX2AttribFormatType::TYPE_16_FLOAT:
        return latte::SQ_DATA_FORMAT::FMT_16_FLOAT;
    case cafe::gx2::GX2AttribFormatType::TYPE_8_8:
        return latte::SQ_DATA_FORMAT::FMT_8_8;
    case cafe::gx2::GX2AttribFormatType::TYPE_32:
        return latte::SQ_DATA_FORMAT::FMT_32;
    case cafe::gx2::GX2AttribFormatType::TYPE_32_FLOAT:
        return latte::SQ_DATA_FORMAT::FMT_32_FLOAT;
    case cafe::gx2::GX2AttribFormatType::TYPE_16_16:
        return latte::SQ_DATA_FORMAT::FMT_16_16;
    case cafe::gx2::GX2AttribFormatType::TYPE_16_16_FLOAT:
        return latte::SQ_DATA_FORMAT::FMT_16_16_FLOAT;
    case cafe::gx2::GX2AttribFormatType::TYPE_10_11_11_FLOAT:
        return latte::SQ_DATA_FORMAT::FMT_10_11_11_FLOAT;
    case cafe::gx2::GX2AttribFormatType::TYPE_8_8_8_8:
        return latte::SQ_DATA_FORMAT::FMT_8_8_8_8;
    case cafe::gx2::GX2AttribFormatType::TYPE_10_10_10_2:
        return latte::SQ_DATA_FORMAT::FMT_10_10_10_2;
    case cafe::gx2::GX2AttribFormatType::TYPE_32_32:
        return latte::SQ_DATA_FORMAT::FMT_32_32;
    case cafe::gx2::GX2AttribFormatType::TYPE_32_32_FLOAT:
        return latte::SQ_DATA_FORMAT::FMT_32_32_FLOAT;
    case cafe::gx2::GX2AttribFormatType::TYPE_16_16_16_16:
        return latte::SQ_DATA_FORMAT::FMT_16_16_16_16;
    case cafe::gx2::GX2AttribFormatType::TYPE_16_16_16_16_FLOAT:
        return latte::SQ_DATA_FORMAT::FMT_16_16_16_16_FLOAT;
    case cafe::gx2::GX2AttribFormatType::TYPE_32_32_32:
        return latte::SQ_DATA_FORMAT::FMT_32_32_32;
    case cafe::gx2::GX2AttribFormatType::TYPE_32_32_32_FLOAT:
        return latte::SQ_DATA_FORMAT::FMT_32_32_32_FLOAT;
    case cafe::gx2::GX2AttribFormatType::TYPE_32_32_32_32:
        return latte::SQ_DATA_FORMAT::FMT_32_32_32_32;
    case cafe::gx2::GX2AttribFormatType::TYPE_32_32_32_32_FLOAT:
        return latte::SQ_DATA_FORMAT::FMT_32_32_32_32_FLOAT;
    default:
        assert(!"Invalid GX2AttribFormat");
        return latte::SQ_DATA_FORMAT::FMT_INVALID;
    }
}

}

uint32_t GX2CalcFetchShaderSize(uint32_t attribCount)
{
    auto cfCount = (uint32_t)((uint32_t)((attribCount + (FetchesPerControlFlow - 1)) / FetchesPerControlFlow) + 1);
    auto cfSize = cfCount * sizeof(latte::ControlFlowInst);

    return static_cast<uint32_t>(align_up(cfSize, 0x10u) + attribCount * sizeof(latte::VertexFetchInst));
}

void GX2InitFetchShader(GX2FetchShader* fetchShader, uint8_t* buffer, uint32_t attribCount, const GX2AttribStream* attribs)
{
    // Calculate instruction pointers
    auto cfCount = (uint32_t)((uint32_t)((attribCount + (FetchesPerControlFlow - 1)) / FetchesPerControlFlow) + 1);
    auto cfSize = cfCount * sizeof(latte::ControlFlowInst);
    auto cfPtr = (latte::ControlFlowInst*)buffer;

    auto fetchOffset = align_up(cfSize, 0x10u);
    auto fetchPtr = (latte::VertexFetchInst*)(buffer + fetchOffset);

    // Setup fetch shader
    fetchShader->type = cafe::gx2::GX2FetchShaderType::NoTessellation;
    fetchShader->attribCount = attribCount;
    fetchShader->data = buffer;
    fetchShader->size = GX2CalcFetchShaderSize(attribCount);

    // Generate fetch instructions
    static const IndexMapEntry indexMap[] = {
        { 0, latte::SQ_CHAN::X },
        { 0, latte::SQ_CHAN::X },
        { 0, latte::SQ_CHAN::X },
        { 0, latte::SQ_CHAN::X },
    };

    for (auto i = 0u; i < attribCount; ++i) {
        latte::VertexFetchInst vfetch;
        auto &attrib = attribs[i];
        std::memset(&vfetch, 0, sizeof(vfetch));

        if (attrib.buffer != 16) {
            // Semantic vertex fetch
            vfetch.word0 = vfetch.word0
                .VTX_INST(latte::SQ_VTX_INST_SEMANTIC)
                .BUFFER_ID(latte::SQ_RES_OFFSET::VS_ATTRIB_RESOURCE_0 + attrib.buffer - latte::SQ_RES_OFFSET::VS_TEX_RESOURCE_0);

            vfetch.word2 = vfetch.word2
                .OFFSET(attribs[i].offset);

            vfetch.padding = static_cast<uint32_t>(attribs[i].nameIdx);

            if (attrib.type) {
                auto selX = latte::SQ_SEL::SEL_X;
                auto fetchType = latte::SQ_VTX_FETCH_TYPE::VERTEX_DATA;

                if (attrib.type == cafe::gx2::GX2AttribIndexType::PerInstance) {
                    fetchType = latte::SQ_VTX_FETCH_TYPE::INSTANCE_DATA;

                    if (attrib.aluDivisor == 1) {
                        selX = latte::SQ_SEL::SEL_W;
                    } else if (attrib.aluDivisor == fetchShader->divisors[0]) {
                        selX = latte::SQ_SEL::SEL_Y;
                    } else if (attrib.aluDivisor == fetchShader->divisors[1]) {
                        selX = latte::SQ_SEL::SEL_Z;
                    } else {
                        fetchShader->divisors[fetchShader->numDivisors] = attrib.aluDivisor;

                        if (fetchShader->numDivisors == 0) {
                            selX = latte::SQ_SEL::SEL_Y;
                        } else if (fetchShader->numDivisors == 1) {
                            selX = latte::SQ_SEL::SEL_Z;
                        }

                        fetchShader->numDivisors++;
                    }
                }

                vfetch.word0 = vfetch.word0
                    .FETCH_TYPE(fetchType)
                    .SRC_SEL_X(selX);
            } else {
                vfetch.word0 = vfetch.word0
                    .SRC_GPR(indexMap[0].gpr)
                    .SRC_SEL_X(static_cast<latte::SQ_SEL>(indexMap[0].chan));
            }

            // Setup dest
            vfetch.gpr = vfetch.gpr
                .DST_GPR(attrib.location);

            vfetch.word1 = vfetch.word1
                .DST_SEL_W(static_cast<latte::SQ_SEL>(attrib.mask & 0x7))
                .DST_SEL_Z(static_cast<latte::SQ_SEL>((attrib.mask >> 8) & 0x7))
                .DST_SEL_Y(static_cast<latte::SQ_SEL>((attrib.mask >> 16) & 0x7))
                .DST_SEL_X(static_cast<latte::SQ_SEL>((attrib.mask >> 24) & 0x7));

            // Setup mega fetch
            vfetch.word2 = vfetch.word2
                .MEGA_FETCH(1);

            vfetch.word0 = vfetch.word0
                .MEGA_FETCH_COUNT(GX2AttribFormatByteSize(attrib.format) - 1);

            // Setup format
            auto dataFormat = getAttribFormatDataFormat(attrib.format);
            auto numFormat = latte::SQ_NUM_FORMAT::NORM;
            auto formatComp = latte::SQ_FORMAT_COMP::UNSIGNED;

            if (attribs[i].format & cafe::gx2::GX2AttribFormatFlags::SCALED) {
                numFormat = latte::SQ_NUM_FORMAT::SCALED;
            } else if (attribs[i].format & cafe::gx2::GX2AttribFormatFlags::INTEGER) {
                numFormat = latte::SQ_NUM_FORMAT::INT;
            }

            if (attribs[i].format & cafe::gx2::GX2AttribFormatFlags::SIGNED) {
                formatComp = latte::SQ_FORMAT_COMP::SIGNED;
            }

            vfetch.word1 = vfetch.word1
                .DATA_FORMAT(dataFormat)
                .NUM_FORMAT_ALL(numFormat)
                .FORMAT_COMP_ALL(formatComp);

            auto swapMode = latte::SQ_ENDIAN::NONE;

            vfetch.word2 = vfetch.word2
                .ENDIAN_SWAP(swapMode);

            // Append to program
            *(fetchPtr++) = vfetch;
        }
    }

    // Generate a VTX CF per 16 VFETCH
    if (attribCount) {
        for (auto i = 0u; i < cfCount - 1; ++i) {
            auto fetches = FetchesPerControlFlow;

            if (attribCount < (i + 1) * FetchesPerControlFlow) {
                // Don't overrun our fetches!
                fetches = attribCount % FetchesPerControlFlow;
            }

            latte::ControlFlowInst inst;
            std::memset(&inst, 0, sizeof(inst));
            inst.word0 = inst.word0
                .ADDR(static_cast<uint32_t>((fetchOffset + sizeof(latte::VertexFetchInst) * i * FetchesPerControlFlow) / 8));
            inst.word1 = inst.word1
                .COUNT((fetches - 1) & 0x7)
                .COUNT_3(((fetches - 1) >> 3) & 0x1)
                .CF_INST(latte::SQ_CF_INST_VTX_TC)
                .BARRIER(0);
            *(cfPtr++) = inst;
        }
    }

    // Generate an EOP
    latte::ControlFlowInst eop;
    std::memset(&eop, 0, sizeof(eop));
    eop.word1 = eop.word1
        .BARRIER(1)
        .CF_INST(latte::SQ_CF_INST_RETURN);
    *(cfPtr++) = eop;

    // Set sq_pgm_resources_fs
    auto sq_pgm_resources_fs = fetchShader->regs.sq_pgm_resources_fs;
    sq_pgm_resources_fs = sq_pgm_resources_fs
        .NUM_GPRS(0u);
    fetchShader->regs.sq_pgm_resources_fs = sq_pgm_resources_fs;
}
