#include "stdafx.h"
#include <algorithm>
#include "CPPCraftDemo.h"
#include <assert.h>

using namespace std;

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------	
void QBRecordCollectionOpt::emplace_back(const QBRecord& record)
{
	fMapCol0[ record.column0 ]	= record;
	fMapCol1[ record.column1 ]	= record;
	fMMCol2.insert( std::pair<long, QBRecord>(record.column2, record) );
	fMapCol3[ record.column3 ]	= record;
}

bool QBRecordCollectionOpt::findRecord(QBRecordCollectionOpt& outRecords, EColumn column, const std::string& matchString) const
{
	outRecords.clear();
	bool	bOk		= false;

	switch (column)
	{
	case EColumn::Column0:
	{
		auto itRec	= fMapCol0.find( std::stoul(matchString) );
		if ( itRec != fMapCol0.end() )
		{
			outRecords.emplace_back( itRec->second );
			bOk		= true;
		}

		break;
	}

	case EColumn::Column1:
	{
		auto itRec	= fMapCol1.find( matchString );
		if ( itRec != fMapCol1.end() )
		{
			outRecords.emplace_back( itRec->second );
			bOk		= true;
		}

		break;
	}

	case EColumn::Column2:
	{
		auto	eqRange		= fMMCol2.equal_range( std::stol(matchString) );
		for ( auto itRec = eqRange.first; itRec != eqRange.second; itRec++ )
		{
			outRecords.emplace_back( itRec->second );
			bOk		= true;
		}

		break;
	}

	case EColumn::Column3:
	{
		auto	itRec		= fMapCol3.find( matchString );
		if (itRec != fMapCol3.end())
		{
			outRecords.emplace_back(itRec->second);
			bOk		= true;
		}

		break;
	}

	default:
		break;
	}

	return bOk;
}

bool QBRecordCollectionOpt::deleteRecordByID(uint id)
{
	bool	bDeleted	= false;
	auto	itRecord	= fMapCol0.find( id );
	if ( itRecord != fMapCol0.end() )
	{
		fMapCol1.erase( itRecord->second.column1 );
		fMapCol3.erase( itRecord->second.column3 );

		auto	eqRange = fMMCol2.equal_range(itRecord->second.column2);
		for ( auto itCol2Rec = eqRange.first; itCol2Rec != eqRange.second; itCol2Rec++ )
		{
			if (itCol2Rec->second.column0 == id)
			{
				fMMCol2.erase( itCol2Rec );
				break;
			}
		}

		fMapCol0.erase( itRecord );

		bDeleted	= true;
		assert( (fMapCol0.size() == fMapCol1.size()) == (fMMCol2.size() == fMapCol3.size()) );
	}

	return bDeleted;
}

void QBRecordCollectionOpt::reserve(int containerSize)
{
	fMapCol0.reserve( containerSize );
	fMapCol1.reserve( containerSize );
	fMMCol2.reserve( containerSize );
	fMapCol3.reserve( containerSize );
}

uint QBRecordCollectionOpt::size() const
{
	assert( fMapCol0.size() == fMapCol1.size() == fMMCol2.size() == fMapCol3.size() );
	return fMapCol0.size();
}

void QBRecordCollectionOpt::clear()
{
	fMapCol0.clear();
	fMapCol1.clear();
	fMMCol2.clear();
	fMapCol3.clear();
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CPPCraftDemo::CPPCraftDemo()
{}

CPPCraftDemo::~CPPCraftDemo()
{}

/**
    Return records that contains a string in the StringValue field
    records - the initial set of records to filter
    matchString - the string to search for
*/
QBRecordCollection CPPCraftDemo::QBFindMatchingRecords(const QBRecordCollection& records, const std::string& columnName, const std::string& matchString)
{
	QBRecordCollection result;
	std::copy_if(records.begin(), records.end(), std::back_inserter(result), [&](QBRecord rec){
		if ( columnName == "column0" ) {
			uint matchValue = std::stoul(matchString);
			return matchValue == rec.column0;
		}
		else if ( columnName == "column1" ) {
			return rec.column1.find(matchString) != std::string::npos;
		}
		else if ( columnName == "column2" ) {
			long matchValue = std::stol(matchString);
			return matchValue == rec.column2;
		}
		else if ( columnName == "column3" ) {
			return rec.column3.find(matchString) != std::string::npos;
		}
		else {
			return false;
		}
	});

	return result;
}

QBRecordCollection CPPCraftDemo::QBFindMatchingRecords(const QBRecordCollection& records, EColumn column, const std::string& matchString)
{
	bool	bMatched	= false;
	QBRecordCollection result;
	for ( const QBRecord& record : records )
	{
		switch (column)
		{
		case EColumn::Column0:
		{
			uint matchValue = std::stoul(matchString);
			if ( matchValue == record.column0 )
			{
				result.emplace_back( record );
				bMatched		= true;
			}
			break;
		}
		case EColumn::Column1:
		{
			if ( record.column1.compare(matchString) == 0 )
			{
				result.emplace_back( record );
				bMatched		= true;
			}
			break;
		}
		case EColumn::Column2:
		{
			long matchValue = std::stol(matchString);
			if (matchValue == record.column2)
			{
				result.emplace_back( record );
			}
			break;
		}
		case EColumn::Column3:
		{
			if ( record.column3.compare(matchString) == 0 )
			{
				result.emplace_back( record );
				bMatched		= true;
			}
			break;
		}
		default:
			break;
		}
		
		if (bMatched)
			break;
	}

	return result;
}

/**
    Utility to populate a record collection
    prefix - prefix for the string value for every record
    numRecords - number of records to populate in the collection
*/
QBRecordCollection CPPCraftDemo::PopulateDummyData(const std::string& prefix, int numRecords)
{
	QBRecordCollection data;
	data.reserve(numRecords);
	for ( uint i = 0; i < numRecords; i++ )
	{
		QBRecord rec = {i, prefix + std::to_string(i), i % 100, std::to_string(i) + prefix};
		data.emplace_back(rec);
	}

	return data;
}

QBRecordCollectionOpt CPPCraftDemo::PopulateDummyDataOpt(const std::string& prefix, int numRecords)
{
	QBRecordCollectionOpt dataNew;
	dataNew.reserve(numRecords);
	for ( uint i = 0; i < numRecords; i++ )
	{
		QBRecord rec = {i, prefix + std::to_string(i), i % 100, std::to_string(i) + prefix};
		dataNew.emplace_back(rec);
	}

	return dataNew;
}

