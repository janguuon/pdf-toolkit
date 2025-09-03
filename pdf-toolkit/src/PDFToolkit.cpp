#include "PDFToolkit.h"
#include <iostream>
#include <sstream>
#include <windows.h>

// 전역 context를 사용하여 생명주기 문제 해결
static fz_context* g_ctx = nullptr;
static int g_ref_count = 0;

static void init_global_context() {
    if (g_ref_count == 0) {
        g_ctx = fz_new_context(NULL, NULL, FZ_STORE_UNLIMITED);
        if (g_ctx) {
            fz_register_document_handlers(g_ctx);
        }
    }
    g_ref_count++;
}

static void cleanup_global_context() {
    g_ref_count--;
    if (g_ref_count == 0 && g_ctx) {
        fz_drop_context(g_ctx);
        g_ctx = nullptr;
    }
}

PDFToolkit::PDFToolkit() : ctx(nullptr) {
    init_global_context();
    ctx = g_ctx;
}

PDFToolkit::~PDFToolkit() {
    cleanup_global_context();
    ctx = nullptr;
}

PDFToolkit::PDFToolkit(PDFToolkit&& other) noexcept : ctx(other.ctx) {
    other.ctx = nullptr;
    if (ctx) {
        g_ref_count++;
    }
}

PDFToolkit& PDFToolkit::operator=(PDFToolkit&& other) noexcept {
    if (this != &other) {
        cleanup_global_context();
        ctx = other.ctx;
        other.ctx = nullptr;
        if (ctx) {
            g_ref_count++;
        }
    }
    return *this;
}

bool PDFToolkit::mergePDFs(const std::vector<std::string>& inputFiles, const std::string& outputFile) {
    if (!ctx) {
        std::cout << "MuPDF context가 초기화되지 않았습니다." << std::endl;
        return false;
    }
    
    if (inputFiles.size() < 2) {
        std::cout << "병합하려면 최소 2개의 PDF 파일이 필요합니다." << std::endl;
        return false;
    }

    pdf_document* output_doc = nullptr;
    
    output_doc = pdf_create_document(ctx);
    if (!output_doc) {
        std::cout << "출력 문서 생성 실패" << std::endl;
        return false;
    }

    for (const auto& inputFile : inputFiles) {
        fz_document* input_doc = fz_open_document(ctx, inputFile.c_str());
        if (!input_doc) {
            std::cout << "파일 열기 실패: " << inputFile.c_str() << std::endl;
            continue;
        }

        pdf_document* input_pdf = pdf_specifics(ctx, input_doc);
        if (!input_pdf) {
            std::cout << "파일이 유효한 PDF가 아닙니다: " << inputFile.c_str() << std::endl;
            fz_drop_document(ctx, input_doc);
            continue;
        }

        pdf_graft_map* map = pdf_new_graft_map(ctx, output_doc);
        if (!map) {
            std::cout << "Graft map 생성 실패" << std::endl;
            fz_drop_document(ctx, input_doc);
            continue;
        }

        int page_count = fz_count_pages(ctx, input_doc);
        for (int i = 0; i < page_count; i++) {
            pdf_graft_mapped_page(ctx, map, -1, input_pdf, i);
        }

        pdf_drop_graft_map(ctx, map);
        fz_drop_document(ctx, input_doc);
    }

    pdf_save_document(ctx, output_doc, outputFile.c_str(), NULL);
    pdf_drop_document(ctx, output_doc);

    std::cout << "PDF 병합 완료: " << outputFile.c_str() << std::endl;
    return true;
}

bool PDFToolkit::deletePages(const std::string& inputFile, const std::string& outputFile,
    int startPage, int endPage) {
    if (!ctx) {
        std::cout << "MuPDF context가 초기화되지 않았습니다." << std::endl;
        return false;
    }
    
    fz_document* doc = fz_open_document(ctx, inputFile.c_str());
    if (!doc) {
        std::cout << "파일 열기 실패: " << inputFile.c_str() << std::endl;
        return false;
    }

    pdf_document* pdf = pdf_specifics(ctx, doc);
    if (!pdf) {
        std::cout << "파일이 유효한 PDF가 아닙니다: " << inputFile.c_str() << std::endl;
        fz_drop_document(ctx, doc);
        return false;
    }

    int total_pages = fz_count_pages(ctx, doc);

    startPage = startPage - 1;
    if (endPage == -1) endPage = startPage + 1;
    else endPage = endPage;

    if (startPage < 0 || endPage > total_pages || startPage >= endPage) {
        std::cout << "잘못된 페이지 범위입니다." << std::endl;
        fz_drop_document(ctx, doc);
        return false;
    }

    pdf_document* output_doc = pdf_create_document(ctx);
    if (!output_doc) {
        std::cout << "출력 문서 생성 실패" << std::endl;
        fz_drop_document(ctx, doc);
        return false;
    }

    pdf_graft_map* map = pdf_new_graft_map(ctx, output_doc);
    if (!map) {
        std::cout << "Graft map 생성 실패" << std::endl;
        pdf_drop_document(ctx, output_doc);
        fz_drop_document(ctx, doc);
        return false;
    }

    for (int i = 0; i < startPage; i++) {
        pdf_graft_mapped_page(ctx, map, -1, pdf, i);
    }

    for (int i = endPage; i < total_pages; i++) {
        pdf_graft_mapped_page(ctx, map, -1, pdf, i);
    }

    pdf_drop_graft_map(ctx, map);
    pdf_save_document(ctx, output_doc, outputFile.c_str(), NULL);
    pdf_drop_document(ctx, output_doc);
    fz_drop_document(ctx, doc);

    std::cout << "페이지 삭제 완료: " << outputFile.c_str() << std::endl;
    return true;
}

bool PDFToolkit::splitPDF(const std::string& inputFile, const std::string& outputPrefix) {
    if (!ctx) {
        std::cout << "MuPDF context가 초기화되지 않았습니다." << std::endl;
        return false;
    }
    
    fz_document* doc = fz_open_document(ctx, inputFile.c_str());
    if (!doc) {
        std::cout << "파일 열기 실패: " << inputFile.c_str() << std::endl;
        return false;
    }

    pdf_document* pdf = pdf_specifics(ctx, doc);
    if (!pdf) {
        std::cout << "파일이 유효한 PDF가 아닙니다: " << inputFile.c_str() << std::endl;
        fz_drop_document(ctx, doc);
        return false;
    }

    int total_pages = fz_count_pages(ctx, doc);

    for (int i = 0; i < total_pages; i++) {
        pdf_document* output_doc = pdf_create_document(ctx);
        if (!output_doc) {
            std::cout << "출력 문서 생성 실패" << std::endl;
            continue;
        }

        pdf_graft_map* map = pdf_new_graft_map(ctx, output_doc);
        if (!map) {
            std::cout << "Graft map 생성 실패" << std::endl;
            pdf_drop_document(ctx, output_doc);
            continue;
        }

        pdf_graft_mapped_page(ctx, map, -1, pdf, i);

        std::stringstream ss;
        ss << outputPrefix << "_page_" << (i + 1) << ".pdf";
        std::string output_filename = ss.str();

        pdf_drop_graft_map(ctx, map);
        pdf_save_document(ctx, output_doc, output_filename.c_str(), NULL);
        pdf_drop_document(ctx, output_doc);

        std::cout << "페이지 " << (i + 1) << " 저장: " << output_filename.c_str() << std::endl;
    }

    fz_drop_document(ctx, doc);
    std::cout << "PDF 분할 완료. 총 " << total_pages << "개 파일 생성" << std::endl;
    return true;
}

int PDFToolkit::getPageCount(const std::string& filePath) {
    if (!ctx) {
        return -1;
    }
    
    fz_document* doc = fz_open_document(ctx, filePath.c_str());
    if (!doc) {
        return -1;
    }

    int page_count = fz_count_pages(ctx, doc);
    fz_drop_document(ctx, doc);
    return page_count;
}

bool PDFToolkit::isValidPDF(const std::string& filePath) {
    if (!ctx) {
        return false;
    }
    
    fz_document* doc = fz_open_document(ctx, filePath.c_str());
    if (!doc) {
        return false;
    }

    pdf_document* pdf = pdf_specifics(ctx, doc);
    bool is_valid = (pdf != nullptr);
    fz_drop_document(ctx, doc);
    return is_valid;
}
