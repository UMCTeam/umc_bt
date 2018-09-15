#include <QtCore/QCoreApplication>
#include <string>
#include <sstream>
#include <iostream>
#include <exception>
#include <functional>
#include <list>
#include <map>

#include "bencoding.h"

using namespace std;

int main(int argc, char *argv[])
{
	if (argc <= 1) {
		cout << "请附加完整路径\n";
		return 0;
	}

	QCoreApplication a(argc, argv);

	BElement* metaInfo = NULL;
	try {
		BElement::Parse(argv[1], &metaInfo);
	}
	catch (exception e) {
		std::cout << "解析异常" << endl;
		return 0;
	}

	ostringstream out;
	{
		std::function<void(BElement*)> GetMetaString;
		GetMetaString = [=, &out, &GetMetaString](BElement* element) -> void {
			switch (element->m_eType)
			{
			case eElementType::e_elem_int:
			{
				out << ((BElementInt*)element)->m_n << "\n";
				break;
			}
			case eElementType::e_elem_string:
			{
				out << ((BElementString*)element)->m_str << "\n";
				break;
			}
			case eElementType::e_elem_list:
			{
				list<BElement*> m_list = ((BElementList*)element)->m_list;
				list<BElement*>::iterator iter;

				for (iter = m_list.begin(); iter != m_list.end(); iter++)
				{
					GetMetaString(*iter);
				}
				break;
			}
			case eElementType::e_elem_dict:
			{
				map<BElementString*, BElement*> m_map = ((BElementDict*)element)->m_dict;
				map<BElementString*, BElement*>::iterator iter;

				for (iter = m_map.begin(); iter != m_map.end(); iter++)
				{
					out << iter->first->m_str << "->";
					GetMetaString(iter->second);
				}
				break;
			}
			};
			
			return;
		};

		GetMetaString(metaInfo);
	}

	cout << out.str();
	delete metaInfo;

	return a.exec();
}
