#include "utility/json/json.h"
//#include "utility/common/dsb_log.h"

namespace Json
{
	void ExportLib()
	{
// 		return;

		std::string strValue = "{\"key1\":\"value1\",\"array\":[{\"key2\":\"value2\"},{\"key2\":\"value3\"},{\"key2\":\"value4\"}]}";
		Json::Reader reader;
		Json::Value value;
		if (reader.parse(strValue, value))
		{
			std::string out = value["key1"].asString();
			std::cout << out << std::endl;
			const Json::Value arrayObj = value["array"];
			for (int i=0; i<arrayObj.size(); i++)
			{
				out = arrayObj[i]["key2"].asString();
				std::cout << out;
				if (i != arrayObj.size() - 1)
					std::cout << std::endl;
			}//end for
		}//end if
	}
}
