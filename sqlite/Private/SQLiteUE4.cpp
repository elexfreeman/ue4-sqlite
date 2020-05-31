// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
#include "../Public/SQLiteUE4.h"
#include "Core.h"
#include "../Public/sqlite3.h"

DEFINE_LOG_CATEGORY(LogDatabase);

#define LOCTEXT_NAMESPACE "FSQLiteUE4"
#define LOGSQLITE(verbosity, text) UE_LOG(LogDatabase, verbosity, TEXT("SQLite: %s"), text)


void FSQLiteUE4::fPrepareStatement(const FString DatabaseName, const FString *Query, sqlite3 **Db, int32 **SqlReturnCode,
										 sqlite3_stmt **PreparedStatement)
{

	ANSICHAR *dbNameAsUtf8 = TCHAR_TO_UTF8(*DatabaseName);

	int32 i = sqlite3_open(dbNameAsUtf8, Db);

	**SqlReturnCode = i;

	ANSICHAR *queryAsUtf8 = TCHAR_TO_UTF8(**Query);

	**SqlReturnCode = sqlite3_prepare_v2(*Db, queryAsUtf8, -1, PreparedStatement, NULL);
}

bool FSQLiteUE4::fExecSql(const FString DatabaseName, const FString Query)
{
	//LOGSQLITE(Warning, *query);

	bool execStatus = false;

	char *zErrMsg = 0;
	sqlite3 *db;

	ANSICHAR *dbNameAsUtf8 = TCHAR_TO_UTF8(*DatabaseName);
	int32 i = sqlite3_open(dbNameAsUtf8, &db);

	if (i == SQLITE_OK)
	{

		int32 k = sqlite3_exec(db, TCHAR_TO_UTF8(*Query), NULL, 0, &zErrMsg);

		if (i == SQLITE_OK)
		{
			execStatus = true;
		}
		else
		{
			LOGSQLITE(Warning, TEXT("CreateTable - Query Exec Failed.."));
		}
	}
	else
	{
		LOGSQLITE(Warning, TEXT("CreateTable - DB Open failed.."));
	}

	sqlite3_close(db);

	return execStatus;
}

SQLiteQueryResult FSQLiteUE4::fRunQueryAndGetResults(FString DatabaseName, FString Query)
{
	SQLiteQueryResult result;

	sqlite3 *db;
	int32 sqlReturnCode = 0;
	int32 *sqlReturnCode1 = &sqlReturnCode;
	sqlite3_stmt *preparedStatement;

	fPrepareStatement(DatabaseName, &Query, &db, &sqlReturnCode1, &preparedStatement);
	sqlReturnCode = *sqlReturnCode1;

	if (sqlReturnCode != SQLITE_OK)
	{
		const char *errorMessage = sqlite3_errmsg(db);
		FString error = "SQL error: " + FString(UTF8_TO_TCHAR(errorMessage));
		LOGSQLITE(Error, *error);
		LOGSQLITE(Error, *FString::Printf(TEXT("The attempted query was: %s"), *Query));
		result.ErrorMessage = error;
		result.Success = false;
		sqlite3_finalize(preparedStatement);
		sqlite3_close(db);
		return result;
	}

	//////////////////////////////////////////////////////////////////////////
	// Get and assign the data
	//////////////////////////////////////////////////////////////////////////

	TArray<SQLiteResultValue> resultRows;

	for (sqlReturnCode = sqlite3_step(preparedStatement);
		 sqlReturnCode != SQLITE_DONE && sqlReturnCode == SQLITE_ROW;
		 sqlReturnCode = sqlite3_step(preparedStatement))
	{
		SQLiteResultValue row;

		LOGSQLITE(Verbose, TEXT("Query returned a result row."));
		int32 resultColumnCount = sqlite3_column_count(preparedStatement);
		for (int32 c = 0; c < resultColumnCount; c++)
		{
			int32 columnType = sqlite3_column_type(preparedStatement, c);
			const char *columnName = sqlite3_column_name(preparedStatement, c);
			FString columnNameStr = UTF8_TO_TCHAR(columnName);
			SQLiteResultField val;
			val.Name = columnNameStr;
			switch (columnType)
			{
			case SQLITE_INTEGER:
				val.Type = SQLiteResultValueTypes::Integer;
				val.IntValue = sqlite3_column_int64(preparedStatement, c);
				break;
			case SQLITE_TEXT:
				val.Type = SQLiteResultValueTypes::Text;
				val.StringValue = UTF8_TO_TCHAR(sqlite3_column_text(preparedStatement, c));
				break;
			case SQLITE_FLOAT:
				val.Type = SQLiteResultValueTypes::Float;
				val.DoubleValue = sqlite3_column_double(preparedStatement, c);
				break;
			case SQLITE_NULL:
			default:
				val.Type = SQLiteResultValueTypes::UnsupportedValueType;
			}

			if (val.Type != SQLiteResultValueTypes::UnsupportedValueType)
			{
				row.Fields.Add(val);
			}
		}

		resultRows.Add(row);
	}

	//////////////////////////////////////////////////////////////////////////
	// Release the statement and close the connection
	//////////////////////////////////////////////////////////////////////////

	sqlite3_finalize(preparedStatement);
	sqlite3_close(db);

	result.Results = resultRows;
	result.Success = true;
	return result;
}

bool FSQLiteUE4::fTest()
{
	return true;
}
