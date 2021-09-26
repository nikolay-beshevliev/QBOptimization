#pragma once
#include <string>
#include <vector>
#include <unordered_map>

typedef unsigned int uint;

enum class EColumn
{
	Column0,
	Column1,
	Column2,
	Column3
};

 /**
    Represents a Record Object
*/
struct QBRecord
{
    uint			column0; // unique id column
    std::string		column1;
    long			column2;
    std::string		column3;
};

/**
Represents a Record Collections
*/
typedef std::vector<QBRecord> QBRecordCollection;

struct QBRecordCollectionOpt
{
public:
	void					emplace_back(const QBRecord& record);
	bool					findRecord(QBRecordCollectionOpt& outRecords, EColumn column, const std::string& matchString) const;
	bool					deleteRecordByID(uint id);
	void					reserve(int containerSize);
	uint					size() const;
	void					clear();

private:
	std::unordered_map<uint, QBRecord>			fMapCol0;
	std::unordered_map<std::string, QBRecord>	fMapCol1;
	std::unordered_multimap<long, QBRecord>		fMMCol2;
	std::unordered_map<std::string, QBRecord>	fMapCol3;
};

class CPPCraftDemo
{
public:
					CPPCraftDemo();
	virtual			~CPPCraftDemo();

	static QBRecordCollection		QBFindMatchingRecords(const QBRecordCollection& records, const std::string& columnName, const std::string& matchString);
	static QBRecordCollection		QBFindMatchingRecords(const QBRecordCollection& records, EColumn column, const std::string& matchString);
	static QBRecordCollection		PopulateDummyData(const std::string& prefix, int numRecords);
	static QBRecordCollectionOpt	PopulateDummyDataOpt(const std::string& prefix, int numRecords);
};