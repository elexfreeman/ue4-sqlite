#pragma once
#include "Core.h"

//Plugins\SQLite3UE4Plugin\Source\SQLite3UE4Plugin\Classes\SQLiteDatabase.h
//Plugins\SQLite3UE4Plugin\ThirdParty\SQLite3\Include\sqlite3.h

struct FSQLiteQueryLogicExpectedNode
{

	FString Query;

	FSQLiteQueryLogicExpectedNode() {}
	FSQLiteQueryLogicExpectedNode(FString LHSQuery, FString Append) : Query(LHSQuery)
	{
		Query += Append;
	}
};

struct FSQLiteQueryTermExpectedNode
{

	FString Query;

	FSQLiteQueryTermExpectedNode() {}
	FSQLiteQueryTermExpectedNode(FString LHSQuery, FString Append) : Query(LHSQuery)
	{
		Query += Append;
	}
};

struct FSQLiteQueryFinalizedQuery
{

	FString Query;

	FSQLiteQueryFinalizedQuery() {}
	FSQLiteQueryFinalizedQuery(FString Query) : Query(Query) {}
};

struct FSQLiteDatabaseReference
{

	/** The database name (not the filename) */
	FString DatabaseName;

	/** The database tables we want to get data from */
	TArray<FString> Tables;
};

struct FSQLiteKeyValuePair
{

	/** The database table field name */
	FString Key;

	/** The value of the field */
	FString Value;
};

struct FSQLiteQueryResultRow
{

	/** A list of field name, field value pairs */
	TArray<FSQLiteKeyValuePair> Fields;
};

struct FSQLiteQueryResult
{

	/** The resulting rows from the query */
	TArray<FSQLiteQueryResultRow> ResultRows;

	/** Was the query successful or not */
	bool Success;

	/** If the query was unsuccessful a human readable error message will be populated here */
	FString ErrorMessage;
};

// A few things for internal use here.
namespace SQLiteResultValueTypes
{
	enum SQLiteResultValType
	{
		Integer,
		Float,
		Text,
		UnsupportedValueType
	};
};

// Result field, used as an intermediary when collecting results from
// an SQLITE3 query.
struct SQLiteResultField
{
	FString StringValue;
	double DoubleValue;
	int64 IntValue;

	FString Name;
	SQLiteResultValueTypes::SQLiteResultValType Type;

	FString ToString()
	{
		if (Type == SQLiteResultValueTypes::Text)
			return StringValue;
		else if (Type == SQLiteResultValueTypes::Integer)
			return FString::Printf(TEXT("%i"), IntValue);
		else if (Type == SQLiteResultValueTypes::Float)
			return FString::Printf(TEXT("%f"), DoubleValue);

		return StringValue;
	}
};

// Represents a single row in the result.
struct SQLiteResultValue
{
	TArray<SQLiteResultField> Fields;
};

// The internal result object.
struct SQLiteQueryResult
{
	bool Success;
	FString ErrorMessage;
	TArray<SQLiteResultValue> Results;
};
