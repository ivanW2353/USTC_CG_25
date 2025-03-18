#pragma once
#include <filesystem>

#include "RHI/api.h"
#include "slang.h"

USTC_CG_NAMESPACE_OPEN_SCOPE
class SlangShaderCompiler {
   public:
    static std::filesystem::path find_root(const std::filesystem::path& p);

    static void save_file(const std::string& filename, const char* data);

    static SlangResult addHLSLPrelude(slang::IGlobalSession* session);
    static SlangResult addHLSLHeaderInclude(SlangCompileRequest* slangRequest);
    static SlangResult addHLSLSupportPreDefine(
        SlangCompileRequest* slangRequest);

    static SlangResult addCPPPrelude(slang::IGlobalSession* session);
    static SlangResult addCPPHeaderInclude(SlangCompileRequest* slangRequest);
#if USTC_CG_WITH_CUDA
    static SlangResult addCUDAPrelude(slang::IGlobalSession* session);

    static SlangResult addOptiXHeaderInclude(SlangCompileRequest* slangRequest);
    static SlangResult addOptiXSupportPreDefine(
        SlangCompileRequest* slangRequest);
    static SlangResult addOptiXSupport(SlangCompileRequest* slangRequest);
#endif
};

USTC_CG_NAMESPACE_CLOSE_SCOPE
