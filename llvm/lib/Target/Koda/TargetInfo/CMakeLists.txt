add_llvm_component_library(LLVMKodaInfo
    KodaTargetInfo.cpp

    LINK_COMPONENTS
    Support

    ADD_TO_COMPONENT
    Koda
)