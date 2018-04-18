#ifndef DCS_HISTORY_DETAIL_H_
#define DCS_HISTORY_DETAIL_H_

#include <string>

typedef struct
{
	std::string operate;             // ����
	std::string history_operator;    // ������
	std::string update_time;         // ����ʱ��
	std::string comment;             // ��ע
	std::string update_content;      // ��������
} DcsHistoryDetail;

typedef std::tr1::shared_ptr<DcsHistoryDetail> DcsHistoryDetailPtr;
typedef std::tr1::shared_ptr<std::vector<DcsHistoryDetailPtr> > DcsHistoryDetailVecPtr;

#endif
