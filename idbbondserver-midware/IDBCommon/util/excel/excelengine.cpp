#include "excelengine.h"

using namespace GenerateExcels;

ExcelEngine::ExcelEngine(void)
{
	m_xls = NULL;
}

ExcelEngine::~ExcelEngine(void)
{	
    Release();
}

ExcelEngine::ExcelEngine(const ExcelEngine &r)
{
}

ExcelEngine &ExcelEngine::operator=(const ExcelEngine &r)
{
	return *this;
}

void ExcelEngine::Release()
{
	if (m_xls)
	{
		fclose(m_xls);
		m_xls = NULL;
	}
}

int ExcelEngine::CreateExeclFile(const char * file_name)
{
	Release();
	m_xls = fopen(file_name, "w+");
    
	if (!m_xls)
		return ERR_OPEN_FAILED;

	fprintf(m_xls, FILE_HTML_TITLE);

	if (LoadXlsHead())
		return ERR_WRITE_HEAD_FAILED;	

	fprintf(m_xls, FILE_BODY_TITLE);
	fprintf(m_xls, FILE_TABLE_TITLE);

	return SUCCESS_EXCEL;
}

int ExcelEngine::LoadXlsHead()
{
    CHECK_BUFFER_WITH_RETURN;
    FILE *p_head = fopen(HEAD_TEXT_FILE, "r");

    if (!p_head)
        return ERR_WRITE_HEAD_FAILED;

    CopyExcelText(p_head, m_xls);
    fclose(p_head);

	return SUCCESS_EXCEL;
}

int ExcelEngine::EndExeclFile()
{
	CHECK_BUFFER_WITH_RETURN;
	fprintf(m_xls, "</table>\n");
	fprintf(m_xls, "</body>\n");
	fprintf(m_xls, "</html>\n");

	Release();
	return SUCCESS_EXCEL;
}

void ExcelEngine::SetColumnWidth(int span_cols, int width_pels)
{
	CHECK_BUFFER;

	if (span_cols>1)
		fprintf(m_xls, " <col width=%d span=%d style=\'mso-width-source:userset;mso-width-alt:%d\'>\n", width_pels, span_cols, width_pels*32);
	else
		fprintf(m_xls, " <col width=%d style=\'mso-width-source:userset;mso-width-alt:%d\'>\n", width_pels, width_pels*32);		
}

void ExcelEngine::StartNewLine(float height)
{
	CHECK_BUFFER;
	fprintf(m_xls, " <tr style=\'mso-height-source:userset;height:%.2fpt\'>\n", height);
}

void ExcelEngine::EndCurrLine()
{
	CHECK_BUFFER;	
	fprintf(m_xls, " </tr>\n");	
}

void ExcelEngine::FillExcelCell(const char* cell_tag, int val)
{
	CHECK_BUFFER;
	fprintf(m_xls, "%s%d</td>\n", cell_tag, val);
}
void ExcelEngine::FillExcelCell(const char* cell_tag, float val)
{
	CHECK_BUFFER;
	fprintf(m_xls, "%s%f</td>\n", cell_tag, val);
}
void ExcelEngine::FillExcelCell(const char* cell_tag, const char* val)
{
	CHECK_BUFFER;
	fprintf(m_xls, "%s%s</td>\n", cell_tag, val);
}

int ExcelEngine::CopyExcelText(FILE *pSource, FILE *pDest)
{
	if (!pSource && !pDest)
		return FAILED_EXCEL;

	char c;	
	while (!feof(pSource))
	{
		c = fgetc(pSource);
		fputc(c, pDest);
	}

	if(c == -1)	
	{
		fseek(pDest, -1L, SEEK_CUR );	
		fputc('\n', pDest);
	}

	return SUCCESS_EXCEL;
}






