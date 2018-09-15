#ifndef BENCODING_H
#define BENCODING_H

#include <vector>
#include <list>
#include <map>

enum eElementType
{
	e_elem_int,
	e_elem_string,
	e_elem_list,
	e_elem_dict,
	e_elem_unknown
};

struct BElement
{
	eElementType m_eType;
	BElement *m_parent;
	
	std::string static Parse(const std::string& file_path, BElement** virtual_root);
	
	static std::vector<BElement *> GLOBAL_ENV;
	static BElement* CreateElement(eElementType eType);
	static eElementType PeekType(std::ifstream& ifs);
	static BElement* Peek(std::ifstream& ifs);
	
	virtual void Construct(std::ifstream& ifs) {}
	virtual void Destruct() {}
	
	virtual ~BElement() {}
};

struct BElementInt : public BElement
{
	__int64 m_n;

	BElementInt() { m_eType = e_elem_int; }
	
	virtual void Construct(std::ifstream& ifs);
};

struct BElementString : public BElement
{
	__int64 m_nLen;
	char *m_str;

	BElementString() { m_eType = e_elem_string; m_str = NULL; }
	
	virtual void Construct(std::ifstream& ifs);
	virtual void Destruct();
	
	virtual	~BElementString() { Destruct(); }
};

struct BElementList : public BElement
{
	std::list<BElement *> m_list;

	BElementList() { m_eType = e_elem_list; }
	
	virtual void Construct(std::ifstream& ifs);
	virtual void Destruct();

	virtual	~BElementList() { Destruct(); }
};

struct BElementDict : public BElement
{
	std::map<BElementString *, BElement *> m_dict;

	BElementDict() { m_eType = e_elem_dict; }
	
	virtual void Construct(std::ifstream& ifs);
	virtual void Destruct();
	
	virtual	~BElementDict() { Destruct(); }
};

#endif // BENCODING_H
