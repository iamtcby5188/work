#ifndef DCS_HISTORY_DETAIL_H_
#define DCS_HISTORY_DETAIL_H_

#include <string>

typedef struct
{
	std::string operate;             // 操作
	std::string history_operator;    // 操作人
	std::string update_time;         // 操作时间
	std::string comment;             // 备注
	std::string update_content;      // 更改内容
} DcsHistoryDetail;

typedef std::tr1::shared_ptr<DcsHistoryDetail> DcsHistoryDetailPtr;
typedef std::tr1::shared_ptr<std::vector<DcsHistoryDetailPtr> > DcsHistoryDetailVecPtr;

#endif
