#pragma once

#include <string>
#include <vector>
#include <stdexcept>
#include "mupdf/fitz.h"
#include "mupdf/pdf.h"

/**
 * @brief MuPDF 라이브러리를 사용한 PDF 조작 도구 클래스
 *
 * 이 클래스는 다음 기능들을 제공합니다:
 * - PDF 파일 병합
 * - PDF 특정 페이지 삭제
 * - PDF 페이지별 분할
 */
class PDFToolkit {
private:
    fz_context* ctx;  ///< MuPDF 컨텍스트

    /**
     * @brief MuPDF 컨텍스트 초기화
     * @throws std::runtime_error 컨텍스트 생성 실패 시
     */
    void initializeContext();

    /**
     * @brief 리소스 정리
     */
    void cleanup();

public:
    /**
     * @brief 생성자 - MuPDF 컨텍스트를 초기화합니다
     * @throws std::runtime_error MuPDF 컨텍스트 생성 실패 시
     */
    PDFToolkit();

    /**
     * @brief 소멸자 - 리소스를 정리합니다
     */
    ~PDFToolkit();

    /**
     * @brief 복사 생성자 (삭제됨)
     */
    PDFToolkit(const PDFToolkit&) = delete;

    /**
     * @brief 복사 대입 연산자 (삭제됨)
     */
    PDFToolkit& operator=(const PDFToolkit&) = delete;

    /**
     * @brief 이동 생성자
     */
    PDFToolkit(PDFToolkit&& other) noexcept;

    /**
     * @brief 이동 대입 연산자
     */
    PDFToolkit& operator=(PDFToolkit&& other) noexcept;

    /**
     * @brief 여러 PDF 파일을 하나로 병합합니다
     * @param inputFiles 병합할 PDF 파일들의 경로 (최소 2개 필요)
     * @param outputFile 출력 PDF 파일 경로
     * @return 성공 시 true, 실패 시 false
     */
    bool mergePDFs(const std::vector<std::string>& inputFiles, const std::string& outputFile);

    /**
     * @brief PDF에서 특정 페이지(들)을 삭제합니다
     * @param inputFile 입력 PDF 파일 경로
     * @param outputFile 출력 PDF 파일 경로
     * @param startPage 삭제할 시작 페이지 (1-based)
     * @param endPage 삭제할 끝 페이지 (1-based, -1이면 startPage만 삭제)
     * @return 성공 시 true, 실패 시 false
     */
    bool deletePages(const std::string& inputFile, const std::string& outputFile,
        int startPage, int endPage = -1);

    /**
     * @brief PDF를 각 페이지별로 분할합니다
     * @param inputFile 입력 PDF 파일 경로
     * @param outputPrefix 출력 파일들의 접두사 (예: "output" -> "output_page_1.pdf")
     * @return 성공 시 true, 실패 시 false
     */
    bool splitPDF(const std::string& inputFile, const std::string& outputPrefix);

    /**
     * @brief PDF 파일의 페이지 수를 반환합니다
     * @param filePath PDF 파일 경로
     * @return 페이지 수 (실패 시 -1)
     */
    int getPageCount(const std::string& filePath);

    /**
     * @brief PDF 파일이 유효한지 확인합니다
     * @param filePath PDF 파일 경로
     * @return 유효한 PDF 파일이면 true, 아니면 false
     */
    bool isValidPDF(const std::string& filePath);
};
