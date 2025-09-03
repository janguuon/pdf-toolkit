#pragma once

#include <string>
#include <vector>
#include <stdexcept>
#include "mupdf/fitz.h"
#include "mupdf/pdf.h"

/**
 * @brief MuPDF ���̺귯���� ����� PDF ���� ���� Ŭ����
 *
 * �� Ŭ������ ���� ��ɵ��� �����մϴ�:
 * - PDF ���� ����
 * - PDF Ư�� ������ ����
 * - PDF �������� ����
 */
class PDFToolkit {
private:
    fz_context* ctx;  ///< MuPDF ���ؽ�Ʈ

    /**
     * @brief MuPDF ���ؽ�Ʈ �ʱ�ȭ
     * @throws std::runtime_error ���ؽ�Ʈ ���� ���� ��
     */
    void initializeContext();

    /**
     * @brief ���ҽ� ����
     */
    void cleanup();

public:
    /**
     * @brief ������ - MuPDF ���ؽ�Ʈ�� �ʱ�ȭ�մϴ�
     * @throws std::runtime_error MuPDF ���ؽ�Ʈ ���� ���� ��
     */
    PDFToolkit();

    /**
     * @brief �Ҹ��� - ���ҽ��� �����մϴ�
     */
    ~PDFToolkit();

    /**
     * @brief ���� ������ (������)
     */
    PDFToolkit(const PDFToolkit&) = delete;

    /**
     * @brief ���� ���� ������ (������)
     */
    PDFToolkit& operator=(const PDFToolkit&) = delete;

    /**
     * @brief �̵� ������
     */
    PDFToolkit(PDFToolkit&& other) noexcept;

    /**
     * @brief �̵� ���� ������
     */
    PDFToolkit& operator=(PDFToolkit&& other) noexcept;

    /**
     * @brief ���� PDF ������ �ϳ��� �����մϴ�
     * @param inputFiles ������ PDF ���ϵ��� ��� (�ּ� 2�� �ʿ�)
     * @param outputFile ��� PDF ���� ���
     * @return ���� �� true, ���� �� false
     */
    bool mergePDFs(const std::vector<std::string>& inputFiles, const std::string& outputFile);

    /**
     * @brief PDF���� Ư�� ������(��)�� �����մϴ�
     * @param inputFile �Է� PDF ���� ���
     * @param outputFile ��� PDF ���� ���
     * @param startPage ������ ���� ������ (1-based)
     * @param endPage ������ �� ������ (1-based, -1�̸� startPage�� ����)
     * @return ���� �� true, ���� �� false
     */
    bool deletePages(const std::string& inputFile, const std::string& outputFile,
        int startPage, int endPage = -1);

    /**
     * @brief PDF�� �� ���������� �����մϴ�
     * @param inputFile �Է� PDF ���� ���
     * @param outputPrefix ��� ���ϵ��� ���λ� (��: "output" -> "output_page_1.pdf")
     * @return ���� �� true, ���� �� false
     */
    bool splitPDF(const std::string& inputFile, const std::string& outputPrefix);

    /**
     * @brief PDF ������ ������ ���� ��ȯ�մϴ�
     * @param filePath PDF ���� ���
     * @return ������ �� (���� �� -1)
     */
    int getPageCount(const std::string& filePath);

    /**
     * @brief PDF ������ ��ȿ���� Ȯ���մϴ�
     * @param filePath PDF ���� ���
     * @return ��ȿ�� PDF �����̸� true, �ƴϸ� false
     */
    bool isValidPDF(const std::string& filePath);
};
