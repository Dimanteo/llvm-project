add_llvm_component_library(LLVMKodaDesc
    KodaMCTargetDesc.cpp

    LINK_COMPONENTS
    MC
    KodaInfo
    Support

    ADD_TO_COMPONENT
    Koda
)