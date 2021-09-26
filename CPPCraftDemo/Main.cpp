#include "stdafx.h"
#include <algorithm>
#include <assert.h>
#include <chrono>
#include <iostream>
#include <ratio>
#include "CPPCraftDemo.h"

int main(int argc, _TCHAR* argv[])
{
	using namespace std::chrono;

	// populate a bunch of data
	auto data			= CPPCraftDemo::PopulateDummyData("testdata", 1000);
	auto dataNew		= CPPCraftDemo::PopulateDummyDataOpt("testdata", 1000);

	// Find a record that contains and measure the performance

	std::string	matchString1	= "testdata500";
	std::string	matchString2	= "24";
	auto startTimer				= steady_clock::now();
	auto filteredSet			= CPPCraftDemo::QBFindMatchingRecords(data, "column1", matchString1);
	auto filteredSet2			= CPPCraftDemo::QBFindMatchingRecords(data, "column2", matchString2);

	std::cout << "profiler: " << double((steady_clock::now() - startTimer).count()) * steady_clock::period::num / steady_clock::period::den << std::endl;

	// Overloaded
	startTimer = steady_clock::now();
	
	auto filteredSet3 = CPPCraftDemo::QBFindMatchingRecords(data, EColumn::Column1, matchString1);
	auto filteredSet4 = CPPCraftDemo::QBFindMatchingRecords(data, EColumn::Column2, matchString2);

	std::cout << "profiler Overloaded: " << double((steady_clock::now() - startTimer).count()) * steady_clock::period::num / steady_clock::period::den << std::endl;

	// Optimized
	startTimer			= steady_clock::now();
	QBRecordCollectionOpt		filteredSet5;
	dataNew.findRecord(filteredSet5, EColumn::Column1, matchString1);

	QBRecordCollectionOpt		filteredSet6;
	dataNew.findRecord(filteredSet6, EColumn::Column2, matchString2);

	std::cout << "profiler Optimized: " << double((steady_clock::now() - startTimer).count()) * steady_clock::period::num / steady_clock::period::den << std::endl;

	// make sure that the function is correct
	assert(filteredSet.size() == 1);
	assert(filteredSet3.size() == 1);
	assert(filteredSet5.size() == 1);

	dataNew.deleteRecordByID( 15 );

	return 0;
}