#ifndef _EXCELENGINE_H_
#define _EXCELENGINE_H_

#include <stdio.h>

namespace GenerateExcels
{
#define GENERATE_SUCCESS 0
#define ERR_OPEN_FAILED 1
#define ERR_WRITE_HEAD_FAILED 2
#define ERR_CLOSE_FILE_ERROR 3
#define ERR_BUFFER_INVALID 4

#define CHECK_BUFFER if(!m_xls)\
	return

#define CHECK_BUFFER_WITH_RETURN if(!m_xls)\
	return ERR_BUFFER_INVALID

#define SUCCESS_EXCEL 0
#define FAILED_EXCEL 1

#define FILE_HTML_TITLE "<html xmlns:v=\"urn:schemas-microsoft-com:vml\"\n\
xmlns:o=\"urn:schemas-microsoft-com:office:office\"\n\
xmlns:x=\"urn:schemas-microsoft-com:office:excel\"\n\
xmlns=\"\">\n"

#define HEAD_TEXT_FILE "./excel/excel.config"
#define FILE_BODY_TITLE "\n\n<body link=blue vlink=purple>\n\n"
#define FILE_TABLE_TITLE "<table x:str border=0 cellpadding=0 cellspacing=0 width=1296 style=\'border-collapse:collapse;table-layout:fixed;width:972pt\'>\n"

#define CELL_NORMAL(font,border,prop) "  <td class=xl"#font" style=\'"border"\' "prop">\n"
#define CELL_COLSPAN(col,font,border,prop) "  <td colspan="#col" class=xl"#font"  style=\'"border"\' "prop">\n"
#define CELL_ROWSPAN(row,font,border,prop) "  <td rowspan="#row" class=xl"#font"  style=\'"border"\' "prop">\n"
#define CELL_SPAN(col,row,font,border,prop) "  <td colspan="#col" rowspan="#row" class=xl"#font" style=\'"border"\' "prop">\n"

#define BDR_SOLID(weight) "border-bottom:"#weight"pt solid black;border-top:none;"
#define BDR_DOUBLE(weight) "border-bottom:"#weight"pt double black;border-top:none;"

class ExcelEngine
{
public:
	ExcelEngine(void);
	~ExcelEngine(void);

private:
	ExcelEngine(const ExcelEngine &c);
	ExcelEngine &operator=(const ExcelEngine &c);

public:
	int CreateExeclFile(const char * file_name);
	int EndExeclFile();

	int LoadXlsHead();
	int CopyExcelText(FILE *pSource, FILE *pDest);

	void SetColumnWidth(int span_cols, int width_pels);

	void StartNewLine(float height);
	void EndCurrLine();

	void FillExcelCell(const char* cell_tag, int val);
	void FillExcelCell(const char* cell_tag, float val);
	void FillExcelCell(const char* cell_tag, const char* val);	

protected:
    FILE* m_xls;
    void Release();	
};

}
#endif // _EXCELENGINE_H_

