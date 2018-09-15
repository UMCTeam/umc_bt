#include "bencoding.h"
#include <fstream>
#include <memory>

std::vector<BElement *> BElement::GLOBAL_ENV = std::vector<BElement *>();

BElement* BElement::CreateElement(eElementType eType)
{
	switch (eType)
	{
		case e_elem_int:
		return (new BElementInt);
		
		case e_elem_string:
		return (new BElementString);
		
		case e_elem_list:
		return (new BElementList);
		
		case e_elem_dict:
		return (new BElementDict);
		
		default:
		return NULL;
	}
}

eElementType BElement::PeekType(std::ifstream& ifs)
{
	int c = ifs.peek();
	if (isdigit(c))
		return e_elem_string;
	else if (c == 'i')
		return e_elem_int;
	else if (c == 'l')
		return e_elem_list;
	else if (c == 'd')
		return e_elem_dict;
	else
		return e_elem_unknown;
}

BElement* BElement::Peek(std::ifstream& ifs)
{
	return CreateElement(PeekType(ifs));
}

std::string BElement::Parse(const std::string& file_path, BElement** virtual_root)
{
	*virtual_root = NULL;
	GLOBAL_ENV.clear();
	
	// open file
	std::ifstream ifs;
	ifs.open(file_path.c_str(), std::ios_base::in | std::ios_base::binary/* | std::ios::nocreate*/);
	if (!ifs.good())
		return "open file error";	
	
	std::auto_ptr<BElementList> auto_root(new BElementList);
	GLOBAL_ENV.push_back(auto_root.get());
	int c = ifs.peek();
	while (c != EOF)
	{
		BElement *pElem = Peek(ifs);
		pElem->Construct(ifs);
		c = ifs.peek();
	}
	GLOBAL_ENV.pop_back();
	
	*virtual_root = auto_root->m_list.front();
	auto_root->m_list.clear();
	
	return "解析成功，记得释放内存哦亲";
}

void BElementInt::Construct(std::ifstream& ifs)
{
	m_parent = GLOBAL_ENV.back();
	if (m_parent->m_eType == e_elem_list)
		((BElementList *)m_parent)->m_list.push_back(this);
	else if (m_parent->m_eType == e_elem_string)
	{
		((BElementDict *)m_parent->m_parent)->m_dict.insert(std::make_pair((BElementString *)m_parent, this));
		m_parent = m_parent->m_parent;
		GLOBAL_ENV.pop_back();
	}
	else
		;// invalid bt file
	
	ifs.get();	// skip 'i' character
	ifs >> m_n;
	ifs.get();	// skip 'e' character
}

void BElementString::Construct(std::ifstream& ifs)
{
	m_parent = GLOBAL_ENV.back();
	if (m_parent->m_eType == e_elem_list)
		((BElementList *)m_parent)->m_list.push_back(this);
	else if (m_parent->m_eType == e_elem_dict)
		GLOBAL_ENV.push_back(this);
	else if (m_parent->m_eType == e_elem_string)
	{
		((BElementDict *)m_parent->m_parent)->m_dict.insert(std::make_pair((BElementString *)m_parent, this));
		m_parent = m_parent->m_parent;
		GLOBAL_ENV.pop_back();
	}
	else
		;// invalid bt file;
	
	ifs >> m_nLen;
	
	ifs.get();	// skip ':' character
	
	m_str = new char[ m_nLen ];
	ifs.read(m_str, m_nLen);
}

void BElementList::Construct(std::ifstream& ifs)
{
	m_parent = GLOBAL_ENV.back();
	if (m_parent->m_eType == e_elem_list)
		((BElementList *)m_parent)->m_list.push_back(this);
	else if (m_parent->m_eType == e_elem_string)
	{
		((BElementDict *)m_parent->m_parent)->m_dict.insert(std::make_pair((BElementString *)m_parent, this));
		m_parent = m_parent->m_parent;
		GLOBAL_ENV.pop_back();
	}
	else
		;// invalid bt file
		
	GLOBAL_ENV.push_back(this);
	
	ifs.get();	// skip 'l' character
	
	int c = ifs.peek();
	while (c != 'e')
	{
		BElement *pElem = Peek(ifs);
		pElem->Construct(ifs);
		c = ifs.peek();
	}
	
	ifs.get();	// skip 'e' character
	
	GLOBAL_ENV.pop_back();
}

void BElementDict::Construct(std::ifstream& ifs)
{
	m_parent = GLOBAL_ENV.back();
	if (m_parent->m_eType == e_elem_list)
		((BElementList *)m_parent)->m_list.push_back(this);
	else if (m_parent->m_eType == e_elem_string)
	{
		((BElementDict *)m_parent->m_parent)->m_dict.insert(std::make_pair((BElementString *)m_parent, this));
		m_parent = m_parent->m_parent;
		GLOBAL_ENV.pop_back();
	}
	else
		;//	invalid bt file
		
	GLOBAL_ENV.push_back(this);
	
	ifs.get();	// skip 'd' character
	
	int c = ifs.peek();
	while (c != 'e')
	{
		BElement *pElem = Peek(ifs);
		pElem->Construct(ifs);
		c = ifs.peek();
	}
	
	ifs.get();	// skip 'e' character
	
	GLOBAL_ENV.pop_back();
}
#include <iostream>
void BElementString::Destruct()
{
	delete [] m_str;
	m_str = NULL;
}

void BElementList::Destruct()
{
	std::list<BElement *>::iterator elem_iter = m_list.begin();
	for (; elem_iter != m_list.end(); elem_iter++)
	{
		delete (*elem_iter);
	}
}
void BElementDict::Destruct()
{
	std::map<BElementString *, BElement *>::iterator elem_iter = m_dict.begin();
	for (; elem_iter != m_dict.end(); elem_iter++)
	{
		delete elem_iter->first;
		delete elem_iter->second;
	}
}
