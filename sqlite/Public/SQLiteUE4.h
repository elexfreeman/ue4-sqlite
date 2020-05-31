// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "Core.h"
#include "SQLiteDatabaseAdd.h"
#include "sqlite3.h"

DECLARE_LOG_CATEGORY_EXTERN(LogDatabase, Log, All);

class FSQLiteUE4
{
public:

	// подготовка данных
	static void fPrepareStatement(const FString DatabaseName, const FString* Query, sqlite3** Db, int32** SqlReturnCode,
		sqlite3_stmt** PreparedStatement);

	// выполнить запрос и получить результат
	static SQLiteQueryResult fRunQueryAndGetResults(FString DatabaseName, FString Query);

	// выполнеить запрос без результатов
	static bool fExecSql(const FString DatabaseName, const FString Query);

	// тестовая ф-я проверки модуля
	static bool fTest();

private:	

};
